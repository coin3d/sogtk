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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/viewers/SoGtkFullViewerP.h>
#include <Inventor/Gtk/viewers/SoAnyPlaneViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>

#include <Inventor/Gtk/SoGtkCursors.h>
#include <Inventor/Gtk/common/pixmaps/x.xpm>
#include <Inventor/Gtk/common/pixmaps/y.xpm>
#include <Inventor/Gtk/common/pixmaps/z.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>
#include <Inventor/Gtk/common/pixmaps/ortho.xpm>

// *************************************************************************

/*!
  \class SoGtkPlaneViewer Inventor/Gtk/viewers/SoGtkPlaneViewer.h
  \brief The SoGtkPlaneViewer class is for examining 3D models by moving the camera in orthogonal planes.
  \ingroup components viewers

  FIXME: write doc
*/

// *************************************************************************

class SoGtkPlaneViewerP {
public:
  SoGtkPlaneViewerP( SoGtkPlaneViewer * publ );
  ~SoGtkPlaneViewerP(void);

  SbPlaneProjector * projector;
  static void visibilityCB( void * data, SbBool visible );

  static SoGtkViewerButton SoGtkPlaneViewerButtons[];
  GdkPixmap * orthopixmap, * perspectivepixmap;
  GdkBitmap * orthomask, * perspectivemask;

  GdkCursor * pancursor, * rotatecursor;
  GdkCursor * dollycursor;
  GdkCursor * arrowcursor, * crosscursor;

  GtkWidget * cameratogglebutton;
  static void xbuttonCB( GtkWidget *, gpointer );
  static void ybuttonCB( GtkWidget *, gpointer );
  static void zbuttonCB( GtkWidget *, gpointer );
  static void camerabuttonCB( GtkWidget *, gpointer );

private:
  SoGtkPlaneViewer * pub;
};

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

#define THIS (PRIVATE(this))

// *************************************************************************

enum {
  X_BUTTON = 0,
  Y_BUTTON = 1,
  Z_BUTTON = 2,
  CAMERA_BUTTON = 3,

  FIRST_BUTTON = 0,
  LAST_BUTTON = CAMERA_BUTTON
};

struct SoGtkViewerButton
SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[] = {
  { // plane X button
    N_("x"), "X",
    (GtkSignalFunc) SoGtkPlaneViewerP::xbuttonCB,
    x_xpm
  },
  { // plane Y button
    N_("y"), "Y",
    (GtkSignalFunc) SoGtkPlaneViewerP::ybuttonCB,
    y_xpm
  },
  { // plane Z button
    N_("z"), "Z",
    (GtkSignalFunc) SoGtkPlaneViewerP::zbuttonCB,
    z_xpm
  },
  { // camera type button
    N_("camera"), "C",
    (GtkSignalFunc) SoGtkPlaneViewerP::camerabuttonCB,
    perspective_xpm
  }
}; // SoGtkPlaneViewerButtons[]

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkPlaneViewer);

// *************************************************************************

/*!
  Public default constructor.
*/

SoGtkPlaneViewer::SoGtkPlaneViewer( // public
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const SoGtkFullViewer::BuildFlag flag,
  const SoGtkViewer::Type type )
: inherited( parent, name, embed, flag, type, FALSE )
{
  this->constructor( TRUE );
} // SoGtkPlaneViewer()

/*!
  Protected destructor used when deriving the SoGtkPlaneViewer component class.
*/

SoGtkPlaneViewer::SoGtkPlaneViewer( // protected
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const SoGtkFullViewer::BuildFlag flag,
  const SoGtkViewer::Type type,
  const SbBool build )
: inherited( parent, name, embed, flag, type, FALSE )
{
  this->constructor( build );
} // SoGtkPlaneViewer()

/*!
  Common code for all constructors.
*/

void
SoGtkPlaneViewer::constructor( // private
  const SbBool build )
{
  this->pimpl = new SoGtkPlaneViewerP(this);
  this->common = new SoAnyPlaneViewer(this);

  this->addVisibilityChangeCallback( SoGtkPlaneViewerP::visibilityCB, this );

  this->setClassName( "SoGtkPlaneViewer" );

  this->setPopupMenuString( _( "Plane Viewer" ) );
  this->setPrefSheetString( _( "Plane Viewer Preference Sheet" ) ); 

  this->setLeftWheelString( _( "Translate Y" ) );
  this->setBottomWheelString( _( "Translate X" ) );
  this->setRightWheelString( _( "Zoom" ) );

  if ( ! build ) return;
  GtkWidget * viewer = this->buildWidget( this->getParentWidget() );
  this->setBaseWidget( viewer );
//  this->setSize( SbVec2s( 500, 460 ) );
} // constructor()

/*!
  The destructor.
*/

SoGtkPlaneViewer::~SoGtkPlaneViewer(
  void )
{
  delete this->common;
  delete this->pimpl;
} // ~SoGtkPlaneViewer()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
} // setViewing()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setCamera( // virtual
  SoCamera * newCamera )
{
  if ( newCamera ) {
    const SoType camtype = newCamera->getTypeId() ;
    const SbBool orthotype =
      camtype.isDerivedFrom( SoOrthographicCamera::getClassTypeId() );

    this->setRightWheelString( orthotype ? _( "Zoom" ) : _( "Dolly" ));
    if ( PRIVATE(this)->cameratogglebutton ) {
      GtkWidget * label = GTK_BIN(PRIVATE(this)->cameratogglebutton)->child;
      gtk_pixmap_set( GTK_PIXMAP(label),
        (orthotype ? PRIVATE(this)->orthopixmap : PRIVATE(this)->perspectivepixmap),
        (orthotype ? PRIVATE(this)->orthomask : PRIVATE(this)->perspectivemask) );
    }
  }
  inherited::setCamera( newCamera );
} // setCamera()

/*!
  Decide whether or not the mouse pointer cursor should be visible in the
  rendering canvas.
*/

void
SoGtkPlaneViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
  this->setCursorRepresentation( this->common->mode );
} // setCursorEnabled()

// *************************************************************************

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkPlaneViewer::buildWidget( // protected
  GtkWidget * parent )
{
  GtkWidget * viewer = inherited::buildWidget( parent );
  gtk_thumbwheel_set_range_boundary_handling(
     GTK_THUMBWHEEL(this->leftWheel), GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE );
  gtk_thumbwheel_set_range_boundary_handling(
     GTK_THUMBWHEEL(this->rightWheel), GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE );
  
  return viewer;
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkPlaneViewer::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoGtkPlaneViewer";
  return defaultWidgetName;
}

/*!
  FIXME: write doc
*/

const char *
SoGtkPlaneViewer::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = N_( "Plane Viewer" );
  return _( defaultTitle );
}

/*!
  FIXME: write doc
*/

const char *
SoGtkPlaneViewer::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = N_( "Plane Viewer" );
  return _( defaultIconTitle );
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  common->translateX( value - this->getBottomWheelValue() );
  inherited::bottomWheelMotion( value );
} // bottomWheelMotion()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::leftWheelMotion( // virtual, protected
  float value )
{
  common->translateY( value - this->getLeftWheelValue() );
  inherited::leftWheelMotion( value );
} // leftWheelMotion()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::rightWheelMotion( // virtual, protected
  float value )
{
  common->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

// *************************************************************************

/*!
  FIXME: write doc
*/

SbBool
SoGtkPlaneViewer::processSoEvent( // virtual, protected
  const SoEvent * const event )
{
  if ( common->processSoEvent( event ) )
    return TRUE;

  return inherited::processSoEvent( event );
} // processSoEvent()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::processEvent( // virtual, protected
  GdkEvent * event )
{
  // SoDebugError::postInfo( "SoGtkPlaneViewer::processEvent", "[invoked]" );
  if ( SoGtkViewer::processCommonEvents( event ) )
    return;

  inherited::processEvent( event );
} // processEvent()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setSeekMode( // virtual, protected
  SbBool enable )
{
  inherited::setSeekMode( enable );
#if 0
  this->common->setMode( on ?
    SoAnyPlaneViewer::SEEK_WAIT_MODE :
    SoAnyPlaneViewer::EXAMINE );
#endif
} // setSeekMode()

/*!
  Overload this method to be able to draw the rotation graphics in rotation
  mode.
*/

void
SoGtkPlaneViewer::actualRedraw( // virtual, protected
  void )
{
  inherited::actualRedraw();
} // actualRedraw()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::createViewerButtons( // virtual, protected
  GtkWidget * parent,
  SbPList * buttonlist )
{
  inherited::createViewerButtons( parent, buttonlist );

  GtkTooltips * tooltips = gtk_tooltips_new();
  GdkColormap * colormap = gtk_widget_get_colormap( parent );

  const size_t buttons = sizeof(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons)
                         / sizeof(SoGtkViewerButton);
  for ( size_t button = 0; button < buttons; button++ ) {
    GtkWidget * widget = (GtkWidget*) 0 ;

    switch(button)
    {
    case CAMERA_BUTTON :
      PRIVATE(this)->cameratogglebutton = widget = gtk_button_new();
      break ;
    default: 
      widget = gtk_button_new();
      break ;
    }

    GTK_WIDGET_UNSET_FLAGS(widget,GTK_CAN_FOCUS);
    gtk_tooltips_set_tip( tooltips, widget, 
      _( SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].keyword ), NULL );

    GdkPixmap * gdk_pixmap = (GdkPixmap *) 0;
    GdkBitmap * gdk_mask   = (GdkBitmap *) 0;

    switch(button)
    {
    case CAMERA_BUTTON :
      {
        SoType t = this->getCameraType();
        if ( t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()) )
        {
          gdk_pixmap = PRIVATE(this)->orthopixmap;
          gdk_mask   = PRIVATE(this)->orthomask;
        }
        else if ( t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) )
        {
          gdk_pixmap = PRIVATE(this)->perspectivepixmap;
          gdk_mask   = PRIVATE(this)->perspectivemask;
        }
        else assert(0 && "unsupported cameratype");
      }
      break;
    default:
      gdk_pixmap =
        gdk_pixmap_colormap_create_from_xpm_d( (GdkWindow *) 0, colormap,
          &gdk_mask, (GdkColor *) 0,
          SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].xpm_data );
      break;
    }


    GtkWidget * label = gtk_pixmap_new( gdk_pixmap, gdk_mask );
    gtk_widget_show( label );

    switch( button )
    {
    case CAMERA_BUTTON:
      break ;
    default :
      gdk_pixmap_unref( gdk_pixmap );
      gdk_bitmap_unref( gdk_mask );
      break ;
    };

    gtk_container_add( GTK_CONTAINER(widget), GTK_WIDGET(label) );
    if ( SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].pressed != NULL ) {
      gtk_signal_connect( GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].pressed),
        (gpointer) this );
    }
    buttonlist->append( widget );
  }
} // createViewerButtons()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoGtkPlaneViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

/*!
  \internal

  Set cursor graphics according to mode.
*/

void
SoGtkPlaneViewer::setCursorRepresentation(
  int mode )
{
  GtkWidget * w = this->getGLWidget();
  assert(w);

  if ( ! PRIVATE(this)->arrowcursor ) {
    if ( GTK_WIDGET_NO_WINDOW(w) )
    {
#if SOGTK_DEBUG
  // FIXME: This should not happen, but there seems to be a bug in 
  // SoGtk's event handling causing this. 20001219 RC.
       SoDebugError::postWarning( 
         "SoGtkPlaneViewer::setCursorRepresentation",
          "widget %x: NO WINDOW\n", (int) w );
#endif
      return ;
    }

    GdkWindow *window = w->window ;
    if ( window == (GdkWindow*) 0 ) 
    {
#if SOGTK_DEBUG
  // FIXME: This should not happen, but there seems to be a bug in 
  // SoGtk's event handling causing this. 20001219 RC.
       SoDebugError::postWarning( 
         "SoGtkPlaneViewer::setCursorRepresentation",
          "widget %x: widget->window == 0\n", (int) w );
#endif
      return ;
    }

    GtkStyle *style = w->style ;
    GdkColor fg = style->black ;
    GdkColor bg = style->white ;

    PRIVATE(this)->arrowcursor = gdk_cursor_new( GDK_TOP_LEFT_ARROW );
    PRIVATE(this)->crosscursor = gdk_cursor_new( GDK_CROSSHAIR );
    PRIVATE(this)->dollycursor = gdk_cursor_new( GDK_SB_UP_ARROW );

    GdkPixmap *panBtm = 
      gdk_bitmap_create_from_data(NULL, (const gchar *) so_gtk_pan_bitmap,
        so_gtk_pan_width, so_gtk_pan_height);
    GdkPixmap *panMask =
      gdk_bitmap_create_from_data(NULL, (const gchar *) so_gtk_pan_mask_bitmap,
        so_gtk_pan_width, so_gtk_pan_height);
    PRIVATE(this)->pancursor = gdk_cursor_new_from_pixmap (
      panBtm, panMask, 
      &fg, &bg, 
      so_gtk_pan_x_hot, so_gtk_pan_y_hot);
    gdk_pixmap_unref (panBtm);
    gdk_pixmap_unref (panMask);

    GdkPixmap *rotateBtm = 
      gdk_bitmap_create_from_data(NULL, (const gchar *) so_gtk_rotate_bitmap,
        so_gtk_rotate_width, so_gtk_rotate_height);
    GdkPixmap *rotateMask =
      gdk_bitmap_create_from_data(NULL, (const gchar *) so_gtk_rotate_mask_bitmap,
        so_gtk_rotate_width, so_gtk_rotate_height);
    PRIVATE(this)->rotatecursor = gdk_cursor_new_from_pixmap (
      rotateBtm, rotateMask, 
      &fg, &bg, 
      so_gtk_rotate_x_hot, so_gtk_rotate_y_hot);
    gdk_pixmap_unref (rotateBtm);
    gdk_pixmap_unref (rotateMask);
  }

  if ( ! this->isCursorEnabled() ) {
    gdk_window_set_cursor( w->window, (GdkCursor*) 0 );
    return;
  }

  switch ( (SoAnyPlaneViewer::PlaneViewerMode) mode) {
//  case SoAnyPlaneViewer::INTERACT:
//  case SoAnyPlaneViewer::EXAMINE:
  case SoAnyPlaneViewer::IDLE_MODE:
    gdk_window_set_cursor( w->window, 0 );
    break ;
  case SoAnyPlaneViewer::DOLLY_MODE:
    gdk_window_set_cursor( w->window, PRIVATE(this)->dollycursor);
    break;
  case SoAnyPlaneViewer::ROTZ_WAIT_MODE:
  case SoAnyPlaneViewer::ROTZ_MODE:
    gdk_window_set_cursor( w->window, PRIVATE(this)->rotatecursor);
    break;
  case SoAnyPlaneViewer::SEEK_WAIT_MODE:
  case SoAnyPlaneViewer::SEEK_MODE:
    gdk_window_set_cursor( w->window, PRIVATE(this)->crosscursor );
    break;
  case SoAnyPlaneViewer::TRANSLATE_MODE:
    gdk_window_set_cursor( w->window, PRIVATE(this)->pancursor);
    break;
  default: 
    assert(0); 
    break;
  }
} // setCursorRepresentation()

// ************************************************************************
//
//  Private implementation
//

/*
*/

SoGtkPlaneViewerP::SoGtkPlaneViewerP(
  SoGtkPlaneViewer * publ )
{
  this->pub = publ;

  this->projector = new SbPlaneProjector;
  SbViewVolume vv;
  vv.ortho(-1, 1, -1, 1, -1, 1);
  this->projector->setViewVolume(vv);

  // Cursors.
  this->rotatecursor = (GdkCursor *) 0;
  this->pancursor = (GdkCursor *) 0;
  this->dollycursor = (GdkCursor *) 0;
  this->crosscursor = (GdkCursor *) 0;
  this->arrowcursor = (GdkCursor *) 0;

  GdkColormap * colormap = gtk_widget_get_colormap (PUBLIC(this)->getParentWidget());

  this->orthopixmap =
    gdk_pixmap_colormap_create_from_xpm_d( (GdkWindow *) 0, colormap,
      &this->orthomask, (GdkColor *) 0, ortho_xpm );

  this->perspectivepixmap =
    gdk_pixmap_colormap_create_from_xpm_d( (GdkWindow *) 0, colormap,
      &this->perspectivemask, (GdkColor *) 0, perspective_xpm );
} // SoGtkPlaneViewerP()

/*
*/

SoGtkPlaneViewerP::~SoGtkPlaneViewerP(void)
{
  // Cursors.
  if ( this->dollycursor ) 
    gdk_cursor_destroy( this->dollycursor );
  if ( this->pancursor )
    gdk_cursor_destroy( this->pancursor );
  if ( this->rotatecursor ) 
    gdk_cursor_destroy( this->rotatecursor );
  if ( this->arrowcursor ) 
    gdk_cursor_destroy( this->arrowcursor );
  if ( this->crosscursor ) 
    gdk_cursor_destroy( this->crosscursor );

  // Button pixmaps.
  gdk_pixmap_unref( this->orthopixmap );
  gdk_bitmap_unref( this->orthomask );  
  gdk_pixmap_unref( this->perspectivepixmap );
  gdk_bitmap_unref( this->perspectivemask );  

  delete this->projector;
} // ~SoGtkPlaneViewerP()

// ************************************************************************

/*!
  \internal
*/

void
SoGtkPlaneViewerP::visibilityCB( // static
  void * closure,
  SbBool visible )
{
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  /* no animation to toggle */
} // visibilityCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::xbuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->common->viewPlaneX();
} // ybuttonCB()

/*
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::ybuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->common->viewPlaneY();
} // ybuttonCB()

/*
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::zbuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->common->viewPlaneZ();
} // zbuttonCB()

/*
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::camerabuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  if ( viewer->getCamera() )
    viewer->toggleCameraType();
} // camerabuttonCB()

// ************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkPlaneViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

