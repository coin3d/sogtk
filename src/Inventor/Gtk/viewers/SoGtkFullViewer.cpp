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

// this stops IRIX build from crashing
// #define NO_THUMBWHEELS

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtktogglebutton.h>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>
#include <Inventor/Gtk/widgets/SoAnyPopupMenu.h>

#include <Inventor/Gtk/viewers/SoAnyFullViewer.h>
#include <Inventor/Gtk/viewers/SoGtkFullViewerP.h>
#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>

// Button icons.
#include <Inventor/Gtk/common/pixmaps/pick.xpm>
#include <Inventor/Gtk/common/pixmaps/view.xpm>
#include <Inventor/Gtk/common/pixmaps/help.xpm>
#include <Inventor/Gtk/common/pixmaps/home.xpm>
#include <Inventor/Gtk/common/pixmaps/set_home.xpm>
#include <Inventor/Gtk/common/pixmaps/view_all.xpm>
#include <Inventor/Gtk/common/pixmaps/seek.xpm>

/*!
  \class SoGtkFullViewer SoGtkFullViewer.h Inventor/Qt/viewers/SoGtkFullViewer.h
  \brief The SoGtkFullViewer class adds decorations to the simple viewer of the parent.
  \ingroup viewers

  TODO: more doc
  ...overview of what this class provides over parent class...
  ...keyboard combinations...mousebutton combinations...explain pan,
  rotate, zoom, dolly, seek...explain the fact that we're moving the
  camera, not the scene...
  ...viewer modes (examine vs interact) w/ explanation of what they are
  useful for...

  \sa SoGtkConstrainedViewer, SoGtkExaminerViewer, SoGtkPlaneViewer
*/

// *************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkFullViewer);

// *************************************************************************

/*¡
  SoGtkFullViewer is missing the class description documentation.

  TODO, low priority:
  <UL>
  <LI>add tooltips to the widgets?</LI>
  <LI>trim length of edit fields</LI>
  </UL>
*/

/*!
  \enum SoGtkFullViewer::BuildFlag
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkFullViewer::BuildFlag SoGtkFullViewer::BUILD_NONE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkFullViewer::BuildFlag SoGtkFullViewer::BUILD_DECORATION
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkFullViewer::BuildFlag SoGtkFullViewer::BUILD_POPUP
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkFullViewer::BuildFlag SoGtkFullViewer::BUILD_ALL
  FIXME: write documentation for enum definition
*/

/*!
  \var GtkWidget * SoGtkFullViewer::leftDecoration
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::leftWheel
  FIXME: write documentation
*/

/*!
  \var char * SoGtkFullViewer::leftWheelStr
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::leftWheelLabel
  FIXME: write documentation
*/

/*!
  \var float SoGtkFullViewer::leftWheelVal
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::bottomDecoration
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::bottomWheel
  FIXME: write documentation
*/

/*!
  \var char * SoGtkFullViewer::bottomWheelStr
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::bottomWheelLabel
  FIXME: write documentation
*/

/*!
  \var float SoGtkFullViewer::bottomWheelVal
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::rightDecoration
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::rightWheel
  FIXME: write documentation
*/

/*!
  \var char * SoGtkFullViewer::rightWheelStr
  FIXME: write documentation
*/

/*!
  \var GtkWidget * SoGtkFullViewer::rightWheelLabel
  FIXME: write documentation
*/

/*!
  \var float SoGtkFullViewer::rightWheelVal
  FIXME: write documentation
*/

/*!
  \var SoAnyPopupMenu * SoGtkFullViewer::prefmenu
  FIXME: write documentation
*/

// *************************************************************************

static const int VIEWERBORDER = 2;
static const int ZOOMSLIDERRESOLUTION = 200;

///////// FIXME start //////////////////////////////////////////////////
// Do something clever about this Qt layout assistant code..
// 990222 mortene.

enum LayoutOrientation { LayoutVertical, LayoutHorizontal };

// *************************************************************************

/*
void
expandSize(QSize & result, const QSize & addend, LayoutOrientation o)
{
  if (o == LayoutHorizontal) {
    result.rwidth() += addend.width();
    if (result.height() < addend.height()) result.setHeight(addend.height());
  }
  else {
    result.rheight() += addend.height();
    if (result.width() < addend.width()) result.setWidth(addend.width());
  }
}
*/

///////// FIXME end ////////////////////////////////////////////////////


// FIXME: rewrite as inline function? 990221 mortene.
#define VIEWERBUTTON(idb) \
  ((GtkWidget *)((*(this->viewerButtons))[idx]))

// Button index values.
enum {
  INTERACT_BUTTON = 0,
  EXAMINE_BUTTON,
  HELP_BUTTON,
  HOME_BUTTON,
  SET_HOME_BUTTON,
  VIEW_ALL_BUTTON,
  SEEK_BUTTON,

  FIRST_BUTTON = 0,
  LAST_BUTTON = SEEK_BUTTON
};

// *************************************************************************

struct SoGtkViewerButton
SoGtkFullViewer::SoGtkFullViewerButtons[] = {
  { // interact button
    N_( "interact" ), "I",
    (GtkSignalFunc) SoGtkFullViewer::interactbuttonClickedCB,
    pick_xpm
  },
  { // view
    N_( "view" ), "E",
    (GtkSignalFunc) SoGtkFullViewer::viewbuttonClickedCB,
    view_xpm
  },
  { // help
    N_( "help" ), "?",
    (GtkSignalFunc) SoGtkFullViewer::helpbuttonClickedCB,
    help_xpm
  },
  { // home
    N_( "home" ), "h",
    (GtkSignalFunc) SoGtkFullViewer::homebuttonClickedCB,
    home_xpm
  },
  { // set home
    N_( "set_home" ), "H",
    (GtkSignalFunc) SoGtkFullViewer::sethomebuttonClickedCB,
    set_home_xpm
  },
  { // view all
    N_( "view_all" ), "V",
    (GtkSignalFunc) SoGtkFullViewer::viewallbuttonClickedCB,
    view_all_xpm
  },
  { // seek
    N_( "seek" ), "S",
    (GtkSignalFunc) SoGtkFullViewer::seekbuttonClickedCB,
    seek_xpm
  }
}; // SoGtkFullViewerButtons[]

// *************************************************************************

/*!
  Constructor. See parent class for explanation of arguments.

  Subclasses will probably usually call with the \c build flag
  set to \c FALSE to be able to do delayed building of the OpenGL
  canvas after other setup tasks has been performed.
*/

SoGtkFullViewer::SoGtkFullViewer(
  GtkWidget * parent,
  const char * name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag buildFlag,
  SoGtkViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, type, FALSE )
, common( new SoAnyFullViewer( this ) )
{
  const int buttons = sizeof(SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);

  this->viewerWidget = (GtkWidget *) NULL;
  this->canvas = (GtkWidget *) NULL;
  this->canvasParent = (GtkWidget *) NULL;

  this->leftWheel = (GtkWidget *) NULL;
  this->leftWheelLabel = (GtkWidget *) NULL;
  this->leftWheelStr = (char *) NULL;
  this->leftWheelVal = 0.0f;
  this->bottomWheel = (GtkWidget *) NULL;
  this->bottomWheelLabel = (GtkWidget *) NULL;
  this->bottomWheelStr = (char *) NULL;
  this->bottomWheelVal = 0.0f;
  this->rightWheel = (GtkWidget *) NULL;
  this->rightWheelLabel = (GtkWidget *) NULL;
  this->rightWheelStr = (char *) NULL;
  this->rightWheelVal = 0.0f;

  this->setLeftWheelString( _( "Motion Y" ) );
  this->setBottomWheelString( _( "Motion X" ) );
  this->setRightWheelString( _( "Motion Z" ) );

  this->zoomrange = SbVec2f( 1.0f, 140.0f );

  this->mainLayout = (GtkWidget *) NULL;
  this->appButtonLayout = (GtkWidget *) NULL;

  this->prefmenu = (SoAnyPopupMenu *) NULL;
  this->prefwindow = (GtkWidget *) NULL;
  this->prefwindowtitle = _( "Viewer Preference Sheet" );

  this->menuEnabled = buildFlag & SoGtkFullViewer::BUILD_POPUP;
  this->decorations = buildFlag & SoGtkFullViewer::BUILD_DECORATION;

  this->menuTitle = _( "Viewer Menu" );

  this->viewerButtons = new SbPList;
  this->appButtonList = new SbPList;
  this->appButtonForm = (GtkWidget *) NULL;

  this->setClassName( "SoGtkFullViewer" );

  if ( ! build ) return;
  GtkWidget * viewerparent = this->getParentWidget();
  GtkWidget * viewer = this->buildWidget( viewerparent );
  this->setBaseWidget( viewer );
  // this->setSize( SbVec2s( 500, 390 ) );
} // SoGtkFullViewer()

// *************************************************************************

/*!
  Destructor.
*/

SoGtkFullViewer::~SoGtkFullViewer(
  void )
{
  delete this->viewerButtons;
  delete this->appButtonList;

  delete [] this->leftWheelStr;
  delete [] this->bottomWheelStr;
  delete [] this->rightWheelStr;
} // ~SoGtkFullViewer()

// *************************************************************************

/*!
  Turn the viewer decorations on or off.

  \sa isDecoration()
*/

void
SoGtkFullViewer::setDecoration(
  const SbBool enable )
{
#if SOGTK_DEBUG
  if ((enable && this->isDecoration()) || (!enable && !this->isDecoration())) {
    SoDebugError::postWarning("SoGtkFullViewer::setDecoration",
                              "decorations already turned %s",
                              enable ? "on" : "off");
    return;
  }
#endif // SOGTK_DEBUG

  this->decorations = enable;
  if ( this->viewerWidget )
    this->showDecorationWidgets(enable);
}

// *************************************************************************

/*!
  Return \c TRUE if the viewer decorations are on, otherwise \c FALSE.

  \sa setDecoration()
*/

SbBool
SoGtkFullViewer::isDecoration(
  void ) const
{
  return this->decorations;
}

// *************************************************************************

/*!
  Decide whether or not if clicking with the right mouse button on the
  OpenGL canvas should reveal a preferences popup menu when in viewing
  mode.

  \sa isPopupMenuEnabled()
*/

void
SoGtkFullViewer::setPopupMenuEnabled(
  const SbBool on )
{
#if SOGTK_DEBUG
  if ((on && this->isPopupMenuEnabled()) ||
      (!on && !this->isPopupMenuEnabled())) {
    SoDebugError::postWarning("SoGtkFullViewer::setPopupMenuEnabled",
                              "popup menu already turned %s",
                              on ? "on" : "off");
    return;
  }
#endif // SOGTK_DEBUG
  this->menuEnabled = on;
}

// *************************************************************************

/*!
  Return \c TRUE if the popup preferences menu is enabled,
  otherwise \c FALSE.

  \sa setPopupMenuEnabled()
*/

SbBool
SoGtkFullViewer::isPopupMenuEnabled(
  void ) const
{
  return this->menuEnabled;
}

// *************************************************************************

/*!
  Returns the widget which is used as the parent of application
  specified buttons. The row of application buttons (if any) will be
  placed in the upper left corner.

  \sa addAppPushButton(), insertAppPushButton(), removeAppPushButton()
*/

GtkWidget *
SoGtkFullViewer::getAppPushButtonParent(
  void ) const
{
  return this->appButtonForm;
} // getAppPushButtonParent()

// *************************************************************************

/*!
  Add an application specific push button to the viewer decorations.
  Application buttons will be laid out in a vertical row from the
  upper left corner.

  The button will be added bottom-most.

  \sa insertAppPushButton(), removeAppPushButton(), getAppPushButtonParent()
*/

void
SoGtkFullViewer::addAppPushButton(
  GtkWidget * button )
{
  this->appButtonList->append( button );
  this->layoutAppButtons( this->getAppPushButtonParent() );
} // addAppPushButton()

// *************************************************************************

/*!
  Insert an application specific push button to the viewer decorations
  at the given \c index.

  \sa addAppPushButton(), removeAppPushButton(), getAppPushButtonParent()
*/

void
SoGtkFullViewer::insertAppPushButton(
  GtkWidget * newButton,
  int index )
{
#if SOGTK_DEBUG
  if ((index < 0) || (index > this->lengthAppPushButton())) {
    SoDebugError::postWarning("SoGtkFullViewer::insertAppPushButton",
                              "index %d out of bounds", index);
    return;
  }
#endif // SOGTK_DEBUG
  this->appButtonList->insert(newButton, index);
  this->layoutAppButtons(this->getAppPushButtonParent());
}

// *************************************************************************

/*!
  Remove one of the application specific buttons.

  \sa addAppPushButton(), insertAppPushButton()
*/

void
SoGtkFullViewer::removeAppPushButton(
  GtkWidget * oldButton )
{
  int idx = this->appButtonList->find(oldButton);

#if SOGTK_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoGtkFullViewer::removeAppPushButton",
                              "tried to remove non-existant button");
    return;
  }
#endif // SOGTK_DEBUG

  this->appButtonList->remove(idx);
  this->layoutAppButtons(this->getAppPushButtonParent());
} // removeAppPushButton()

// *************************************************************************

/*!
  Return the index of a particular button that has been specified by
  the application, or -1 of the button has not been added.

  \sa addAppPushButton()
*/

int
SoGtkFullViewer::findAppPushButton(
  GtkWidget * button ) const
{
  return this->appButtonList->find( button );
} // findAppPushButton()

// *************************************************************************

/*!
  Return number of application specific buttons added.

  \sa addAppPushButton(), insertAddAppPushButton()
*/

int
SoGtkFullViewer::lengthAppPushButton(
  void ) const
{
  return this->appButtonList->getLength();
} // lengthAppPushButton()

// *************************************************************************

/*!
  Returns the render area OpenGL canvas widget.
*/

GtkWidget *
SoGtkFullViewer::getRenderAreaWidget(
  void )
{
  return this->canvas;
} // getRenderAreaWidget()

// *************************************************************************

/*!
  Set a flag to indicate whether we're in viewing mode (where the
  user can drag the model or scene around), or in interaction mode (where
  all window events from mouse, keyboard, etc are sent to the scene graph).

  Overloaded from parent class to correctly set the user interface
  indicators on the current state, namely the upper right push buttons
  indicating interact or view mode, the respective item on the popup menu
  and to grey out the seek mode activation button while in interact mode.
*/

void
SoGtkFullViewer::setViewing(
  SbBool enable )
{
  if ( ( enable && this->isViewing() ) ||
       ( ! enable && ! this->isViewing() ) ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning( "SoGtkFullViewer::setViewing",
                               "view mode already %sset", enable ? "" : "un");
#endif // SOGTK_DEBUG
    return;
  }

  inherited::setViewing( enable );

  gtk_widget_set_sensitive(this->seekbutton, enable ? TRUE : FALSE);
} // setViewing()

// *************************************************************************

/*!
  Overloaded from parent to make sure the user interface indicators on
  the camera features in the preferences sheet are updated correctly.
*/

void
SoGtkFullViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );

  if (this->prefwindow) {
    this->setZoomSliderPosition(this->getCameraZoom());
    this->setZoomFieldString(this->getCameraZoom());

    const gboolean enable = camera ? TRUE : FALSE;
    gtk_widget_set_sensitive(this->zoomslider, enable);
    gtk_widget_set_sensitive(this->zoomfield, enable); 
    gtk_widget_set_sensitive(this->zoomrangefrom, enable);
    gtk_widget_set_sensitive(this->zoomrangeto, enable); 
  }
}

// *************************************************************************

/*!
  Overloaded from parent class to make sure the preferences window
  will be hidden automatically whenever the viewer window is hidden.
*/

void
SoGtkFullViewer::hide(
  void )
{
  inherited::hide();
  if ( this->prefwindow )
    gtk_widget_hide( this->prefwindow );
} // hide()

// *************************************************************************

/*!
  \internal

  Catch close events on the preferences window (to convert to hide events)
  and right mouse button presses (to pop up the preferences menu).
*/

SbBool
SoGtkFullViewer::eventFilter(
  GtkObject * obj,
  GdkEvent * ev )
{
  if ( inherited::eventFilter( obj, ev ) )
    return TRUE;
  return FALSE;
} // eventFilter()

// *************************************************************************

/*!
  This will build the main view widgets, along with the decorations
  widgets and popup menu if they are enabled.
*/

GtkWidget *
SoGtkFullViewer::buildWidget(
  GtkWidget * parent )
{
  GtkWidget * root = gtk_vbox_new( FALSE, 0 );
  g_return_val_if_fail( root != NULL, (GtkWidget *) NULL );
  GtkWidget * croot = gtk_hbox_new( FALSE, 0 );
  g_return_val_if_fail( croot != NULL, (GtkWidget *) NULL );

  this->canvas = inherited::buildWidget( croot );

  this->viewerWidget = croot;
  this->canvasParent = parent;

  if ( this->decorations ) {
    this->buildDecoration( parent );
//    this->showDecorationWidgets( TRUE );
  }

  gtk_box_pack_start( GTK_BOX(croot), GTK_WIDGET(this->leftDecoration), FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(croot), GTK_WIDGET(this->canvas), TRUE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(croot), GTK_WIDGET(this->rightDecoration), FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(root), GTK_WIDGET(croot), TRUE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(root), GTK_WIDGET(this->bottomDecoration), FALSE, TRUE, FALSE );
  gtk_container_add( GTK_CONTAINER(parent), GTK_WIDGET(root) );

  gtk_widget_show( GTK_WIDGET(this->leftDecoration) );
  gtk_widget_show( GTK_WIDGET(this->canvas) );
  gtk_widget_show( GTK_WIDGET(this->rightDecoration) );
  gtk_widget_show( GTK_WIDGET(croot) );
  gtk_widget_show( GTK_WIDGET(this->bottomDecoration) );

  if ( this->menuEnabled )
    this->buildPopupMenu();

  this->setSize( SbVec2s( 500, 390 ) );
  this->viewerWidget = root;
  return this->viewerWidget;
} // buildWidget()

// *************************************************************************

/*!
  Build viewer decorations.
*/

void
SoGtkFullViewer::buildDecoration(
  GtkWidget * parent )
{
  this->leftDecoration   = this->buildLeftTrim( parent );
  this->bottomDecoration = this->buildBottomTrim( parent );
  this->rightDecoration  = this->buildRightTrim( parent );
} // buildDecoration()

// *************************************************************************

/*!
  Build decorations on the left of the render canvas.  Overload this
  method in subclasses if you want your own decorations on the viewer
  window.
*/

/*
  standard trim is guaranteed to be 30 pixels wide
*/

GtkWidget *
SoGtkFullViewer::buildLeftTrim(
  GtkWidget * parent )
{
  GtkWidget * trim = gtk_vbox_new( FALSE, TRUE );
  g_return_val_if_fail( trim != NULL, (GtkWidget *) NULL );
  gtk_widget_set_usize( GTK_WIDGET(trim), 30, 0 );
  // set background color

#ifndef NO_THUMBWHEELS
  this->leftWheel = gtk_thumbwheel_new( 1 );
  g_return_val_if_fail( this->leftWheel != NULL, (GtkWidget *) NULL );
  gtk_misc_set_padding( GTK_MISC(this->leftWheel), 2, 2 );

  gtk_signal_connect( GTK_OBJECT(this->leftWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->leftWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->leftWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelReleasedCB), (gpointer) this );

  gtk_box_pack_end( GTK_BOX(trim), GTK_WIDGET(this->leftWheel), FALSE, FALSE, TRUE );
  gtk_widget_show( GTK_WIDGET(this->leftWheel) );
#endif

  return trim;
} // buildLeftTrim()

// *************************************************************************

/*!
  Build decorations on the bottom of the render canvas. Overload this
  method in subclasses if you want your own decorations on the viewer window.
*/

GtkWidget *
SoGtkFullViewer::buildBottomTrim(
  GtkWidget * parent )
{
  GtkWidget * trim = gtk_hbox_new( FALSE, TRUE );
  g_return_val_if_fail( trim != NULL, (GtkWidget *) NULL );
  gtk_widget_set_usize( trim, 0, 30 );

  this->leftWheelLabel = gtk_label_new( this->leftWheelStr );
  g_return_val_if_fail( this->leftWheelLabel != NULL, (GtkWidget *) NULL );
  gtk_misc_set_padding( GTK_MISC(this->leftWheelLabel), 3, 0 );

  this->bottomWheelLabel = gtk_label_new( this->bottomWheelStr );
  g_return_val_if_fail( this->bottomWheelLabel != NULL, (GtkWidget *) NULL );
  gtk_misc_set_padding( GTK_MISC(this->bottomWheelLabel), 3, 0 );

  this->rightWheelLabel = gtk_label_new( this->rightWheelStr );
  g_return_val_if_fail( this->rightWheelLabel != NULL, (GtkWidget *) NULL );
  gtk_misc_set_padding( GTK_MISC(this->rightWheelLabel), 3, 0 );

#ifndef NO_THUMBWHEELS
  this->bottomWheel = gtk_thumbwheel_new( 0 );
  g_return_val_if_fail( this->bottomWheel != NULL, (GtkWidget *) NULL );
  gtk_misc_set_padding( GTK_MISC(this->bottomWheel), 2, 2 );

  gtk_signal_connect( GTK_OBJECT(this->bottomWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::bottomwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->bottomWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::bottomwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->bottomWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::bottomwheelReleasedCB), (gpointer) this );
#endif

  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(this->leftWheelLabel), FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(this->bottomWheelLabel), FALSE, TRUE, FALSE );
#ifndef NO_THUMBWHEELS
  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(this->bottomWheel), FALSE, TRUE, TRUE );
#endif
  gtk_box_pack_end( GTK_BOX(trim), GTK_WIDGET(this->rightWheelLabel), FALSE, TRUE, FALSE );
  gtk_widget_show( GTK_WIDGET(this->leftWheelLabel) );
  gtk_widget_show( GTK_WIDGET(this->bottomWheelLabel) );
#ifndef NO_THUMBWHEELS
  gtk_widget_show( GTK_WIDGET(this->bottomWheel) );
#endif
  gtk_widget_show( GTK_WIDGET(this->rightWheelLabel) );

  return trim;
} // buildBottomTrim()

// *************************************************************************

/*!
  Build decorations on the right side of the render canvas. Overload this
  method in subclasses if you want your own decorations on the viewer window.
*/

GtkWidget *
SoGtkFullViewer::buildRightTrim(
  GtkWidget * parent )
{
  GtkWidget * trim = gtk_vbox_new( FALSE, TRUE );
  g_return_val_if_fail( trim != NULL, (GtkWidget *) NULL );
  gtk_widget_set_usize( trim, 30, 0 );

  GtkWidget * buttons = this->buildViewerButtons( trim );
  g_return_val_if_fail( buttons != NULL, (GtkWidget *) NULL );
  gtk_widget_set_usize( buttons, 30, 0 );
  gtk_widget_show( buttons );

#ifndef NO_THUMBWHEELS
  this->rightWheel = gtk_thumbwheel_new( 1 );
  g_return_val_if_fail( this->rightWheel != NULL, (GtkWidget *) NULL );
  gtk_misc_set_padding( GTK_MISC(this->rightWheel), 2, 2 );

  gtk_signal_connect( GTK_OBJECT(this->rightWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::rightwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->rightWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::rightwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->rightWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::rightwheelReleasedCB), (gpointer) this );
#endif

  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(buttons), FALSE, TRUE, TRUE );
#ifndef NO_THUMBWHEELS
  gtk_box_pack_end( GTK_BOX(trim), GTK_WIDGET(this->rightWheel), FALSE, TRUE, TRUE );

  gtk_widget_show( GTK_WIDGET(this->rightWheel) );
#endif

  return trim;
} // buildRightTrim()

// *************************************************************************

/*!
  Build the application specified button row (if any buttons were
  set up).
*/

GtkWidget *
SoGtkFullViewer::buildAppButtons(GtkWidget * parent)
{
/*
  this->appbuttonform = new GtkWidget(parent);

  if (this->lengthAppPushButton() > 0)
    this->layoutAppButtons(this->appbuttonform);

*/
  return this->appButtonForm;
}

// *************************************************************************

/*!
  Build and layout viewer specified button row.
*/

GtkWidget *
SoGtkFullViewer::buildViewerButtons(
  GtkWidget * parent )
{
  GtkWidget * buttons = gtk_vbox_new( FALSE, FALSE );

  this->viewerButtons = new SbPList;
  this->createViewerButtons( buttons, this->viewerButtons );

  const int num = this->viewerButtons->getLength();
  for ( int i = 0; i < num; i++ ) {
    GtkWidget * widget = (GtkWidget *) (*this->viewerButtons)[i];
    gtk_widget_set_usize( widget, 30, 30 );
    gtk_box_pack_start( GTK_BOX(buttons), widget, TRUE, FALSE, FALSE );
    gtk_widget_show( widget );
  }
  gtk_widget_show( buttons );
  return buttons;
} // buildViewerButtons()

// *************************************************************************

/*!
  Create the viewer buttons with pixmaps and event connections.
  Overload this function to add buttons to the viewer button row.
*/

void
SoGtkFullViewer::createViewerButtons( // virtual
  GtkWidget * parent,
  SbPList * buttonlist )
{
  GtkTooltips *tooltips = gtk_tooltips_new ();

  GdkColormap *colormap = gtk_widget_get_colormap (parent);
  GdkBitmap *mask ;

  GSList *viewing_group = NULL ;
  const size_t buttons = sizeof(SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);
  for ( size_t button = 0; button < buttons; button++ ) {
    GtkWidget * widget = (GtkWidget*) 0 ;
    switch (button) {
    case INTERACT_BUTTON:
      this->interactbutton = widget = gtk_radio_button_new(viewing_group);
      gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(widget),FALSE);
      viewing_group = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
        this->isViewing() ? FALSE : TRUE );
      break ;
    case EXAMINE_BUTTON:
      this->viewbutton = widget = gtk_radio_button_new(viewing_group);
      gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(widget),FALSE);
      viewing_group = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
        this->isViewing() ? TRUE : FALSE );
      break ;
    case SEEK_BUTTON:
      this->seekbutton = widget = gtk_button_new();
      gtk_widget_set_sensitive(widget,this->isViewing() ? TRUE : FALSE);
      break ;
    default:
      widget = gtk_button_new();
      break ;
    }

    gtk_tooltips_set_tip (tooltips, widget,
      _( SoGtkFullViewerButtons[button].keyword ), (const gchar *) NULL);

    GdkPixmap * gdk_pixmap =
      gdk_pixmap_colormap_create_from_xpm_d( (GdkWindow *) NULL, colormap,
        &mask, (GdkColor *) NULL,
        SoGtkFullViewerButtons[button].xpm_data);
    GtkWidget * label = gtk_pixmap_new( gdk_pixmap, mask );
    gtk_widget_show( label );

    gdk_pixmap_unref (gdk_pixmap);
    gdk_bitmap_unref (mask);

    gtk_container_add( GTK_CONTAINER(widget), GTK_WIDGET(label) );
    if ( (void *) SoGtkFullViewerButtons[button].pressed != NULL ) {
      gtk_signal_connect( GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(SoGtkFullViewerButtons[button].pressed),
        (gpointer) this );
    }
    buttonlist->append( widget );
  }
} // createViewerButtons()

// *************************************************************************

/*!
  Make a popup menu with preferences settings.
*/

void
SoGtkFullViewer::buildPopupMenu(
  void )
{
  this->prefmenu = common->setupStandardPopupMenu();
  assert( this->prefmenu != NULL );
} // buildPopupMenu()

// *************************************************************************

/*!
  Set title of popup menu.
*/

void
SoGtkFullViewer::setPopupMenuString(
  const char * str )
{
  this->menuTitle = str ? str : "";
//  SOGTK_STUB();
//  if ( this->prefMenu )
//    this->prefMenu->changeItem( this->menutitle.getString(),
//                                MENUTITLE_ITEM );
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::openPopupMenu(
  const SbVec2s position )
{
  if ( ! this->prefmenu && this->menuEnabled )
    this->buildPopupMenu();
  if ( this->prefmenu ) {
    this->common->prepareMenu( this->prefmenu );
    this->prefmenu->popUp( this->getGLWidget(), position[0], position[1] );
  }
} // openPopupMenu()

// *************************************************************************

/*!
  Overload this method in subclass viewers to append more widgets to
  the bottom of the preferences sheet window.
*/

GtkWidget *
SoGtkFullViewer::makeSubPreferences(
  GtkWidget * ) // parent )
{
  return (GtkWidget *) NULL;
}

// *************************************************************************

/*!
  Set title of preferences sheet.
*/

void
SoGtkFullViewer::setPrefSheetString(
  const char * title )
{
  this->prefwindowtitle = title ? title : "";
  if (this->prefwindow)
    gtk_window_set_title (GTK_WINDOW (this->prefwindow),
      this->prefwindowtitle.getString());
}

// *************************************************************************

/*!
  Called when the user start to drag the thumbwheel in the left frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa leftWheelMotion(), leftWheelFinish()
  \sa bottomWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::leftWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // leftWheelStart()

/*!
  Called repeatedly as the user drags the thumbwheel in the left frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa leftWheelStart(), leftWheelFinish()
  \sa bottomWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::leftWheelMotion( // virtual
  float value )
{
  this->leftWheelVal = value;
} // leftWheelMotion()

/*!
  Called as the user let go of the thumbwheel in the left frame after
  a drag operation. Overload this method in subclassed viewers to provide
  your own functionality on the thumbwheel.

  \sa leftWheelStart(), leftWheelMotion()
  \sa bottomWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::leftWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // leftWheelFinish()

/*!
  Get current value of the left thumbwheel.

  \sa leftWheelMotion()
*/

float
SoGtkFullViewer::getLeftWheelValue(
  void ) const
{
  return this->leftWheelVal;
} // getLeftWheelValue()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::setLeftWheelValue(
  const float value )
{
  this->leftWheelVal = value;
} // setLeftWheelValue()

// *************************************************************************

/*!
  Called when the user start to drag the thumbwheel in the bottom frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa bottomWheelMotion(), bottomWheelFinish()
  \sa leftWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::bottomWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // bottomWheelStart()

/*!
  Called repeatedly as the user drags the thumbwheel in the bottom frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa bottomWheelStart(), bottomWheelFinish()
  \sa leftWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::bottomWheelMotion( // virtual
  float value )
{
  this->bottomWheelVal = value;
} // bottomWheelMotion()

/*!
  Called as the user let go of the thumbwheel in the bottom frame after
  a drag operation. Overload this method in subclassed viewers to provide
  your own functionality on the thumbwheel.

  \sa bottomWheelStart(), bottomWheelMotion()
  \sa leftWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::bottomWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // bottomWheelFinish()

/*!
  Get current value of the bottom thumbwheel.

  \sa bottomWheelMotion()
*/

float
SoGtkFullViewer::getBottomWheelValue(
  void ) const
{
  return this->bottomWheelVal;
} // getBottomWheelValue()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::setBottomWheelValue(
  const float value )
{
  this->bottomWheelVal = value;
} // setBottomWheelValue()

// *************************************************************************

/*!
  Called when the user start to drag the thumbwheel in the right frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa rightWheelMotion(), rightWheelFinish()
  \sa leftWheelStart(), bottomWheelStart()
*/

void
SoGtkFullViewer::rightWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // rightWheelStart()

/*!
  Called repeatedly as the user drags the thumbwheel in the right frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa rightWheelStart(), rightWheelFinish()
  \sa leftWheelStart(), bottomWheelStart()
*/

void
SoGtkFullViewer::rightWheelMotion( // virtual
  float value )
{
  this->rightWheelVal = value;
} // rightWheelMotion()

/*!
  Called as the user let go of the thumbwheel in the right frame after
  a drag operation. Overload this method in subclassed viewers to provide
  your own functionality on the thumbwheel.

  \sa rightWheelStart(), rightWheelMotion()
  \sa leftWheelStart(), bottomWheelStart()
*/

void
SoGtkFullViewer::rightWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // rightWheelFinish()

/*!
  Get current value of the right thumbwheel.

  \sa rightWheelMotion()
*/

float
SoGtkFullViewer::getRightWheelValue(
  void ) const
{
  return this->rightWheelVal;
} // getRightWheelValue()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::setRightWheelValue(
  const float value )
{
  this->rightWheelVal = value;
} // setRightWheelValue()

// *************************************************************************

/*!
  Set label for the left thumbwheel.
*/

void
SoGtkFullViewer::setLeftWheelString(
  const char * string )
{
  delete [] this->leftWheelStr;
  this->leftWheelStr = (char *) NULL;
  if ( string )
    this->leftWheelStr = strcpy( new char [strlen(string)+1], string );
  if ( this->leftWheelLabel )
    gtk_label_set_text( GTK_LABEL(this->leftWheelLabel),
      this->leftWheelStr ? this->leftWheelStr : "" );
} // setLeftWheelString()

// *************************************************************************

/*!
  Set label of the bottom thumbwheel.
*/

void
SoGtkFullViewer::setBottomWheelString(
  const char * string )
{
  delete [] this->bottomWheelStr;
  this->bottomWheelStr = (char *) NULL;
  if ( string )
    this->bottomWheelStr = strcpy( new char [strlen(string)+1], string );
  if ( this->bottomWheelLabel )
    gtk_label_set_text( GTK_LABEL(this->bottomWheelLabel),
      this->bottomWheelStr ? this->bottomWheelStr : "" );
} // setBottomWheelString()

// *************************************************************************

/*!
  Set label of the right thumbwheel.
*/

void
SoGtkFullViewer::setRightWheelString(
  const char * string )
{
  delete [] this->rightWheelStr;
  this->rightWheelStr = (char *) NULL;
  if ( string )
    this->rightWheelStr = strcpy( new char [strlen(string)+1], string );
  if ( this->rightWheelLabel )
    gtk_label_set_text( GTK_LABEL(this->rightWheelLabel),
      this->rightWheelStr ? this->rightWheelStr : "" );
} // setRightWheelString()

// *************************************************************************

/*!
  Overload this method to provide functionality when the user clicks
  the Help button.
*/

void
SoGtkFullViewer::openViewerHelpCard( // virtual
  void )
{
  const char * classname = this->getClassName();
  if ( classname && strlen(classname) > 0 )
    SoGtkComponent::openHelpCard( classname );
} // openViewerHelpCard()

// *************************************************************************

/*!
  \internal

  Show or hide decorations. Will make and activate a Qt layout grid
  if we're turning the decorations on.
*/

void
SoGtkFullViewer::showDecorationWidgets(
  SbBool enable )
{
//  if ( this->mainLayout )
//    delete this->mainLayout;

  assert(this->viewerWidget);
  assert(this->canvasParent);

/*
  if ( enable )
    gtk_widget_show( GTK_WIDGET(this->rightDecoration) );
  else
    gtk_widget_hide( GTK_WIDGET(this->rightDecoration) );
*/

/*
  if (onOff) {
    for (int i = FIRSTDECORATION; i <= LASTDECORATION; i++) {
      assert(this->decorform[i]);
      this->decorform[i]->show();
    }

    QGridLayout * g = new QGridLayout(this->viewerwidget, 2, 1, 0, -1 ); // VIEWERBORDER);

    g->addWidget(this->decorform[BOTTOMDECORATION], 1, 0);

    QGridLayout * subLayout = new QGridLayout( 1, 3, 0 );
    g->addLayout(subLayout, 0, 0);

    subLayout->addWidget(this->decorform[LEFTDECORATION], 0, 0);
    subLayout->addWidget(this->canvasparent, 0, 1);
    subLayout->addWidget(this->decorform[RIGHTDECORATION], 0, 2);

//     subLayout->setColStretch(1, 1);
//     g->setRowStretch(0, 1);

    this->mainlayout = g;
  }
  else {
    QGridLayout * g = new QGridLayout(this->viewerwidget, 1, 1, 0, -1 );
    g->addWidget(this->canvasparent, 0, 0);
    this->mainlayout = g;

    for (int i = FIRSTDECORATION; i <= LASTDECORATION; i++)
      this->decorform[i]->hide();
  }

  this->mainlayout->activate();
*/
}

// *************************************************************************

/*!
  Layout application specified buttons.
*/

void
SoGtkFullViewer::layoutAppButtons(
  GtkWidget * form )
{
}

// *************************************************************************

/*!
  Create preferences sheet.
*/

GtkWidget *
SoGtkFullViewer::makePreferencesWindow(
  void )
{
  this->prefwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (this->prefwindow),
    this->prefwindowtitle.getString());
  gtk_window_set_position (GTK_WINDOW (this->prefwindow),  GTK_WIN_POS_MOUSE);

  GtkWidget* form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (this->prefwindow), form);

  GtkWidget* w ;
  w = makeSeekPreferences(form);

  w = makeSeekDistancePreferences(form);

  w = makeZoomPreferences(form);

  w = makeAutoclipPreferences(form);

  w = makeStereoPreferences(form);

  w = makeSubPreferences(form);

  gtk_signal_connect (GTK_OBJECT (this->prefwindow), "destroy",
                      GTK_SIGNAL_FUNC (preferencesDestroyed),
                      (gpointer) this);

  return this->prefwindow ;
}

/*!
  \internal
  Gtk Signal Handler.
*/
void SoGtkFullViewer::preferencesDestroyed(
  GtkObject 		*object,
  gpointer         	closure)
{
  SoGtkFullViewer* viewer = (SoGtkFullViewer*) closure;
  viewer->prefwindow = (GtkWidget *) NULL;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  seek-to-point functionality.
*/

GtkWidget *
SoGtkFullViewer::makeSeekPreferences(
  GtkWidget * parent )
{
  GSList *rbg1 = (GSList *) NULL;

  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget *hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (form), hbox2, TRUE, TRUE, 0);

  GtkWidget *label6 = gtk_label_new ( _( "Seek animation time:" ) );
  gtk_widget_show (label6);
  gtk_box_pack_start (GTK_BOX (hbox2), label6, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label6), 4, 0);

  GtkWidget *entry3 = gtk_entry_new();
  gtk_widget_show (entry3);
  gtk_box_pack_start (GTK_BOX (hbox2), entry3, FALSE, FALSE, 0);
  gtk_widget_set_usize (entry3, 48, 24);

  char buffer[16];
  sprintf( buffer, "%.2f", this->getSeekTime() );
  gtk_entry_set_text (GTK_ENTRY (entry3), buffer );

  GtkWidget *label7 = gtk_label_new ( _( "seconds" ) );
  gtk_widget_show (label7);
  gtk_box_pack_start (GTK_BOX (hbox2), label7, FALSE, TRUE, 0);

  GtkWidget *hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (form), hbox3, TRUE, TRUE, 0);

  GtkWidget *label8 = gtk_label_new ( _( "Seek to:" ) );
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox3), label8, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label8), 4, 0);

  GtkWidget *rb1 = gtk_radio_button_new_with_label (rbg1, _( "point" ) );
  rbg1 = gtk_radio_button_group (GTK_RADIO_BUTTON (rb1));
  gtk_widget_show (rb1);
  gtk_box_pack_start (GTK_BOX (hbox3), rb1, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rb1),
    this->isDetailSeek());

  GtkWidget *rb2 = gtk_radio_button_new_with_label (rbg1, _( "object" ));
  rbg1 = gtk_radio_button_group (GTK_RADIO_BUTTON (rb2));
  gtk_widget_show (rb2);
  gtk_box_pack_start (GTK_BOX (hbox3), rb2, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rb1),
    !this->isDetailSeek());

  gtk_signal_connect (GTK_OBJECT (entry3), "activate",
                      GTK_SIGNAL_FUNC (seekAnimationTimeChanged),
                      (gpointer) this);

  gtk_signal_connect (GTK_OBJECT (rb1), "toggled",
                      GTK_SIGNAL_FUNC (seekDetailToggled),
                      (gpointer) this);

  return form;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  seek-to-point functionality.
*/

GtkWidget *
SoGtkFullViewer::makeSeekDistancePreferences(
  GtkWidget * parent )
{
  GSList *bg = (GSList *) NULL;

  GtkWidget* form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget* hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (form), hbox4, TRUE, TRUE, 0);

  GtkWidget *label4 = gtk_label_new ( _( "Seek distance:" ) );
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (hbox4), label4, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label4), 4, 0);

  this->seekdistancewheel = gtk_thumbwheel_new (0);
  gtk_thumbwheel_set_range_boundary_handling(
    GTK_THUMBWHEEL(this->seekdistancewheel),
    GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE );
  gtk_thumbwheel_set_value( GTK_THUMBWHEEL(this->seekdistancewheel),
    sqrt(this->getSeekDistance()) );
  gtk_widget_show (this->seekdistancewheel);
  gtk_box_pack_start (GTK_BOX (hbox4), this->seekdistancewheel, FALSE, FALSE, 0);

  this->seekdistancefield = gtk_entry_new ();
  gtk_widget_show (this->seekdistancefield);
  gtk_widget_set_usize(this->seekdistancefield,64,0);
  gtk_entry_set_max_length(GTK_ENTRY(this->seekdistancefield), 6);
  gtk_box_pack_start (GTK_BOX (hbox4), this->seekdistancefield, FALSE, FALSE, 0);

  char buffer[16];
  sprintf( buffer, "%.2f", this->getSeekDistance() );
  gtk_entry_set_text (GTK_ENTRY (this->seekdistancefield), buffer );

  GtkWidget *hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (form), hbox5, TRUE, FALSE, 0);

  GtkWidget *r1 = gtk_radio_button_new_with_label (bg, _( "percentage" ) );
  bg = gtk_radio_button_group (GTK_RADIO_BUTTON (r1));
  gtk_widget_show (r1);
  gtk_box_pack_start (GTK_BOX (hbox5), r1, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r1),
    this->isSeekValuePercentage() );

  GtkWidget *r2 = gtk_radio_button_new_with_label (bg, _( "absolute" ) );
  bg = gtk_radio_button_group (GTK_RADIO_BUTTON (r2));
  gtk_widget_show (r2);
  gtk_box_pack_start (GTK_BOX (hbox5), r2, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r2),
    !this->isSeekValuePercentage() );

  gtk_signal_connect (GTK_OBJECT (this->seekdistancefield), "activate",
                      GTK_SIGNAL_FUNC (seekDistanceEdit),
                      (gpointer) this);
  gtk_signal_connect (GTK_OBJECT (this->seekdistancewheel), "value_changed",
                      GTK_SIGNAL_FUNC (seekDistanceWheelChanged),
                      (gpointer) this);
  gtk_signal_connect (GTK_OBJECT (r1), "toggled",
                      GTK_SIGNAL_FUNC (seekDistanceTypeToggle),
                      (gpointer) this);

  return form;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  camera zoom functionality.
*/

GtkWidget *
SoGtkFullViewer::makeZoomPreferences(
  GtkWidget * parent )
{
  char buffer[16] ;

  GtkWidget* form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget *hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (form), hbox1, TRUE, FALSE, 0);

  GtkWidget *label9 = gtk_label_new ( _( "Camera Zoom:" ) );
  gtk_widget_show (label9);
  gtk_box_pack_start (GTK_BOX (hbox1), label9, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label9), 0.0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label9), 4, 0);

  GtkObject *adj = gtk_adjustment_new (
     0.0, 0.0, ZOOMSLIDERRESOLUTION, 1.0, 0, 0) ;
  this->zoomslider = gtk_hscale_new (GTK_ADJUSTMENT (adj));
  gtk_scale_set_draw_value (GTK_SCALE (this->zoomslider), FALSE);
  gtk_widget_show (this->zoomslider);
  gtk_box_pack_start (GTK_BOX (hbox1), this->zoomslider, FALSE, TRUE, 0);

  this->zoomfield = gtk_entry_new();
  gtk_widget_show (this->zoomfield);
  gtk_widget_set_usize(this->zoomfield,64,20);
  gtk_entry_set_max_length(GTK_ENTRY(this->zoomfield), 6);
  gtk_box_pack_start (GTK_BOX (hbox1), this->zoomfield, FALSE, FALSE, 0);

  GtkWidget *hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (form), hbox2, TRUE, TRUE, 0);

  GtkWidget *label10 = gtk_label_new ( _( "Zoom slider ranges from:" ) );
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (hbox2), label10, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label10), 4, 0);

  this->zoomrangefrom = gtk_entry_new();
  sprintf( buffer, "%.1f", this->zoomrange[0] );
  gtk_entry_set_text( GTK_ENTRY(this->zoomrangefrom), buffer );
  gtk_widget_set_usize(this->zoomrangefrom,64,20);
  gtk_widget_show (this->zoomrangefrom);
  gtk_box_pack_start (GTK_BOX (hbox2), this->zoomrangefrom, FALSE, FALSE, 0);

  GtkWidget *label11 = gtk_label_new ( _( "to:" ) );
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox2), label11, FALSE, FALSE, 0);

  this->zoomrangeto = gtk_entry_new ();
  sprintf( buffer, "%.1f", this->zoomrange[1] );
  gtk_entry_set_text( GTK_ENTRY(this->zoomrangeto), buffer );
  gtk_widget_set_usize(this->zoomrangeto,64,20);
  gtk_widget_show (this->zoomrangeto);
  gtk_box_pack_start (GTK_BOX (hbox2), this->zoomrangeto, FALSE, FALSE, 0);

  SoCamera *cam = this->getCamera();
  if ( cam )
  {
    this->setZoomSliderPosition(this->getCameraZoom());
    this->setZoomFieldString(this->getCameraZoom());
  }
  else
  {
    gtk_widget_set_sensitive(this->zoomslider, FALSE);
    gtk_widget_set_sensitive(this->zoomfield, FALSE); 
    gtk_widget_set_sensitive(this->zoomrangefrom, FALSE);
    gtk_widget_set_sensitive(this->zoomrangeto, FALSE); 
  }

  gtk_signal_connect (GTK_OBJECT (this->zoomfield), "activate",
                      GTK_SIGNAL_FUNC (zoomFieldChanged),
                      (gpointer) this);

  gtk_signal_connect (GTK_OBJECT (this->zoomrangefrom), "activate",
                      GTK_SIGNAL_FUNC (zoomRangeChanged1),
                      (gpointer) this);

  gtk_signal_connect (GTK_OBJECT (this->zoomrangeto), "activate",
                      GTK_SIGNAL_FUNC (zoomRangeChanged2),
                      (gpointer) this);

  gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
                      GTK_SIGNAL_FUNC (zoomSliderMoved),
                      (gpointer) this);

  return form;
}

// *************************************************************************

/*!
  Enable or disable interaction through the near and far clipping
  widgets.
*/

void
SoGtkFullViewer::setEnabledClippingWidgets(
  SbBool flag )
{
#if 1
  gtk_widget_set_sensitive( this->clippingtable, flag );
#else
// FIXME: This trips a bug in Gtk's interaction with gtkthumbwheel
// Could be a bug in gtk_table or in gtk_thumbwheel :)
  if ( flag )
  {
    gtk_widget_show( this->clippingtable );
//    gtk_widget_show( this->nearclippinglabel );
//    gtk_widget_show( this->nearclippingwheel );
//    gtk_widget_show( this->nearclippingedit ); 
//    gtk_widget_show( this->farclippinglabel ); 
//    gtk_widget_show( this->farclippingwheel ); 
//    gtk_widget_show( this->farclippingedit );  
  }
  else
  {
    gtk_widget_hide( this->clippingtable );
//    gtk_widget_hide( this->nearclippinglabel );
//    gtk_widget_hide( this->nearclippingwheel );
//    gtk_widget_hide( this->nearclippingedit ); 
//    gtk_widget_hide( this->farclippinglabel ); 
//    gtk_widget_hide( this->farclippingwheel ); 
//    gtk_widget_hide( this->farclippingedit );  
  }
#endif
  SoCamera * cam = this->getCamera();
  if ( !cam ) return ;

  {
    char buffer[16] ;

    sprintf( buffer, "%.3f", cam->nearDistance.getValue() );
    gtk_entry_set_text( GTK_ENTRY(this->nearclippingedit), buffer );

    sprintf( buffer, "%.3f", cam->farDistance.getValue() );
    gtk_entry_set_text( GTK_ENTRY(this->farclippingedit), buffer );
  }
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  manual control of the near and far clippping planes.
*/

GtkWidget *
SoGtkFullViewer::makeAutoclipPreferences(
  GtkWidget * dialog )
{
  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (dialog), form);

  GtkWidget *checkbutton1 = gtk_check_button_new_with_label ( 
    _( "Auto clipping planes" ) );
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), 
     this->isAutoClipping());
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (form), checkbutton1, FALSE, FALSE, 0);

  this->clippingtable = gtk_table_new (2, 3, FALSE);
  gtk_box_pack_start (GTK_BOX (form), this->clippingtable, FALSE, FALSE, 0);
  gtk_widget_show( this->clippingtable );

  this->nearclippinglabel = gtk_label_new ( 
    _( "near plane:" ) );
  gtk_label_set_justify (GTK_LABEL (this->nearclippinglabel), GTK_JUSTIFY_RIGHT); 
  gtk_misc_set_alignment (GTK_MISC (this->nearclippinglabel), 1, 0.5);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->nearclippinglabel, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show( this->nearclippinglabel );

  this->nearclippingwheel = gtk_thumbwheel_new (0);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->nearclippingwheel, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show( this->nearclippingwheel );

  this->nearclippingedit = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->nearclippingedit, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show( this->nearclippingedit );

  this->farclippinglabel = gtk_label_new ( 
    _( "far plane:" ) );
  gtk_label_set_justify (GTK_LABEL (this->farclippinglabel), GTK_JUSTIFY_RIGHT); 
  gtk_misc_set_alignment (GTK_MISC (this->farclippinglabel), 1, 0.5);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->farclippinglabel, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show( this->farclippinglabel );

  this->farclippingwheel = gtk_thumbwheel_new (0);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->farclippingwheel, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show( this->farclippingwheel );

  this->farclippingedit = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->farclippingedit, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show( this->farclippingedit );

  gtk_signal_connect (GTK_OBJECT (checkbutton1), 
    "toggled", GTK_SIGNAL_FUNC (clippingToggled), this);

  gtk_signal_connect(GTK_OBJECT(this->nearclippingwheel), 
    "attached", GTK_SIGNAL_FUNC (increaseInteractiveCount), this);
  gtk_signal_connect(GTK_OBJECT(this->nearclippingwheel), 
    "value_changed", GTK_SIGNAL_FUNC (nearclippingwheelMoved), this);
  gtk_signal_connect(GTK_OBJECT(this->nearclippingwheel), 
    "released", GTK_SIGNAL_FUNC (decreaseInteractiveCount), this);

  gtk_signal_connect(GTK_OBJECT(this->farclippingwheel), 
    "attached", GTK_SIGNAL_FUNC (increaseInteractiveCount), this);
  gtk_signal_connect(GTK_OBJECT(this->farclippingwheel), 
    "value_changed", GTK_SIGNAL_FUNC (farclippingwheelMoved), this);
  gtk_signal_connect(GTK_OBJECT(this->farclippingwheel), 
    "released", GTK_SIGNAL_FUNC (decreaseInteractiveCount), this);

  this->setEnabledClippingWidgets(!this->isAutoClipping());

  return form;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  stereo viewing controls.
*/

GtkWidget *
SoGtkFullViewer::makeStereoPreferences(
  GtkWidget * dialog )
{
  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (dialog), form);

  GtkWidget *checkbutton1 = gtk_check_button_new_with_label ( 
    _( "Stereo Viewing" ) );
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), 
     this->isStereoViewing());
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (form), checkbutton1, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (checkbutton1), 
    "toggled", GTK_SIGNAL_FUNC (stereoToggled), this);

  return form;
}

// *************************************************************************

/*!
  Set camera zoom value.
*/

void
SoGtkFullViewer::setCameraZoom(const float val)
{
  SoCamera * cam = this->getCamera();
  if ( cam == NULL ) return; // can happen if scenegraph is empty

  SoType t = cam->getTypeId();

  if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
    ((SoPerspectiveCamera *)cam)->heightAngle = val * 2.0f * M_PI / 360.0f;
  else if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
    ((SoOrthographicCamera *)cam)->height = val;
#if SOGTK_DEBUG
  else assert(0);
#endif // SOGTK_DEBUG
}

// *************************************************************************
/*!
  Return camera zoom value.
*/
float
SoGtkFullViewer::getCameraZoom(void)
{
  SoCamera * cam = this->getCamera();
  if ( cam == NULL ) return 0.0f; // can happen if scenegraph is empty

  SoType t = cam->getTypeId();

  if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) )
    return ((SoPerspectiveCamera *)cam)->heightAngle.getValue() /
      2.0f * 360.0f / M_PI;
  else if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()) )
    return ((SoOrthographicCamera *)cam)->height.getValue();

#if SOGTK_DEBUG
  assert(0);
#endif // SOGTK_DEBUG
  return 0.0f;
}

// *************************************************************************
/*!
  Update the Gtk HScale representing the camera zoom.
*/
void
SoGtkFullViewer::setZoomSliderPosition(float zoom)
{
  if (!this->prefwindow) return;

  float f =
    (zoom - this->zoomrange[0]) / (this->zoomrange[1] - this->zoomrange[0]);
  f = SoGtkClamp(f, 0.0f, 1.0f) * ZOOMSLIDERRESOLUTION;

  GtkAdjustment *adj = GTK_RANGE(this->zoomslider)->adjustment ;
  gtk_adjustment_set_value( adj, f );
}

// *************************************************************************
/*!
  Set string in the camera zoom edit field.
*/
void
SoGtkFullViewer::setZoomFieldString(float zoom)
{
  if (!this->prefwindow) return;

  char buffer[16];
  sprintf(buffer, "%.1f", zoom);
  gtk_entry_set_text(GTK_ENTRY(this->zoomfield), buffer);
}

// *************************************************************************

/*!
  \internal
  SoGtk Slot.
*/

void
SoGtkFullViewer::selectedViewing()
{
  this->setViewing(this->isViewing() ? FALSE : TRUE);
}

// *************************************************************************

/*!
  \internal
  SoGtk slot.
*/

void
SoGtkFullViewer::selectedDecoration()
{
  this->setDecoration(this->isDecoration() ? FALSE : TRUE);
}

// *************************************************************************

/*!
  \internal
  SoGtk slot.
*/

void
SoGtkFullViewer::selectedHeadlight()
{
  this->setHeadlight(this->isHeadlight() ? FALSE : TRUE);
}

// *************************************************************************

/*!
  \internal
  SoGtk slot.
*/

void
SoGtkFullViewer::copyviewSelected()
{
  this->copyView(SbTime::getTimeOfDay());
}

// *************************************************************************

/*!
  \internal
  SoGtk slot.
*/

void
SoGtkFullViewer::pasteviewSelected()
{
  this->pasteView(SbTime::getTimeOfDay());
}

// *************************************************************************

/*!
  \internal
  SoGtk Slot.
*/

void
SoGtkFullViewer::selectedPrefs(void)
{
  if (!this->prefwindow) this->prefwindow = this->makePreferencesWindow();
  gtk_widget_show( this->prefwindow );
} // selectedPrefs()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::seekAnimationTimeChanged(
  GtkEditable     *editable,
  gpointer         closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  char *s = gtk_editable_get_chars(editable,0,-1);
  float val;
  if ((sscanf(s, "%f", &val) == 1) && (val >= 0.0f)) viewer->setSeekTime(val);
  g_free(s);

  char buffer[16] ;
  sprintf(buffer, "%.2f", viewer->getSeekTime() );
  gtk_entry_set_text( GTK_ENTRY(editable), buffer );
} // seekAnimationTimeChanged()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::seekDetailToggled(
  GtkToggleButton	*button,
  gpointer            	closure )
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;

  viewer->setDetailSeek( gtk_toggle_button_get_active(button) ?
    TRUE : FALSE );
} // seekDetailToggle()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::seekDistanceWheelChanged(
  GtkWidget	*wheel,
  gpointer 	closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  gfloat val = gtk_thumbwheel_get_value( GTK_THUMBWHEEL(wheel) );

  if (val < 0.1f) {
    val = 0.1f;
    gtk_thumbwheel_set_value( GTK_THUMBWHEEL(viewer->seekdistancewheel), val );
  }
  viewer->setSeekDistance(val * val);

  char buffer[16] ;
  sprintf(buffer, "%.2f", viewer->getSeekDistance() );
  gtk_entry_set_text( GTK_ENTRY(viewer->seekdistancefield), buffer );
} // seekDistanceWheelChanged()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::seekDistanceEdit(
  GtkEditable         *editable,
  gpointer            closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  char *s = gtk_editable_get_chars(editable,0,-1);
  float val;

  if ((sscanf(s, "%f", &val) == 1) && (val > 0.0f))
  {
    viewer->setSeekDistance(val);
    gtk_thumbwheel_set_value( GTK_THUMBWHEEL(viewer->seekdistancewheel),
      sqrt(val));
  }
  g_free(s);

  /* else */
  {
    char buffer[16] ;
    sprintf(buffer, "%.2f", viewer->getSeekDistance() );
    gtk_entry_set_text( GTK_ENTRY(viewer->seekdistancefield), buffer );
  }
} // seekDistanceEdit()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::seekDistanceTypeToggle(
  GtkToggleButton	*button,
  gpointer            	closure )
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;

  viewer->setSeekValueAsPercentage( gtk_toggle_button_get_active(button) ?
    TRUE : FALSE );
} // seekDistanceTypeToggle()


// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::zoomSliderMoved(
  GtkAdjustment *adjustment,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;

  float f = adjustment->value / float(ZOOMSLIDERRESOLUTION);
  f = viewer->zoomrange[0] + f * (viewer->zoomrange[1] - viewer->zoomrange[0]);

  viewer->setCameraZoom(f);
  viewer->setZoomFieldString(f);
} // zoomSliderMoved()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::zoomFieldChanged(
  GtkEditable         *editable,
  gpointer            closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer *) closure ;

  float val;
  char *s = gtk_editable_get_chars(editable,0,-1);
  if (sscanf(s, "%f", &val) == 1) {
    val = SoGtkClamp(val, 0.001f, 179.999f );
    viewer->setCameraZoom(val);
    viewer->setZoomSliderPosition(val);
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf( buffer, "%.1f", viewer->getCameraZoom() );
    gtk_entry_set_text( GTK_ENTRY(editable), buffer );
  }
} // zoomFieldChanged()


// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::zoomRangeChanged1(
  GtkEditable         *editable,
  gpointer            closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer *) closure ;

  float val;
  char *s = gtk_editable_get_chars(editable,0,-1);
  if (sscanf(s, "%f", &val) == 1) {
    if (val > 0.0f && val < 180.0f && val < viewer->zoomrange[1]) {
      viewer->zoomrange[0] = val;
      viewer->setZoomSliderPosition(viewer->getCameraZoom());
    }
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf( buffer, "%.1f", viewer->zoomrange[0] );
    gtk_entry_set_text( GTK_ENTRY(editable), buffer );
  }
} // zoomRangeChanged1()


// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::zoomRangeChanged2(
  GtkEditable         *editable,
  gpointer            closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer *) closure ;

  float val;
  char *s = gtk_editable_get_chars(editable,0,-1);
  if (sscanf(s, "%f", &val) == 1) {
    if (val > 0.0f && val < 180.0f && val > viewer->zoomrange[0]) {
      viewer->zoomrange[1] = val;
      viewer->setZoomSliderPosition(viewer->getCameraZoom());
    }
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf( buffer, "%.1f", viewer->zoomrange[1] );
    gtk_entry_set_text( GTK_ENTRY(editable), buffer );
  }
} // zoomRangeChanged2()


// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::clippingToggled(
  GtkToggleButton *toggle_button,
  gpointer         closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  bool flag = gtk_toggle_button_get_active (toggle_button);

  viewer->setAutoClipping(flag);
  viewer->setEnabledClippingWidgets(!flag);
} // clippingToggled()     

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::increaseInteractiveCount(
  GtkWidget	*w,
  gpointer 	closure )
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  viewer->interactiveCountInc();
} // increaseInteractiveCount()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::decreaseInteractiveCount(
  GtkWidget	*w,
  gpointer	closure )
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  viewer->interactiveCountDec();
} // decreaseInteractiveCount()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::nearclippingwheelMoved(
  GtkWidget		*w,
  gpointer		closure )
{
  GtkThumbWheel *nearwheel = (GtkThumbWheel*) w ;
  SoGtkFullViewer* viewer = (SoGtkFullViewer*) closure ;

  assert( viewer->farclippingwheel != NULL );

  float val = gtk_thumbwheel_get_value( nearwheel );
  if ( val < 0.001f ) {
    val = 0.001f;
    gtk_thumbwheel_set_value( nearwheel, val );
  }
  float farval = gtk_thumbwheel_get_value( 
    GTK_THUMBWHEEL(viewer->farclippingwheel) );
  if ( val >= farval ) {
    val = farval - 0.001f;
    gtk_thumbwheel_set_value( GTK_THUMBWHEEL(viewer->nearclippingwheel), val );
  }

  SoCamera * cam = viewer->getCamera();
  if (!cam) return;

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->nearDistance = val;

  char buffer[16] ;
  sprintf( buffer, "%.3f", cam->nearDistance.getValue());
  gtk_entry_set_text( GTK_ENTRY(viewer->nearclippingedit), buffer );
} // nearclippingwheelMoved()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::farclippingwheelMoved(
  GtkWidget		*w,
  gpointer		closure )
{
  GtkThumbWheel *farwheel = (GtkThumbWheel*) w ;
  SoGtkFullViewer* viewer = (SoGtkFullViewer*) closure ;

  assert( viewer->nearclippingwheel != NULL );

  float val = gtk_thumbwheel_get_value( farwheel );
  float nearval = gtk_thumbwheel_get_value( 
    GTK_THUMBWHEEL(viewer->nearclippingwheel) );
  if ( val < nearval ) {
    val = nearval + 0.001f;
    gtk_thumbwheel_set_value( farwheel, val );
  }

  SoCamera * cam = viewer->getCamera();
  if (!cam) return ;

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->farDistance = val;

  char buffer[16] ;
  sprintf( buffer, "%.3f", cam->farDistance.getValue());
  gtk_entry_set_text( GTK_ENTRY(viewer->farclippingedit), buffer );
} // farclippingwheelMoved()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::nearclipEditPressed(
  GtkWidget	*w,
  gpointer	closure )
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;

  SoCamera * cam = viewer->getCamera();
  if (!cam) return;

  char *s =
    gtk_editable_get_chars(GTK_EDITABLE(viewer->nearclippingedit),0,-1);
  float val;
  if (sscanf(s, "%f", &val) == 1) {
    // FIXME: sanity check on val? 990223 mortene.
    cam->nearDistance = val;
    gtk_thumbwheel_set_value( GTK_THUMBWHEEL(viewer->nearclippingwheel), val );
  }
  g_free(s);

  /* else */
  {
    char buffer[16] ;
    sprintf( buffer, "%.3f", cam->nearDistance.getValue() );
    gtk_entry_set_text( GTK_ENTRY(viewer->nearclippingedit), buffer );
  }
} // nearclipEditPressed()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::farclipEditPressed(
  GtkWidget	*w,
  gpointer	closure )
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;

  SoCamera * cam = viewer->getCamera();
  if (!cam) return;

  char *s = gtk_editable_get_chars(GTK_EDITABLE(viewer->farclippingedit),0,-1 );
  float val;
  if (sscanf(s, "%f", &val) == 1) {
    // FIXME: sanity check on val? 990223 mortene.
    cam->farDistance = val;
    gtk_thumbwheel_set_value( GTK_THUMBWHEEL(viewer->farclippingwheel), val);
  }
  g_free(s);

  /* else */
  {
    char buffer[16] ;
    sprintf( buffer, "%.3f", cam->farDistance.getValue() );
    gtk_entry_set_text( GTK_ENTRY(viewer->farclippingedit), buffer );
  }
} // farclipEditPressed()

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewer::stereoToggled(
  GtkToggleButton *toggle_button,
  gpointer         closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  bool flag = gtk_toggle_button_get_active (toggle_button);

  SOGTK_STUB();

  // FIXME: this is a dummy dialog
  SoGtk::createSimpleErrorDialog(GTK_WIDGET(toggle_button), 
    "Stereo Error Dialog", 
    _( "Stereo Viewing can't be set on this machine." ) );
} // stereoToggled()     

// *************************************************************************

/*!
  FIXME: write doc
*/

SbBool
SoGtkFullViewer::processSoEvent(
  const SoEvent * const event )
{
  if ( common->processSoEvent(event) ) return TRUE;
  return inherited::processSoEvent(event);
} // processSoEvent()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::interactbuttonClicked(
  void )
{
  if ( ! this->isViewing() ) {
    GtkWidget * button = this->interactbutton ;
    gtk_signal_handler_block_by_data( GTK_OBJECT(button), (gpointer) this );
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(button), TRUE );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(button), (gpointer) this );
    return;
  }
  this->setViewing( FALSE );
} // interactbuttonClicked()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::interactbuttonClickedCB( // static
  GtkWidget *w,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;

  if (viewer->isViewing())
    viewer->setViewing(FALSE);
} // interactbuttonClickedCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::viewbuttonClicked(
  void )
{

  if ( this->isViewing() ) {
    GtkWidget * button = this->viewbutton ;
    gtk_signal_handler_block_by_data( GTK_OBJECT(button), (gpointer) this );
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(button), TRUE );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(button), (gpointer) this );
    return;
  }
  this->setViewing( TRUE );
} // viewbuttonClicked()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::viewbuttonClickedCB( // static
  GtkWidget * w,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  if ( !viewer->isViewing()) 
    viewer->setViewing(TRUE);
} // viewbuttonClickedCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::helpbuttonClicked(
  void )
{
  this->openViewerHelpCard();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::helpbuttonClickedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->helpbuttonClicked();
} // helpbuttonClickedCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::homebuttonClicked(
  void )
{
  this->resetToHomePosition();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::homebuttonClickedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->homebuttonClicked();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::sethomebuttonClicked(
  void )
{
  this->saveHomePosition();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::sethomebuttonClickedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->sethomebuttonClicked();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::viewallbuttonClicked(
  void )
{
  this->viewAll();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::viewallbuttonClickedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->viewallbuttonClicked();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::seekbuttonClicked(
  void )
{
  this->setSeekMode( this->isSeekMode() ? FALSE : TRUE );
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::seekbuttonClickedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->seekbuttonClicked();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::leftwheelPressedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelStart();
} // leftwheelPressedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::leftwheelMovedCB( // static
  GtkWidget * wheel,
  gpointer closure )
{
  assert( closure != NULL );
  gfloat value = gtk_thumbwheel_get_value( GTK_THUMBWHEEL(wheel) );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelMotion( value );
} // leftWheelMovedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::leftwheelReleasedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelFinish();
} // leftwheelReleasedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::bottomwheelPressedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelStart();
} // bottomwheelPressedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::bottomwheelMovedCB( // static
  GtkWidget * wheel,
  gpointer closure )
{
  assert( closure != NULL );
  gfloat value = gtk_thumbwheel_get_value( GTK_THUMBWHEEL(wheel) );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelMotion( value );
} // bottomWheelMovedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::bottomwheelReleasedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelFinish();
} // bottomwheelReleasedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::rightwheelPressedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelStart();
} // rightwheelPressedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::rightwheelMovedCB( // static
  GtkWidget * wheel,
  gpointer closure )
{
  assert( closure != NULL );
  gfloat value = gtk_thumbwheel_get_value( GTK_THUMBWHEEL(wheel) );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelMotion( value );
} // rightWheelMovedCB()

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::rightwheelReleasedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelFinish();
} // rightwheelReleasedCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

SbBool
SoGtkFullViewer::addPointer(
  const char * name,
  void * data )
{
  return TRUE;
} // addPointer()

/*!
  FIXME: write doc
*/

SbBool
SoGtkFullViewer::setCurrentPointer(
  const char * name )
{
  return TRUE;
} // setCurrentPointer()

/*!
  FIXME: write doc
*/

const char *
SoGtkFullViewer::getCurrentPointer(
  void ) const
{
  return (const char *) NULL;
} // getCurrentPointer()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkFullViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

