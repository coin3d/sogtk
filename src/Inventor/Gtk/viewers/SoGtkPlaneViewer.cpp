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
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/viewers/SoGtkFullViewerP.h>
#include <Inventor/Gtk/viewers/SoAnyPlaneViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>

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

SOGTK_OBJECT_SOURCE(SoGtkPlaneViewer);

// *************************************************************************

struct SoGtkViewerButton
SoGtkPlaneViewer::SoGtkPlaneViewerButtons[] = {
  { // plane X button
    N_("x"), "X",
    (GtkSignalFunc) SoGtkPlaneViewer::xbuttonCB,
    x_xpm
  },
  { // plane Y button
    N_("y"), "Y",
    (GtkSignalFunc) SoGtkPlaneViewer::ybuttonCB,
    y_xpm
  },
  { // plane Z button
    N_("z"), "Z",
    (GtkSignalFunc) SoGtkPlaneViewer::zbuttonCB,
    z_xpm
  },
  { // camera type button
    N_("camera"), "C",
    (GtkSignalFunc) SoGtkPlaneViewer::camerabuttonCB,
    perspective_xpm
  }
}; // SoGtkPlaneViewerButtons[]

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
, common( new SoAnyPlaneViewer(this) )
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
, common( new SoAnyPlaneViewer(this) )
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
  const int buttons = sizeof(SoGtkPlaneViewerButtons) / sizeof(SoGtkViewerButton);

  this->setLeftWheelString( _( "Translate Y" ) );
  this->setBottomWheelString( _( "Translate X" ) );
  this->setRightWheelString( _( "Zoom" ) );

  this->setClassName( "SoGtkPlaneViewer" );

  if ( ! build ) return;
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * viewerbase = this->buildWidget( parent );
  this->setBaseWidget( viewerbase );
  this->setSize( SbVec2s( 500, 460 ) );
} // constructor()

/*!
  The destructor.
*/

SoGtkPlaneViewer::~SoGtkPlaneViewer(
  void )
{
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
  SoCamera * camera )
{
  inherited::setCamera( camera );
  // switch camera button pixmap
} // setCamera()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
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
  // this->leftWheel->setRangeBoundaryHandling( SoGtkThumbWheel::ACCUMULATE );
  // this->bottomWheel->setRangeBoundaryHandling( SoGtkThumbWheel::ACCUMULATE );
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
  this->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

/*!
  This method moves the camera inwards and outwards.  It will be moved to
  common code soon.
*/

void
SoGtkPlaneViewer::zoom(
  const float difference )
{
  SoCamera * camera = this->getCamera();
  assert( camera != NULL );

  SoType type = camera->getTypeId();
  float multiplicator = exp( difference ); // in the range of <0, ->>

  if ( type.isDerivedFrom( SoOrthographicCamera::getClassTypeId() ) ) {
    SoOrthographicCamera * orthocam = (SoOrthographicCamera *) camera;
    orthocam->height = orthocam->height.getValue() * multiplicator;
  } else if ( type.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) ) {
    float oldfocaldist = camera->focalDistance.getValue();
    camera->focalDistance = oldfocaldist * multiplicator;
    SbVec3f direction;
    camera->orientation.getValue().multVec( SbVec3f( 0, 0, -1 ), direction );
    camera->position = camera->position.getValue() +
      (camera->focalDistance.getValue() - oldfocaldist) * -direction;
  } else {
    assert( 0 && "impossible" );
  }
} // zoom()

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
} // setSeekMode()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::actualRedraw( // virtual, protected
  void )
{
  inherited::actualRedraw();
} // actualRedraw()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::createPrefSheet( // virtual, protected
  void )
{
//  inherited::createPrefSheet();
} // createPrefSheet()

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
  GdkBitmap * mask;

  const size_t buttons = sizeof(SoGtkPlaneViewerButtons) / sizeof(SoGtkViewerButton);
  for ( size_t button = 0; button < buttons; button++ ) {
    GtkWidget * widget = GTK_WIDGET(gtk_button_new());
    gtk_tooltips_set_tip( tooltips, widget, 
      _( SoGtkPlaneViewerButtons[button].keyword ), NULL );

    GdkPixmap * gdk_pixmap =
      gdk_pixmap_colormap_create_from_xpm_d( NULL, colormap,
        &mask, NULL,
        SoGtkPlaneViewerButtons[button].xpm_data );

    GtkWidget * label = gtk_pixmap_new( gdk_pixmap, mask );
    gtk_widget_show( label );

    gdk_pixmap_unref( gdk_pixmap );
    gdk_bitmap_unref( mask );

    gtk_container_add( GTK_CONTAINER(widget), GTK_WIDGET(label) );
    if ( SoGtkPlaneViewerButtons[button].pressed != NULL ) {
      gtk_signal_connect( GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(SoGtkPlaneViewerButtons[button].pressed),
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
  FIXME: write doc
*/

void
SoGtkPlaneViewer::xbuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->common->viewPlaneX();
} // ybuttonCB()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::ybuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->common->viewPlaneY();
} // ybuttonCB()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::zbuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->common->viewPlaneZ();
} // zbuttonCB()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::camerabuttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->toggleCameraType();
} // camerabuttonCB()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkPlaneViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

