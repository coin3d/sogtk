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

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <Inventor/Gtk/viewers/SoGtkFullViewerP.h>
#include <Inventor/Gtk/viewers/SoAnyPlaneViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>

#include <Inventor/Gtk/common/pixmaps/x.xpm>
#include <Inventor/Gtk/common/pixmaps/y.xpm>
#include <Inventor/Gtk/common/pixmaps/z.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>
#include <Inventor/Gtk/common/pixmaps/ortho.xpm>

/*!
  \class SoGtkPlaneViewer Inventor/Gtk/viewers/SoGtkPlaneViewer.h
  \brief The SoGtkPlaneViewer class is for examining 3D models by
  moving the camera in planes.
*/

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkPlaneViewer);

// *************************************************************************

struct SoGtkViewerButton
SoGtkPlaneViewer::SoGtkPlaneViewerButtons[] = {
  { // plane X button
    "x", "X",
    (GtkSignalFunc) SoGtkPlaneViewer::buttonCB,
    x_xpm,
    NULL, NULL
  },
  { // plane Y button
    "y", "Y",
    (GtkSignalFunc) SoGtkPlaneViewer::buttonCB,
    y_xpm,
    NULL, NULL
  },
  { // plane Z button
    "z", "Z",
    (GtkSignalFunc) SoGtkPlaneViewer::buttonCB,
    z_xpm,
    NULL, NULL
  },
  { // camera type button
    "camera", "C",
    (GtkSignalFunc) SoGtkPlaneViewer::buttonCB,
    perspective_xpm,
    NULL, NULL
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
  this->buttons = new SoGtkViewerButton [ buttons ];
  memcpy( this->buttons, SoGtkPlaneViewerButtons, sizeof(SoGtkPlaneViewerButtons) );

  this->setLeftWheelString( "Translate Y" );
  this->setBottomWheelString( "Translate X" );
  this->setRightWheelString( "Zoom" );

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
  delete [] this->buttons;
} // ~SoGtkPlaneViewer()

// *************************************************************************

/*!
*/

void
SoGtkPlaneViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
} // setViewing()

/*!
*/

void
SoGtkPlaneViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );
  // switch camera button pixmap
} // setCamera()

/*!
*/

void
SoGtkPlaneViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
} // setCursorEnabled()

// *************************************************************************

/*!
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
*/

const char *
SoGtkPlaneViewer::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoGtkPlaneViewer";
  return defaultWidgetName;
}

/*!
*/

const char *
SoGtkPlaneViewer::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Plane Viewer";
  return defaultTitle;
}

/*!
*/

const char *
SoGtkPlaneViewer::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Plane Viewer";
  return defaultIconTitle;
}

// *************************************************************************

/*!
*/

void
SoGtkPlaneViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  common->translateX( value - this->getBottomWheelValue() );
  inherited::bottomWheelMotion( value );
} // bottomWheelMotion()

/*!
*/

void
SoGtkPlaneViewer::leftWheelMotion( // virtual, protected
  float value )
{
  common->translateY( value - this->getLeftWheelValue() );
  inherited::leftWheelMotion( value );
} // leftWheelMotion()

/*!
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
*/

void
SoGtkPlaneViewer::setSeekMode( // virtual, protected
  SbBool enable )
{
  inherited::setSeekMode( enable );
} // setSeekMode()

/*!
*/

void
SoGtkPlaneViewer::actualRedraw( // virtual, protected
  void )
{
  inherited::actualRedraw();
} // actualRedraw()

void
SoGtkPlaneViewer::createPrefSheet( // virtual, protected
  void )
{
//  inherited::createPrefSheet();
} // createPrefSheet()

// *************************************************************************

/*!
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

  const int buttons = sizeof(SoGtkPlaneViewerButtons) / sizeof(SoGtkViewerButton);
  for ( int button = 0; button < buttons; button++ ) {
    GtkWidget * widget = GTK_WIDGET(gtk_button_new());
    gtk_tooltips_set_tip( tooltips, widget, this->buttons[button].keyword, NULL );

    GdkPixmap * gdk_pixmap =
      gdk_pixmap_colormap_create_from_xpm_d( NULL, colormap,
        &mask, NULL,
        this->buttons[button].xpm_data );

    GtkWidget * label = gtk_pixmap_new( gdk_pixmap, mask );
    gdk_pixmap_unref( gdk_pixmap );
    gdk_bitmap_unref( mask );

    gtk_widget_show( label );
    this->buttons[button].lwidget = label;

    gtk_container_add( GTK_CONTAINER(widget), GTK_WIDGET(label) );
    if ( this->buttons[button].pressed != NULL ) {
      gtk_signal_connect( GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(this->buttons[button].pressed),
        (gpointer) this );
    }
    this->buttons[button].bwidget = widget;
    buttonlist->append( widget );
  }
} // createViewerButtons()

// *************************************************************************

/*!
*/

void
SoGtkPlaneViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoGtkPlaneViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

/*!
*/

int
SoGtkPlaneViewer::findButton(
  GtkWidget * widget ) const
{
  const int buttons = sizeof(SoGtkPlaneViewerButtons) / sizeof(SoGtkViewerButton);
  for ( int button = 0; button < buttons; button++ ) {
    if ( this->buttons[button].bwidget == widget ) return button;
  }
  return -1;
} // findButton()

/*!
*/

void
SoGtkPlaneViewer::buttonCB(
   GtkWidget * button,
   gpointer closure )
{
  assert( closure != NULL );
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  const int idx = viewer->findButton( button );
  if ( idx == -1 ) {
    SoDebugError::postInfo( "SoGtkPlaneViewer::buttonCB", "unknown button" );
  } else if ( strcmp( viewer->buttons[idx].keyword, "x" ) == 0 ) {
    viewer->common->viewPlaneX();
  } else if ( strcmp( viewer->buttons[idx].keyword, "y" ) == 0 ) {
    viewer->common->viewPlaneY();
  } else if ( strcmp( viewer->buttons[idx].keyword, "z" ) == 0 ) {
    viewer->common->viewPlaneZ();
  } else if ( strcmp( viewer->buttons[idx].keyword, "camera" ) == 0 ) {
    viewer->toggleCameraType();
  } else {
    SoDebugError::postInfo( "SoGtkPlaneViewer::buttonCB", "unsupported button" );
  }
} // buttonCB()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkPlaneViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

