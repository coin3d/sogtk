/**************************************************************************
 *
 *  This file is part of the Coin SoGtk GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin SoGtk for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#include <stdio.h>

#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>

#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/widgets/SoAnyThumbWheel.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

/*
 * gtkthumbwheel - my first GTK+ widget.
 *
 * TODO
 * - make sure mousedown actually happens on wheel
 * - cache pregenerated thumbwheel pixmaps
 * - track mouse motion and update wheel values / wheel graphics
 * - create thumbwheel configuration/state methods
 * - make widget un-resizable
 *
 * CONSIDER
 * - put thumbwheel into public domain and/or contribute it to The GTK+ Team.
 *   (C-ify source code first)
 */

// *************************************************************************

extern "C" {
static gint gtk_thumbwheel_motion_notify(GtkWidget * widget, GdkEventMotion * event);
static gint gtk_thumbwheel_button_release(GtkWidget * widget, GdkEventButton * event);
static gint gtk_thumbwheel_button_press(GtkWidget * widget, GdkEventButton * event);
static gint gtk_thumbwheel_expose(GtkWidget * widget, GdkEventExpose * event);
static void gtk_thumbwheel_size_allocate(GtkWidget * widget, GtkAllocation * allocation);
static void gtk_thumbwheel_realize(GtkWidget * widget);
static void gtk_thumbwheel_destroy(GtkObject * object);
static void gtk_thumbwheel_init(GtkThumbWheel * thumbwheel);
static void gtk_thumbwheel_class_init(GtkThumbWheelClass * cclass);
static void gtk_thumbwheel_value_changed (GtkThumbWheel        *thumbwheel);

static void gtk_thumbwheel_paint(GtkWidget * widget, GdkRectangle * area);
}; /* extern "C" */

enum {
  ATTACHED,
  VALUE_CHANGED,
  RELEASED,
  NUM_SIGNALS
};

static GtkMiscClass * parent_class = NULL;
static guint thumbwheel_signals[NUM_SIGNALS] = { 0 };

/* Default sizes of the widget */
static const guint GTK_THUMBWHEEL_DEFAULT_NARROW = 16 ;
static const guint GTK_THUMBWHEEL_DEFAULT_LENGTH = 122 ;

/* Padding between border of the widget and the thumbwheel */
static const guint GTK_THUMBWHEEL_PAD_NARROW = 2 ;
static const guint GTK_THUMBWHEEL_PAD_LENGTH = 4 ;

/* Sizes of the thumbwheel */
static const guint GTK_THUMBWHEEL_WHEEL_NARROW =
  (GTK_THUMBWHEEL_DEFAULT_NARROW - 2 * GTK_THUMBWHEEL_PAD_NARROW) ;
static const guint GTK_THUMBWHEEL_WHEEL_LENGTH =
  (GTK_THUMBWHEEL_DEFAULT_LENGTH - 2 * GTK_THUMBWHEEL_PAD_LENGTH) ;

enum {
  THUMBWHEEL_IDLE,
  THUMBWHEEL_DRAGGING
};

// *************************************************************************

GtkType
gtk_thumbwheel_get_type(
  void)
{
  static GtkType thumbwheel_type = 0;

  if (! thumbwheel_type)
  {
    static const GtkTypeInfo thumbwheel_info = {
      "GtkThumbWheel",
      sizeof(GtkThumbWheel),
      sizeof(GtkThumbWheelClass),
      (GtkClassInitFunc) gtk_thumbwheel_class_init,
      (GtkObjectInitFunc) gtk_thumbwheel_init,
      NULL,
      NULL,
      (GtkClassInitFunc) NULL
    };
    thumbwheel_type = 
      gtk_type_unique (GTK_TYPE_MISC, &thumbwheel_info);
  }

  return thumbwheel_type;
} // gtk_thumbwheel_get_type()

// *************************************************************************

static
void
gtk_thumbwheel_class_init(
  GtkThumbWheelClass * cclass)
{
  GtkObjectClass * object_class = (GtkObjectClass *) cclass;
  GtkWidgetClass * widget_class = (GtkWidgetClass *) cclass;

  gdk_rgb_init();

  parent_class = (GtkMiscClass *) gtk_type_class (GTK_TYPE_MISC);

  object_class->destroy = gtk_thumbwheel_destroy;

  widget_class->realize = gtk_thumbwheel_realize;
  widget_class->expose_event = gtk_thumbwheel_expose;
  widget_class->size_allocate = gtk_thumbwheel_size_allocate;
  widget_class->button_press_event = gtk_thumbwheel_button_press;
  widget_class->motion_notify_event = gtk_thumbwheel_motion_notify;
  widget_class->button_release_event = gtk_thumbwheel_button_release;

  thumbwheel_signals[ ATTACHED ] =
    gtk_signal_new("attached",
      GTK_RUN_FIRST,
      object_class->type,
      GTK_SIGNAL_OFFSET(GtkThumbWheelClass, attached),
      gtk_signal_default_marshaller,
      GTK_TYPE_NONE, 0);

  thumbwheel_signals[ VALUE_CHANGED ] =
    gtk_signal_new("value_changed",
      GTK_RUN_FIRST,
      object_class->type,
      GTK_SIGNAL_OFFSET(GtkThumbWheelClass, value_changed),
      gtk_signal_default_marshaller,
      GTK_TYPE_NONE, 0);

  thumbwheel_signals[ RELEASED ] =
    gtk_signal_new("released",
      GTK_RUN_FIRST,
      object_class->type,
      GTK_SIGNAL_OFFSET(GtkThumbWheelClass, released),
      gtk_signal_default_marshaller,
      GTK_TYPE_NONE, 0);

  gtk_object_class_add_signals(object_class, thumbwheel_signals, NUM_SIGNALS);

  cclass->attached      = NULL;
  cclass->value_changed = NULL;
  cclass->released      = NULL;

} // gtk_thumbwheel_class_init()

// *************************************************************************

static
void
gtk_thumbwheel_init(
  GtkThumbWheel * thumbwheel)
{
  thumbwheel->state = THUMBWHEEL_IDLE;
  thumbwheel->policy = 0;
  thumbwheel->button = 0;
  thumbwheel->vertical = 0;
  thumbwheel->downpos = 0;
  thumbwheel->value = 0.0f;
  thumbwheel->tempvalue = 0.0f;
  thumbwheel->bitmap = 0 ;

  SoAnyThumbWheel * wheel = new SoAnyThumbWheel;
  wheel->setMovement(SoAnyThumbWheel::UNIFORM);
  wheel->setBoundaryHandling(SoAnyThumbWheel::ACCUMULATE);
  wheel->setGraphicsByteOrder(SoAnyThumbWheel::ABGR);

  thumbwheel->wheel = (void *) wheel;

} // gtk_thumbwheel_init()

// *************************************************************************

GtkWidget *
gtk_thumbwheel_new(
  gint vertical)
{
  GtkThumbWheel *thumbwheel ;
  GtkMisc       *misc ;

  thumbwheel = (GtkThumbWheel *) gtk_type_new (GTK_TYPE_THUMBWHEEL);
  misc = GTK_MISC(thumbwheel) ;

  thumbwheel->vertical = (vertical != FALSE) ? 1 : 0;
  if (thumbwheel->vertical)
  {  
    thumbwheel->wheel_area.x = GTK_THUMBWHEEL_PAD_NARROW + misc->xpad ;
    thumbwheel->wheel_area.y = GTK_THUMBWHEEL_PAD_LENGTH + misc->ypad ;
    thumbwheel->wheel_area.width = GTK_THUMBWHEEL_WHEEL_NARROW ;
    thumbwheel->wheel_area.height = GTK_THUMBWHEEL_WHEEL_LENGTH ;

    GTK_WIDGET (thumbwheel)->requisition.width = 
      GTK_THUMBWHEEL_DEFAULT_NARROW + misc->xpad * 2;
    GTK_WIDGET (thumbwheel)->requisition.height =  
      GTK_THUMBWHEEL_DEFAULT_LENGTH + misc->ypad * 2;
  }
  else
  {
    thumbwheel->wheel_area.x = GTK_THUMBWHEEL_PAD_LENGTH + misc->xpad ;
    thumbwheel->wheel_area.y = GTK_THUMBWHEEL_PAD_NARROW + misc->ypad ;
    thumbwheel->wheel_area.width = GTK_THUMBWHEEL_WHEEL_LENGTH ;
    thumbwheel->wheel_area.height = GTK_THUMBWHEEL_WHEEL_NARROW ;

    GTK_WIDGET (thumbwheel)->requisition.width = 
      GTK_THUMBWHEEL_DEFAULT_LENGTH + misc->xpad * 2;
    GTK_WIDGET (thumbwheel)->requisition.height =  
      GTK_THUMBWHEEL_DEFAULT_NARROW + misc->ypad * 2;
  }

  return GTK_WIDGET (thumbwheel);
} // gtk_thumbwheel_new()

// *************************************************************************

static
void
gtk_thumbwheel_destroy(
  GtkObject * object)
{
  GtkThumbWheel *thumbwheel ;

  g_return_if_fail(object != NULL);
  g_return_if_fail(GTK_IS_THUMBWHEEL(object));

  thumbwheel = GTK_THUMBWHEEL(object);
  g_free((gpointer) thumbwheel->bitmap);

  delete (SoAnyThumbWheel*) thumbwheel->wheel;

  if (GTK_OBJECT_CLASS(parent_class)->destroy)
    (*GTK_OBJECT_CLASS(parent_class)->destroy)(object);
} // gtk_thumbwheel_destroy()

// *************************************************************************

void
gtk_thumbwheel_set_update_policy(
  GtkThumbWheel * thumbwheel,
  GtkUpdateType update)
{
  g_return_if_fail(thumbwheel != NULL);
  g_return_if_fail(GTK_IS_THUMBWHEEL(thumbwheel));
  thumbwheel->policy = update;
} // get_thumbwheel_set_update_policy()

// *************************************************************************

static
void
gtk_thumbwheel_realize(
  GtkWidget * widget)
{
  gint attributes_mask;

  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_THUMBWHEEL(widget));

  GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);

  GdkWindowAttr attributes;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = gtk_widget_get_events(widget) |
    GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK;
  attributes.visual = gtk_widget_get_visual(widget);
  attributes.colormap = gtk_widget_get_colormap(widget);
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window =
    gdk_window_new(gtk_widget_get_parent_window(widget),
      &attributes, attributes_mask);
  widget->style = gtk_style_attach(widget->style, widget->window);
  gdk_window_set_user_data(widget->window, widget);
  gtk_style_set_background(widget->style, widget->window, GTK_STATE_NORMAL);

  // Initially set up everything
  GtkThumbWheel *thumbwheel = GTK_THUMBWHEEL(widget);

  thumbwheel->img =
    ((SoAnyThumbWheel *) thumbwheel->wheel)->
    getBitmapForValue (thumbwheel->tempvalue,
      GTK_WIDGET_IS_SENSITIVE(widget) ? 
        SoAnyThumbWheel::ENABLED : SoAnyThumbWheel::DISABLED);

  ((SoAnyThumbWheel *) thumbwheel->wheel)->drawBitmap (thumbwheel->img, 
     (void *) thumbwheel->bitmap, 
     (thumbwheel->vertical != 0) ?
        SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);
} // gtk_thumbwheel_realize()

// *************************************************************************

static
void
gtk_thumbwheel_size_allocate(
  GtkWidget * widget,
  GtkAllocation * allocation)
{
  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_THUMBWHEEL(widget));
  g_return_if_fail(allocation != NULL);

  widget->allocation = *allocation;

  GtkThumbWheel *thumbwheel = GTK_THUMBWHEEL (widget);

  // This is where we finally know the sizes
  thumbwheel->wheel_area.x = 
    + widget->allocation.width / 2 
    - thumbwheel->wheel_area.width / 2 ; 
  thumbwheel->wheel_area.y = 
    + widget->allocation.height / 2 
    - thumbwheel->wheel_area.height / 2 ; 

  // urgh, but ...
  if (thumbwheel->bitmap)
    g_free((gpointer) thumbwheel->bitmap);
  thumbwheel->bitmap = g_new(guchar, 
      thumbwheel->wheel_area.width * thumbwheel->wheel_area.height * 4 * 4);
	// just "enough" for now

  ((SoAnyThumbWheel *) thumbwheel->wheel)->setSize (
      GTK_THUMBWHEEL_WHEEL_LENGTH, 
      GTK_THUMBWHEEL_WHEEL_NARROW);

  thumbwheel->img = 0 ;  

  if (GTK_WIDGET_REALIZED (widget))
  {
    gdk_window_move_resize (widget->window,
	allocation->x, allocation->y, 
        allocation->width, allocation->height);
  }
} // gtk_thumbwheel_size_allocate()

// *************************************************************************

static
gint
gtk_thumbwheel_expose(
  GtkWidget * widget,
  GdkEventExpose * event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_THUMBWHEEL(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);

  if (GTK_WIDGET_DRAWABLE(widget))
    {
      GtkThumbWheel *thumbwheel = GTK_THUMBWHEEL (widget);
      SoAnyThumbWheel *anywheel = (SoAnyThumbWheel *) thumbwheel->wheel ;

      /* setting sensitivity generates expose events */
      int img = anywheel->getBitmapForValue (thumbwheel->tempvalue,
        GTK_WIDGET_IS_SENSITIVE(widget) ?
          SoAnyThumbWheel::ENABLED : SoAnyThumbWheel::DISABLED);

      if (img != thumbwheel->img)
      {
        anywheel->drawBitmap (img,
          (void *) thumbwheel->bitmap,
          (thumbwheel->vertical != 0) ?
          SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);
        thumbwheel->img = img ;
      }

      gtk_thumbwheel_paint(widget, &event->area);
    }

  return FALSE;
} // gtk_thumbwheel_expose()

// *************************************************************************

static
gint
gtk_thumbwheel_button_press(
  GtkWidget * widget,
  GdkEventButton * event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_THUMBWHEEL (widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);

  if ((event->button == 1) && (thumbwheel->state == THUMBWHEEL_IDLE))
  {
    gdk_pointer_grab(widget->window, FALSE,
      GdkEventMask(GDK_POINTER_MOTION_HINT_MASK
      | GDK_BUTTON1_MOTION_MASK   
      | GDK_BUTTON_RELEASE_MASK),  
      NULL, NULL, event->time);   

    if (thumbwheel->vertical != 0)
      thumbwheel->downpos = (gint) event->y;
    else
      thumbwheel->downpos = (gint) event->x;

    thumbwheel->state = THUMBWHEEL_DRAGGING;

    gtk_signal_emit(GTK_OBJECT (widget), thumbwheel_signals[ ATTACHED ]);

    return TRUE ;
  }

  return FALSE;
} // gtk_thumbwheel_button_press()

// *************************************************************************

static
gint
gtk_thumbwheel_button_release(
  GtkWidget * widget,
  GdkEventButton * event)
{
  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_THUMBWHEEL(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);

  GtkThumbWheel * thumbwheel = GTK_THUMBWHEEL(widget);

  if ((thumbwheel->state == THUMBWHEEL_DRAGGING) && (event->button == 1))
  {

    thumbwheel->value = thumbwheel->tempvalue;
    thumbwheel->state = THUMBWHEEL_IDLE;
    gdk_pointer_ungrab (event->time);

    gtk_signal_emit (GTK_OBJECT (widget), thumbwheel_signals[ RELEASED ]);

    return TRUE ;
  }

  return FALSE;
} // gtk_thumbwheel_button_release()

// *************************************************************************

static
gint
gtk_thumbwheel_motion_notify(
  GtkWidget * widget,
  GdkEventMotion * event)
{
  GtkThumbWheel *thumbwheel ;
  gfloat position ;

  g_return_val_if_fail(widget != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_THUMBWHEEL(widget), FALSE);
  g_return_val_if_fail(event != NULL, FALSE);

  thumbwheel = GTK_THUMBWHEEL(widget);

  if (thumbwheel->state == THUMBWHEEL_DRAGGING)
  {
    if (event->is_hint)
    {
      gint x, y ;

      gdk_window_get_pointer(widget->window, &x , &y, NULL);
      position =  (thumbwheel->vertical != FALSE) ? y : x ;
    }
    else
    {
      position = (thumbwheel->vertical != FALSE) ? event->y : event->x;
    }

    thumbwheel->tempvalue =
      ((SoAnyThumbWheel *) thumbwheel->wheel)->
        calculateValue (thumbwheel->value, thumbwheel->downpos,
	  position - thumbwheel->downpos);

    int img = ((SoAnyThumbWheel *) thumbwheel->wheel)->
      getBitmapForValue (thumbwheel->tempvalue, SoAnyThumbWheel::ENABLED);

    if (img != thumbwheel->img)
    {
      ((SoAnyThumbWheel *) thumbwheel->wheel)->drawBitmap (img, 
        (void *) thumbwheel->bitmap, (thumbwheel->vertical != 0) ?
	  SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);
      thumbwheel->img = img ;

      gtk_thumbwheel_paint (GTK_WIDGET (thumbwheel), NULL);
    }

    gtk_thumbwheel_value_changed(thumbwheel);

    return TRUE ;
  }

  return FALSE;
} // gtk_thumbwheel_motion_notify()

// *************************************************************************

void
gtk_thumbwheel_set_value (GtkThumbWheel        *thumbwheel,
                          gfloat                value)
{
  g_return_if_fail (thumbwheel != NULL);
  g_return_if_fail (GTK_IS_THUMBWHEEL (thumbwheel));

  value = ((SoAnyThumbWheel*) thumbwheel->wheel) ->calculateValue(value, 0, 0);

  if (thumbwheel->state == THUMBWHEEL_DRAGGING)
    {
      thumbwheel->tempvalue = value;
    }
  else
    {
      thumbwheel->value = value;
    }
} // gtk_thumbwheel_set_value()

void
gtk_thumbwheel_value_changed (GtkThumbWheel        *thumbwheel)
{
  g_return_if_fail (thumbwheel != NULL);
  g_return_if_fail (GTK_IS_THUMBWHEEL (thumbwheel));

  gtk_signal_emit (GTK_OBJECT (thumbwheel),
    thumbwheel_signals[VALUE_CHANGED], 0);
}
 

// *************************************************************************

gfloat
gtk_thumbwheel_get_value(
  GtkThumbWheel * thumbwheel)
{
  if (thumbwheel->state == THUMBWHEEL_DRAGGING)
    return thumbwheel->tempvalue;
  return thumbwheel->value;
} // gtk_thumbwheel_get_value()

// *************************************************************************

void
gtk_thumbwheel_set_motion_method(
  GtkThumbWheel 	* thumbwheel,
  GtkThumbWheelMotionType	method)
{
  SoAnyThumbWheel *any_thumbwheel ;

  g_return_if_fail (thumbwheel != NULL);
  g_return_if_fail (GTK_IS_THUMBWHEEL (thumbwheel));

  any_thumbwheel = (SoAnyThumbWheel*) thumbwheel->wheel ;

  switch (method)
  {
    case GTK_THUMBWHEEL_MOTION_UNIFORM:
      any_thumbwheel->setMovement(SoAnyThumbWheel::UNIFORM);
      break ;
    case GTK_THUMBWHEEL_MOTION_AUTHENTIC:
      any_thumbwheel->setMovement(SoAnyThumbWheel::AUTHENTIC);
      break ;
    default:
      g_assert(0 && "impossible");
  }
} // gtk_thumbwheel_set_motion_method()

GtkThumbWheelMotionType
gtk_thumbwheel_get_motion_method(
  GtkThumbWheel 			* thumbwheel)
{
  SoAnyThumbWheel *any_thumbwheel ;
  GtkThumbWheelMotionType	method = GTK_THUMBWHEEL_MOTION_UNIFORM;

  g_return_val_if_fail (thumbwheel != NULL, method);
  g_return_val_if_fail (GTK_IS_THUMBWHEEL (thumbwheel), method);

  any_thumbwheel = (SoAnyThumbWheel*) thumbwheel->wheel ;

  switch (any_thumbwheel->getMovement()) {
  case SoAnyThumbWheel::UNIFORM:
    method = GTK_THUMBWHEEL_MOTION_UNIFORM;
    break ;
  case SoAnyThumbWheel::AUTHENTIC:
    method = GTK_THUMBWHEEL_MOTION_AUTHENTIC;
    break ;
  default :
    g_assert(0 && "impossible");
  }
  return method ;
}

// *************************************************************************

void
gtk_thumbwheel_set_range_boundary_handling(
  GtkThumbWheel 			* thumbwheel,
  GtkThumbWheelBoundaryType	handling)
{
  SoAnyThumbWheel *any_thumbwheel ;

  g_return_if_fail (thumbwheel != NULL);
  g_return_if_fail (GTK_IS_THUMBWHEEL (thumbwheel));

  any_thumbwheel = (SoAnyThumbWheel*) thumbwheel->wheel ;

  switch (handling)
  {
    case GTK_THUMBWHEEL_BOUNDARY_CLAMP:
      any_thumbwheel->setBoundaryHandling(SoAnyThumbWheel::CLAMP);
      break ;
    case GTK_THUMBWHEEL_BOUNDARY_MODULATE:
      any_thumbwheel->setBoundaryHandling(SoAnyThumbWheel::MODULATE);
      break ;
    case GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE:
      any_thumbwheel->setBoundaryHandling(SoAnyThumbWheel::ACCUMULATE);
      break ;
    default:
      g_assert(0 && "impossible");
  }
} // gtk_thumbwheel_set_range_boundary_handling()

GtkThumbWheelBoundaryType
gtk_thumbwheel_get_range_boundary_handling(
  GtkThumbWheel 			* thumbwheel)
{
  GtkThumbWheelBoundaryType handling = 
    GTK_THUMBWHEEL_BOUNDARY_CLAMP ;
  SoAnyThumbWheel *any_thumbwheel ;

  g_return_val_if_fail (thumbwheel != NULL, handling);
  g_return_val_if_fail (GTK_IS_THUMBWHEEL (thumbwheel), handling);

  any_thumbwheel = (SoAnyThumbWheel*) thumbwheel->wheel ;

  switch (any_thumbwheel->getBoundaryHandling()) {
  case SoAnyThumbWheel::CLAMP:
    handling = GTK_THUMBWHEEL_BOUNDARY_CLAMP;
    break ;
  case SoAnyThumbWheel::MODULATE:
    handling = GTK_THUMBWHEEL_BOUNDARY_MODULATE;
    break ;
  case SoAnyThumbWheel::ACCUMULATE:
    handling = GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE;
    break ;
  default:
    g_assert(0 && "impossible");
  }

  return handling ;
}

// *************************************************************************

static
void
gtk_thumbwheel_paint (
  GtkWidget * widget,
  GdkRectangle * area)
{
  GtkThumbWheel *thumbwheel;

  if (GTK_WIDGET_DRAWABLE (widget))
  {
    thumbwheel = GTK_THUMBWHEEL (widget);

    gdk_draw_rectangle(widget->window,
      widget->style->black_gc,
      FALSE,
      thumbwheel->wheel_area.x - 1,
      thumbwheel->wheel_area.y - 1,
      thumbwheel->wheel_area.width + 1,
      thumbwheel->wheel_area.height + 1);

    gdk_draw_rgb_32_image (widget->window,
      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
      thumbwheel->wheel_area.x,
      thumbwheel->wheel_area.y,
      thumbwheel->wheel_area.width,
      thumbwheel->wheel_area.height,
      GDK_RGB_DITHER_NONE, 
      thumbwheel->bitmap,
      thumbwheel->wheel_area.width * 4);
  }
}
