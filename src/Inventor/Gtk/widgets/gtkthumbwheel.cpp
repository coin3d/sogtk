/**************************************************************************\
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#include <stdio.h>

#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>

#include <Inventor/Gtk/widgets/SoAnyThumbWheel.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

/*
 * gtkthumbwheel - my first GTK+ widget.
 *
 * TODO
 * - cache pregenerated thumbwheel pixmaps
 * - track mouse motion and update wheel values / wheel graphics
 * - create thumbwheel configuration/state methods
 * - implement (emit) and check/debug thumbwheel signals
 * - make widget un-resizable
 * - document some stuff
 *
 * CONSIDER
 * - put thumbwheel into public domain and/or contribute it to The GTK+ Team.
 *   (C-ify source code first)
 */

// *************************************************************************

extern "C" {

void gtk_thumbwheel_update(
  GtkThumbWheel * thumbwheel );
gint gtk_thumbwheel_timer(
  GtkThumbWheel * thumbwheel );
gint gtk_thumbwheel_motion_notify(
  GtkWidget * widget, GdkEventMotion * event );
gint gtk_thumbwheel_button_release(
  GtkWidget * widget, GdkEventButton * event );
gint gtk_thumbwheel_button_press(
  GtkWidget * widget, GdkEventButton * event );
gint gtk_thumbwheel_expose(
  GtkWidget * widget, GdkEventExpose * event );
void gtk_thumbwheel_size_allocate(
  GtkWidget * widget, GtkAllocation * allocation );
void gtk_thumbwheel_size_request(
  GtkWidget * widget, GtkRequisition * requisition );
void gtk_thumbwheel_realize(
  GtkWidget * widget );
void gtk_thumbwheel_destroy(
  GtkObject * object );
void gtk_thumbwheel_init(
  GtkThumbWheel * thumbwheel );
void gtk_thumbwheel_class_init(
  GtkThumbWheelClass * cclass );

}; /* extern "C" */

enum {
    ATTACHED,
    VALUE_CHANGED,
    RELEASED,
    NUM_SIGNALS
};

static GtkWidgetClass * parent_class = NULL;
static guint thumbwheel_signals[NUM_SIGNALS] = { 0 };

static const guint GTK_THUMBWHEEL_DEFAULT_WIDTH = 24;
static const guint GTK_THUMBWHEEL_DEFAULT_LENGTH = 90;

enum {
  THUMBWHEEL_DISABLED,
  THUMBWHEEL_IDLE,
  THUMBWHEEL_DRAGGING
};

// *************************************************************************

guint
gtk_thumbwheel_get_type(
  void )
{
  static guint thumbwheel_type = 0;
  if ( ! thumbwheel_type ) {
    GtkTypeInfo thumbwheel_info = {
      "GtkThumbWheel",
      sizeof(GtkThumbWheel),
      sizeof(GtkThumbWheelClass),
      (GtkClassInitFunc) gtk_thumbwheel_class_init,
      (GtkObjectInitFunc) gtk_thumbwheel_init,
      (GtkArgSetFunc) NULL,
      (GtkArgGetFunc) NULL,
      (GtkClassInitFunc) NULL
    };
    thumbwheel_type =
      gtk_type_unique( gtk_widget_get_type(), &thumbwheel_info );
  }
  return thumbwheel_type;
} // gtk_thumbwheel_get_type()

// *************************************************************************

static
void
gtk_thumbwheel_class_init(
  GtkThumbWheelClass * cclass )
{
  GtkObjectClass * object_class = (GtkObjectClass *) cclass;
  GtkWidgetClass * widget_class = (GtkWidgetClass *) cclass;

  gdk_rgb_init();

  parent_class = (GtkWidgetClass *) gtk_type_class( gtk_widget_get_type() );

  object_class->destroy = gtk_thumbwheel_destroy;

  widget_class->realize = gtk_thumbwheel_realize;
  widget_class->expose_event = gtk_thumbwheel_expose;
  widget_class->size_request = gtk_thumbwheel_size_request;
  widget_class->size_allocate = gtk_thumbwheel_size_allocate;
  widget_class->button_press_event = gtk_thumbwheel_button_press;
  widget_class->motion_notify_event = gtk_thumbwheel_motion_notify;
  widget_class->button_release_event = gtk_thumbwheel_button_release;

  thumbwheel_signals[ ATTACHED ] =
    gtk_signal_new( "attached",
      GTK_RUN_FIRST,
      object_class->type,
      GTK_SIGNAL_OFFSET( GtkThumbWheelClass, attached ),
      gtk_signal_default_marshaller,
      GTK_TYPE_NONE,
      0 );

  thumbwheel_signals[ VALUE_CHANGED ] =
    gtk_signal_new( "value_changed",
      GTK_RUN_FIRST,
      object_class->type,
      GTK_SIGNAL_OFFSET( GtkThumbWheelClass, value_changed ),
      gtk_signal_default_marshaller,
      GTK_TYPE_NONE,
      1,
      GTK_TYPE_FLOAT );

  thumbwheel_signals[ RELEASED ] =
    gtk_signal_new( "released",
      GTK_RUN_FIRST,
      object_class->type,
      GTK_SIGNAL_OFFSET( GtkThumbWheelClass, released ),
      gtk_signal_default_marshaller,
      GTK_TYPE_NONE,
      0 );

  gtk_object_class_add_signals( object_class, thumbwheel_signals, NUM_SIGNALS );

  cclass->attached      = NULL;
  cclass->value_changed = NULL;
  cclass->released      = NULL;

} // gtk_thumbwheel_class_init()

// *************************************************************************

static
void
gtk_thumbwheel_init(
  GtkThumbWheel * thumbwheel )
{
  thumbwheel->state = THUMBWHEEL_IDLE;
  thumbwheel->policy = 0;
  thumbwheel->button = 0;
  thumbwheel->vertical = 0;
  thumbwheel->downpos = 0;
  thumbwheel->value = 0.0f;
  thumbwheel->tempvalue = 0.0f;
  SoAnyThumbWheel * wheel = new SoAnyThumbWheel;
  wheel->SetWheelMotionMethod( SoAnyThumbWheel::UNIFORM );
  wheel->SetWheelRangeBoundaryHandling( SoAnyThumbWheel::MODULATE );
  thumbwheel->wheel = (void *) wheel;
} // gtk_thumbwheel_init()

// *************************************************************************

GtkWidget *
gtk_thumbwheel_new(
  gint vertical )
{
  GtkThumbWheel * thumbwheel =
    GTK_THUMBWHEEL(gtk_type_new( gtk_thumbwheel_get_type() ));
  thumbwheel->vertical = (vertical != FALSE) ? 1 : 0;
  return GTK_WIDGET(thumbwheel);
} // gtk_thumbwheel_new()

// *************************************************************************

static
void
gtk_thumbwheel_destroy(
  GtkObject * object )
{
  g_return_if_fail( object != NULL );
  g_return_if_fail( GTK_IS_THUMBWHEEL(object) );
  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(object);
  if ( GTK_OBJECT_CLASS(parent_class)->destroy )
    (*GTK_OBJECT_CLASS(parent_class)->destroy)( object );
} // gtk_thumbwheel_destroy()

// *************************************************************************

void
get_thumbwheel_set_update_policy(
  GtkThumbWheel * thumbwheel,
  GtkUpdateType update )
{
  g_return_if_fail( thumbwheel != NULL );
  g_return_if_fail( GTK_IS_THUMBWHEEL(thumbwheel) );
  thumbwheel->policy = update;
} // get_thumbwheel_set_update_policy()

// *************************************************************************

static
void
gtk_thumbwheel_realize(
  GtkWidget * widget )
{
  gint attributes_mask;

  g_return_if_fail( widget != NULL );
  g_return_if_fail( GTK_IS_THUMBWHEEL(widget) );

  GTK_WIDGET_SET_FLAGS( widget, GTK_REALIZED );
  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);

  GdkWindowAttr attributes;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events( widget ) |
    GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
/*  GDK_BUTTON_MOTION_MASK | GDK_BUTTON1_MOTION_MASK |
    GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK | */
    GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK;
  attributes.visual = gtk_widget_get_visual( widget );
  attributes.colormap = gtk_widget_get_colormap( widget );
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window =
    gdk_window_new( widget->parent->window, &attributes, attributes_mask );
  widget->style = gtk_style_attach( widget->style, widget->window );
  gdk_window_set_user_data( widget->window, widget );
  gtk_style_set_background( widget->style, widget->window, GTK_STATE_ACTIVE );
} // gtk_thumbwheel_realize()

// *************************************************************************

static
void
gtk_thumbwheel_size_request(
  GtkWidget * widget,
  GtkRequisition * requisition )
{
  g_return_if_fail( widget != NULL );
  g_return_if_fail( GTK_IS_THUMBWHEEL(widget) );
  g_return_if_fail( requisition != NULL );

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);
  if ( thumbwheel->vertical ) {
    requisition->width = GTK_THUMBWHEEL_DEFAULT_WIDTH;
    requisition->height = GTK_THUMBWHEEL_DEFAULT_LENGTH;
  } else {
    requisition->width = GTK_THUMBWHEEL_DEFAULT_LENGTH;
    requisition->height = GTK_THUMBWHEEL_DEFAULT_WIDTH;
  }
} // gtk_thumbwheel_size_request()

// *************************************************************************

static
void
gtk_thumbwheel_size_allocate(
  GtkWidget * widget,
  GtkAllocation * allocation )
{
  g_return_if_fail( widget != NULL );
  g_return_if_fail( GTK_IS_THUMBWHEEL(widget) );
  g_return_if_fail( allocation != NULL );

  widget->allocation = *allocation;
  if ( GTK_WIDGET_REALIZED(widget) ) {
    GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);
    gdk_window_move_resize( widget->window,
      allocation->x, allocation->y, allocation->width, allocation->height );
  }
} // gtk_thumbwheel_size_allocate()

// *************************************************************************

static
gint
gtk_thumbwheel_expose(
  GtkWidget * widget,
  GdkEventExpose * event )
{
  g_return_val_if_fail( widget != NULL, FALSE );
  g_return_val_if_fail( GTK_IS_THUMBWHEEL(widget), FALSE );
  g_return_val_if_fail( event != NULL, FALSE );

  if ( event->count > 0 )
    return FALSE;

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);

  gdk_window_clear_area( widget->window,
    0, 0, widget->allocation.width, widget->allocation.height );

  gdk_draw_line( widget->window,
    widget->style->light_gc[GTK_WIDGET_STATE(widget)],
    0, 0, widget->allocation.width - 1, 0 );
  gdk_draw_line( widget->window,
    widget->style->light_gc[GTK_WIDGET_STATE(widget)],
    1, 1, widget->allocation.width - 2, 1 );

  gdk_draw_line( widget->window,
    widget->style->light_gc[GTK_WIDGET_STATE(widget)],
    0, 0, 0, widget->allocation.height - 1 );
  gdk_draw_line( widget->window,
    widget->style->light_gc[GTK_WIDGET_STATE(widget)],
    1, 1, 1, widget->allocation.height - 2 );

  gdk_draw_line( widget->window,
    widget->style->dark_gc[GTK_WIDGET_STATE(widget)],
                               1, widget->allocation.height - 1,
    widget->allocation.width - 1, widget->allocation.height - 1 );
  gdk_draw_line( widget->window,
    widget->style->dark_gc[GTK_WIDGET_STATE(widget)],
                               2, widget->allocation.height - 2,
    widget->allocation.width - 2, widget->allocation.height - 2 );

  gdk_draw_line( widget->window,
    widget->style->dark_gc[GTK_WIDGET_STATE(widget)],
    widget->allocation.width - 1, 1,
    widget->allocation.width - 1, widget->allocation.height - 1 );

  gdk_draw_line( widget->window,
    widget->style->dark_gc[GTK_WIDGET_STATE(widget)],
    widget->allocation.width - 2, 2,
    widget->allocation.width - 2, widget->allocation.height - 1 );

  if ( thumbwheel->vertical != 0 ) {
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      5, 9, 5, widget->allocation.height - 10 );
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      5, 9, widget->allocation.width - 6, 9 );
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      widget->allocation.width - 6, 9,
      widget->allocation.width - 6, widget->allocation.height - 10 );
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      5, widget->allocation.height - 10,
      widget->allocation.width - 6, widget->allocation.height - 10 );
  } else {
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      9, 5,
      9, widget->allocation.height - 6 );
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      9, 5,
      widget->allocation.width - 10, 5 );
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      widget->allocation.width - 10, 5,
      widget->allocation.width - 10, widget->allocation.height - 6 );
    gdk_draw_line( widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
      9, widget->allocation.height - 6,
      widget->allocation.width - 10, widget->allocation.height - 6 );
  }

  static guchar bitmap[90*26*4*4]; // just "enough" for now

  if ( thumbwheel->vertical != 0 ) {
    ((SoAnyThumbWheel *)thumbwheel->wheel)->SetWheelSize(
      widget->allocation.height - 20, widget->allocation.width - 12 );
  } else {
    ((SoAnyThumbWheel *)thumbwheel->wheel)->SetWheelSize(
      widget->allocation.width - 20, widget->allocation.height - 12 );
  }

  int img = ((SoAnyThumbWheel *)thumbwheel->wheel)->GetBitmapForValue(
    thumbwheel->tempvalue, SoAnyThumbWheel::ENABLED );

  ((SoAnyThumbWheel *)thumbwheel->wheel)->SetGraphicsByteOrder( SoAnyThumbWheel::ABGR );
  ((SoAnyThumbWheel *)thumbwheel->wheel)->SetColor( 0, 150, 200 );
  ((SoAnyThumbWheel *)thumbwheel->wheel)->DrawBitmap( img, (void *) bitmap,
    (thumbwheel->vertical != 0) ? SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL );

  if ( thumbwheel->vertical != 0 ) {
    gdk_draw_rgb_32_image(
      widget->window, widget->style->fg_gc[ GTK_WIDGET_STATE(widget) ],
      6, 10, widget->allocation.width - 12, widget->allocation.height - 20,
      GDK_RGB_DITHER_NONE, bitmap, (widget->allocation.width - 12) * 4 );
  } else {
    gdk_draw_rgb_32_image(
      widget->window, widget->style->fg_gc[ GTK_WIDGET_STATE(widget) ],
      10, 6, widget->allocation.width - 20, widget->allocation.height - 12,
      GDK_RGB_DITHER_NONE, bitmap, (widget->allocation.width - 20) * 4 );
  }

  return FALSE;
} // gtk_thumbwheel_expose()

// *************************************************************************

static
gint
gtk_thumbwheel_button_press(
  GtkWidget * widget,
  GdkEventButton * event )
{
  g_return_val_if_fail( widget != NULL, FALSE );
  g_return_val_if_fail( GTK_IS_THUMBWHEEL(widget), FALSE );
  g_return_val_if_fail( event != NULL, FALSE );

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);

  if ( thumbwheel->state != THUMBWHEEL_IDLE )
    return FALSE;


  fprintf( stderr, "> press\n" );

  if ( thumbwheel->vertical != 0 ) {
    thumbwheel->downpos = (gint) event->y;
  } else {
    thumbwheel->downpos = (gint) event->x;
  }

  thumbwheel->state = THUMBWHEEL_DRAGGING;

  return FALSE;
} // gtk_thumbwheel_button_press()

// *************************************************************************

static
gint
gtk_thumbwheel_button_release(
  GtkWidget * widget,
  GdkEventButton * event )
{
  g_return_val_if_fail( widget != NULL, FALSE );
  g_return_val_if_fail( GTK_IS_THUMBWHEEL(widget), FALSE );
  g_return_val_if_fail( event != NULL, FALSE );

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);

  if ( thumbwheel->state != THUMBWHEEL_DRAGGING )
    return FALSE;

  fprintf( stderr, "> release\n" );

  thumbwheel->tempvalue = thumbwheel->value;
  thumbwheel->state = THUMBWHEEL_IDLE;

  return FALSE;
} // gtk_thumbwheel_button_release()

// *************************************************************************

/*
  The gtk_timeout_add() method is probably not the correct way to track
  the mouse, but I haven't found another better method yet...
*/

static
gint
gtk_thumbwheel_motion(
  GtkWidget * widget )
{
  g_return_val_if_fail( widget != NULL, FALSE );
  g_return_val_if_fail( GTK_IS_THUMBWHEEL(widget), FALSE );

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);
  if ( thumbwheel->state != THUMBWHEEL_DRAGGING )
    return FALSE;

  gint x, y;
  GdkModifierType mods;
  gdk_window_get_pointer( widget->window, &x, &y, &mods );

  gint position = ( thumbwheel->vertical != FALSE ) ? y : x;

  thumbwheel->tempvalue =
    ((SoAnyThumbWheel *)thumbwheel->wheel)->CalculateValue( thumbwheel->value,
      thumbwheel->downpos, position - thumbwheel->downpos );

  static int bmp = -1;
  int img = ((SoAnyThumbWheel *)thumbwheel->wheel)->GetBitmapForValue(
    thumbwheel->tempvalue, SoAnyThumbWheel::ENABLED );
  if ( img != bmp )
    gtk_widget_draw( GTK_WIDGET(thumbwheel), NULL );
  bmp = img;

  gtk_timeout_add( 100, gtk_thumbwheel_motion, (gpointer) thumbwheel );
  return FALSE;
} // gtk_thumbwheel_motion()


static
gint
gtk_thumbwheel_motion_notify(
  GtkWidget * widget,
  GdkEventMotion * event )
{
  g_return_val_if_fail( widget != NULL, FALSE );
  g_return_val_if_fail( GTK_IS_THUMBWHEEL(widget), FALSE );
  g_return_val_if_fail( event != NULL, FALSE );

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);
  if ( thumbwheel->state != THUMBWHEEL_DRAGGING )
    return FALSE;

  gtk_widget_draw( GTK_WIDGET(thumbwheel), NULL );
  gtk_timeout_add( 100, gtk_thumbwheel_motion, (gpointer) thumbwheel );
  return FALSE;
} // gtk_thumbwheel_motion_notify()

// *************************************************************************

void
gtk_thumbwheel_set_value(
  GtkThumbWheel * thumbwheel,
  gfloat value )
{
  thumbwheel->value = value;
} // gtk_thumbwheel_set_value()

// *************************************************************************

gfloat
gtk_thumbwheel_get_value(
  GtkThumbWheel * thumbwheel )
{
  return thumbwheel->value;
} // gtk_thumbwheel_get_value()

// *************************************************************************

void
gtk_thumbwheel_enable(
  GtkThumbWheel * thumbwheel )
{
  thumbwheel->state = THUMBWHEEL_IDLE;
} // gtk_thumbwheel_enable()

// *************************************************************************

void
gtk_thumbwheel_disable(
  GtkThumbWheel * thumbwheel )
{
  thumbwheel->state = THUMBWHEEL_DISABLED;
} // gtk_thumbwheel_disable()

// *************************************************************************

void
gtk_thumbwheel_set_motion_method(
  GtkThumbWheel * thumbwheel )
{
} // gtk_thumbwheel_set_motion_method()

// *************************************************************************

void
gtk_thumbwheel_set_range_boundary_handling(
  GtkThumbWheel * thumbwheel )
{
} // gtk_thumbwheel_set_range_boundary_handling()

// *************************************************************************
