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
  \ingroup gtkviewers

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

struct SoGtkViewerButton {
  char * keyword;
  char * label;
  GtkSignalFunc pressed;
  char ** xpm_data;
  GtkWidget * bwidget;
  GtkWidget * lwidget;
//  GtkType type;
};

struct SoGtkViewerButton
SoGtkFullViewer::SoGtkFullViewerButtons[] = {
  { // interact button
    "interact", "I",
    (GtkSignalFunc) SoGtkFullViewer::interactbuttonClickedCB,
    pick_xpm,
    NULL, NULL
  },
  { // view
    "view", "E",
    (GtkSignalFunc) SoGtkFullViewer::viewbuttonClickedCB,
    view_xpm,
    NULL, NULL
  },
  { // help
    "help", "?",
    (GtkSignalFunc) SoGtkFullViewer::helpbuttonClickedCB,
    help_xpm,
    NULL, NULL
  },
  { // home
    "home", "h",
    (GtkSignalFunc) SoGtkFullViewer::homebuttonClickedCB,
    home_xpm,
    NULL, NULL
  },
  { // set home
    "set_home", "H",
    (GtkSignalFunc) SoGtkFullViewer::sethomebuttonClickedCB,
    set_home_xpm,
    NULL, NULL
  }, 
  { // view all
    "view_all", "V",
    (GtkSignalFunc) SoGtkFullViewer::viewallbuttonClickedCB,
    view_all_xpm,
    NULL, NULL
  },
  { // seek
    "seek", "S",
    (GtkSignalFunc) SoGtkFullViewer::seekbuttonClickedCB,
    seek_xpm,
    NULL, NULL
  }
}; // SoGtkFullViewerButtons[]

// *************************************************************************

/*!
  Constructor. See parent class for explanation of arguments.

  Subclasses will probably usually call with the \c buildNow flag
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
  this->buttons = new SoGtkViewerButton [ buttons ];
  memcpy( this->buttons, SoGtkFullViewerButtons, sizeof(SoGtkFullViewerButtons) );

  this->viewerWidget = NULL;
  this->canvas = NULL;
  this->canvasParent = NULL;

  this->leftWheel = NULL;
  this->leftWheelLabel = NULL;
  this->leftWheelStr = NULL;
  this->leftWheelVal = 0.0f;
  this->bottomWheel = NULL;
  this->bottomWheelLabel = NULL;
  this->bottomWheelStr = NULL;
  this->bottomWheelVal = 0.0f;
  this->rightWheel = NULL;
  this->rightWheelLabel = NULL;
  this->rightWheelStr = NULL;
  this->rightWheelVal = 0.0f;

  this->setLeftWheelString( "Motion Y" );
  this->setBottomWheelString( "Motion X" );
  this->setRightWheelString( "Motion Z" );

  this->zoomrange = SbVec2f( 1.0f, 140.0f );

  this->mainLayout = NULL;
  this->appButtonLayout = NULL;

  this->prefmenu = NULL;
  this->prefwindow = NULL;
  this->prefwindowtitle = "Viewer Preference Sheet";

  this->menuEnabled = buildFlag & SoGtkFullViewer::BUILD_POPUP;
  this->decorations = buildFlag & SoGtkFullViewer::BUILD_DECORATION;

  this->menuTitle = "Viewer Menu";

  this->viewerButtons = new SbPList;
  this->appButtonList = new SbPList;
  this->appButtonForm = NULL;

  this->setClassName( "SoGtkFullViewer" );

  if ( ! build ) return;
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * viewer = this->buildWidget( parent );
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
  delete [] this->buttons;
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
  if ( this->prefmenu )
    this->prefmenu->SetMenuItemMarked( DECORATION_ITEM, enable );
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

  if ( this->prefmenu )
    this->prefmenu->SetMenuItemMarked( EXAMINING_ITEM, enable );

  inherited::setViewing( enable );

  GtkWidget * interact_button = findButton( "interact" );
  GtkWidget * view_button = findButton( "view" );
  if ( interact_button && view_button ) {
    gtk_signal_handler_block_by_data( GTK_OBJECT(interact_button), (gpointer) this );
    gtk_signal_handler_block_by_data( GTK_OBJECT(view_button), (gpointer) this );
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(interact_button), enable ? FALSE : TRUE );
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(view_button), enable ? TRUE : FALSE );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(interact_button), (gpointer) this );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(view_button), (gpointer) this );
  }

//  GtkWidget * seek_button = findButton( "seek" );
//  VIEWERBUTTON(SEEK_BUTTON)->setEnabled(enable);
} // setViewing()

// *************************************************************************

/*!
  Overloaded from parent to update user interface indicator for headlight
  on or off in the popup menu.
*/

void
SoGtkFullViewer::setHeadlight(
  SbBool enable )
{
  inherited::setHeadlight( enable );
  if ( this->prefmenu )
    this->prefmenu->SetMenuItemMarked( HEADLIGHT_ITEM, enable );
} // setHeadlight()

// *************************************************************************

/*!
  Overloaded from parent to make sure the user interface indicator in
  the popup menu is updated correctly.
*/

void
SoGtkFullViewer::setDrawStyle(
  SoGtkViewer::DrawType type,
  SoGtkViewer::DrawStyle style )
{
  inherited::setDrawStyle( type, style );
  if ( this->prefmenu )
    common->setDrawStyleMenuActivation( type, style );
} // setDrawStyle()

// *************************************************************************

/*!
  Overloaded from parent to make sure the user interface indicators in
  the popup menu are updated correctly.
*/

void
SoGtkFullViewer::setBufferingType(
  SoGtkViewer::BufferType type )
{
  inherited::setBufferingType( type );

  if ( this->prefmenu ) {
    //
  }
}

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

    const SbBool enable = camera ? TRUE : FALSE;
//    this->zoomslider->setEnabled(enable);
//    this->zoomfield->setEnabled(enable);
//    this->zoomrangefrom->setEnabled(enable);
//    this->zoomrangeto->setEnabled(enable);
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
  g_return_val_if_fail( root != NULL, NULL );
  GtkWidget * croot = gtk_hbox_new( FALSE, 0 ); 
  g_return_val_if_fail( croot != NULL, NULL );

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
  g_return_val_if_fail( trim != NULL, NULL );
  gtk_widget_set_usize( GTK_WIDGET(trim), 30, 0 );
  // set background color

  this->leftWheel = gtk_thumbwheel_new( 1 );
  g_return_val_if_fail( this->leftWheel != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(this->leftWheel), 2, 2 );

  gtk_signal_connect( GTK_OBJECT(this->leftWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->leftWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->leftWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelReleasedCB), (gpointer) this );

  gtk_box_pack_end( GTK_BOX(trim), GTK_WIDGET(this->leftWheel), FALSE, FALSE, TRUE );
  gtk_widget_show( GTK_WIDGET(this->leftWheel) );

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
  g_return_val_if_fail( trim != NULL, NULL );
  gtk_widget_set_usize( trim, 0, 30 );

  this->leftWheelLabel = gtk_label_new( this->leftWheelStr );
  g_return_val_if_fail( this->leftWheelLabel != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(this->leftWheelLabel), 3, 0 );

  this->bottomWheelLabel = gtk_label_new( this->bottomWheelStr );
  g_return_val_if_fail( this->bottomWheelLabel != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(this->bottomWheelLabel), 3, 0 );

  this->rightWheelLabel = gtk_label_new( this->rightWheelStr );
  g_return_val_if_fail( this->rightWheelLabel != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(this->rightWheelLabel), 3, 0 );

  this->bottomWheel = gtk_thumbwheel_new( 0 );
  g_return_val_if_fail( this->bottomWheel != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(this->bottomWheel), 2, 2 );

  gtk_signal_connect( GTK_OBJECT(this->bottomWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::bottomwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->bottomWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::bottomwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->bottomWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::bottomwheelReleasedCB), (gpointer) this );

  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(this->leftWheelLabel), FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(this->bottomWheelLabel), FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(this->bottomWheel), FALSE, TRUE, TRUE );
  gtk_box_pack_end( GTK_BOX(trim), GTK_WIDGET(this->rightWheelLabel), FALSE, TRUE, FALSE );
  gtk_widget_show( GTK_WIDGET(this->leftWheelLabel) );
  gtk_widget_show( GTK_WIDGET(this->bottomWheelLabel) );
  gtk_widget_show( GTK_WIDGET(this->bottomWheel) );
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
  g_return_val_if_fail( trim != NULL, NULL );
  gtk_widget_set_usize( trim, 30, 0 );

  GtkWidget * buttons = this->buildViewerButtons( trim );
  g_return_val_if_fail( buttons != NULL, NULL );
  gtk_widget_set_usize( buttons, 30, 0 );
  gtk_widget_show( buttons );

  this->rightWheel = gtk_thumbwheel_new( 1 );
  g_return_val_if_fail( this->rightWheel != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(this->rightWheel), 2, 2 );

  gtk_signal_connect( GTK_OBJECT(this->rightWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::rightwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->rightWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::rightwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(this->rightWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::rightwheelReleasedCB), (gpointer) this );

  gtk_box_pack_start( GTK_BOX(trim), GTK_WIDGET(buttons), FALSE, TRUE, TRUE );
  gtk_box_pack_end( GTK_BOX(trim), GTK_WIDGET(this->rightWheel), FALSE, TRUE, TRUE );

  gtk_widget_show( GTK_WIDGET(this->rightWheel) );

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

  const int buttons = sizeof(SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);
  for ( int button = 0; button < buttons; button++ ) {
    GtkWidget * widget = GTK_WIDGET(button < 2 ? gtk_toggle_button_new() : gtk_button_new());
    if ( button == 0 && ! this->isViewing() )
      gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(widget), TRUE );
    if ( button == 1 && this->isViewing() )
      gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(widget), TRUE );

    gtk_tooltips_set_tip (tooltips, widget,
      this->buttons[button].keyword, NULL);

    GdkPixmap * gdk_pixmap = 
      gdk_pixmap_colormap_create_from_xpm_d (NULL, colormap, 
        &mask, NULL,
        this->buttons[button].xpm_data);
    GtkWidget * label = gtk_pixmap_new( gdk_pixmap, mask );

    gdk_pixmap_unref (gdk_pixmap);
    gdk_bitmap_unref (mask);

    gtk_widget_show( label );
    this->buttons[button].lwidget = label;

    gtk_container_add( GTK_CONTAINER(widget), GTK_WIDGET(label) );
    if ( this->buttons[button].pressed != NULL ) {
      gtk_signal_connect( GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(this->buttons[button].pressed),
        (gpointer) this );
      if ( button < 2 )
        gtk_signal_connect( GTK_OBJECT(widget), "released",
          GTK_SIGNAL_FUNC(this->buttons[button].pressed),
          (gpointer) this );
    }
    this->buttons[button].bwidget = widget;
    buttonlist->append( widget );
  }
} // createViewerButtons()

/*!
*/

GtkWidget *
SoGtkFullViewer::findButton( // private
  const char * const keyword )
{
  const int buttons = sizeof(SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);
  for ( int i = 0; i < buttons; i++ )
    if ( strcmp( keyword, this->buttons[i].keyword ) == 0 )
      return this->buttons[i].bwidget;
  return (GtkWidget *) NULL;
}

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
  if ( this->isViewing() )
    this->prefmenu->SetMenuItemMarked( EXAMINING_ITEM, TRUE );
  if ( this->isDecoration() )
    this->prefmenu->SetMenuItemMarked( DECORATION_ITEM, TRUE );
  if ( this->isHeadlight() )
    this->prefmenu->SetMenuItemMarked( HEADLIGHT_ITEM, TRUE );
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
  if ( this->prefmenu )
    this->prefmenu->PopUp( this->getGLWidget(), position[0], position[1] );
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
  return NULL;
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
//  if (this->prefwindow)
//    this->prefwindow->setCaption(this->prefwindowtitle.getString());
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
  if ( this->leftWheelStr ) {
    delete [] this->leftWheelStr;
    this->leftWheelStr = NULL;
  }
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
  if ( this->bottomWheelStr ) {
    delete [] this->bottomWheelStr;
    this->bottomWheelStr = NULL;
  }
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
  if ( this->rightWheelStr ) {
    delete [] this->rightWheelStr;
    this->rightWheelStr = NULL;
  }
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

#if 0
  w = makeAutoclipPreferences(form);
  
//  w = makeStereoPreferences(form);

//  w = makeSpinAnimationPreferences(form);

  w = makeSubPreferences(form);
#endif

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
  SoGtkFullViewer* viewer = (SoGtkFullViewer*) closure ;
  viewer->prefwindow = NULL ;
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
  GSList *rbg1 = NULL;

  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget *hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (form), hbox2, TRUE, TRUE, 0);

  GtkWidget *label6 = gtk_label_new ( "Seek animation time:" );
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

  GtkWidget *label7 = gtk_label_new ( "seconds" );
  gtk_widget_show (label7);
  gtk_box_pack_start (GTK_BOX (hbox2), label7, FALSE, TRUE, 0);

  GtkWidget *hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (form), hbox3, TRUE, TRUE, 0);

  GtkWidget *label8 = gtk_label_new ( "Seek to:" );
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox3), label8, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label8), 4, 0);

  GtkWidget *rb1 = gtk_radio_button_new_with_label (rbg1, "point" );
  rbg1 = gtk_radio_button_group (GTK_RADIO_BUTTON (rb1));
  gtk_widget_show (rb1);
  gtk_box_pack_start (GTK_BOX (hbox3), rb1, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rb1), 
    this->isDetailSeek());

  GtkWidget *rb2 = gtk_radio_button_new_with_label (rbg1, "object" );
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
  GSList *bg = NULL;

  GtkWidget* form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget* hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (form), hbox4, TRUE, TRUE, 0);

  GtkWidget *label4 = gtk_label_new ( "Seek distance:" );
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

  GtkWidget *r1 = gtk_radio_button_new_with_label (bg, "percentage" );
  bg = gtk_radio_button_group (GTK_RADIO_BUTTON (r1));
  gtk_widget_show (r1);
  gtk_box_pack_start (GTK_BOX (hbox5), r1, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r1), 
    this->isSeekValuePercentage() );

  GtkWidget *r2 = gtk_radio_button_new_with_label (bg, "absolute" );
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

  GtkWidget *label9 = gtk_label_new ( "Camera Zoom:" );
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

  GtkWidget *label10 = gtk_label_new ( "Zoom slider ranges from:" );
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

  GtkWidget *label11 = gtk_label_new ( "to:" );
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox2), label11, FALSE, FALSE, 0);

  this->zoomrangeto = gtk_entry_new ();
  sprintf( buffer, "%.1f", this->zoomrange[1] );
  gtk_entry_set_text( GTK_ENTRY(this->zoomrangeto), buffer );
  gtk_widget_set_usize(this->zoomrangeto,64,20);
  gtk_widget_show (this->zoomrangeto);
  gtk_box_pack_start (GTK_BOX (hbox2), this->zoomrangeto, FALSE, FALSE, 0);

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
  return NULL;
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
  f = SoGtkMax(0.0f, SoGtkMin(f, 1.0f)) * ZOOMSLIDERRESOLUTION;

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
  Qt slot.
*/

/*
void
SoGtkFullViewer::selectedViewing()
{
  this->setViewing(this->isViewing() ? FALSE : TRUE);
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::selectedDecoration()
{
  this->setDecoration(this->isDecoration() ? FALSE : TRUE);
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::selectedHeadlight()
{
  this->setHeadlight(this->isHeadlight() ? FALSE : TRUE);
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::copyviewSelected()
{
  this->copyView(SbTime::getTimeOfDay());
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::pasteviewSelected()
{
  this->pasteView(SbTime::getTimeOfDay());
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::drawstyleActivated(
  int id )
{
  // Take care of any buffer menu selection first.
  switch (id) {
  case SINGLE_BUFFER_ITEM:
    this->setBufferingType(SoGtkViewer::BUFFER_SINGLE);
    return;
  case DOUBLE_BUFFER_ITEM:
    this->setBufferingType(SoGtkViewer::BUFFER_DOUBLE);
    return;
  case INTERACTIVE_BUFFER_ITEM:
    this->setBufferingType(SoGtkViewer::BUFFER_INTERACTIVE);
    return;
  };


  SoGtkViewer::DrawStyle val;

  // FIXME: use a dict or two? 990220 mortene.
  switch (id) {
  case AS_IS_ITEM: val = SoGtkViewer::VIEW_AS_IS; break;
  case HIDDEN_LINE_ITEM: val = SoGtkViewer::VIEW_HIDDEN_LINE; break;
  case NO_TEXTURE_ITEM: val = SoGtkViewer::VIEW_NO_TEXTURE; break;
  case LOW_RESOLUTION_ITEM: val = SoGtkViewer::VIEW_LOW_COMPLEXITY; break;
  case WIREFRAME_ITEM: val = SoGtkViewer::VIEW_LINE; break;
  case POINTS_ITEM: val = SoGtkViewer::VIEW_POINT; break;
  case BOUNDING_BOX_ITEM: val = SoGtkViewer::VIEW_BBOX; break;

  case MOVE_SAME_AS_STILL_ITEM: val = SoGtkViewer::VIEW_SAME_AS_STILL; break;
  case MOVE_NO_TEXTURE_ITEM: val = SoGtkViewer::VIEW_NO_TEXTURE; break;
  case MOVE_LOW_RES_ITEM: val = SoGtkViewer::VIEW_LOW_COMPLEXITY; break;
  case MOVE_WIREFRAME_ITEM: val = SoGtkViewer::VIEW_LINE; break;
  case MOVE_LOW_RES_WIREFRAME_ITEM: val = SoGtkViewer::VIEW_LOW_RES_LINE; break;
  case MOVE_POINTS_ITEM: val = SoGtkViewer::VIEW_POINT; break;
  case MOVE_LOW_RES_POINTS_ITEM: val = SoGtkViewer::VIEW_LOW_RES_POINT; break;
  case MOVE_BOUNDING_BOX_ITEM: val = SoGtkViewer::VIEW_BBOX; break;

  default: assert(0); break;
  }

  SoGtkViewer::DrawType type;

  switch (id) {
  case AS_IS_ITEM:
  case HIDDEN_LINE_ITEM:
  case NO_TEXTURE_ITEM:
  case LOW_RESOLUTION_ITEM:
  case WIREFRAME_ITEM:
  case POINTS_ITEM:
  case BOUNDING_BOX_ITEM:
    type = SoGtkViewer::STILL;
    break;

  case MOVE_SAME_AS_STILL_ITEM:
  case MOVE_NO_TEXTURE_ITEM:
  case MOVE_LOW_RES_ITEM:
  case MOVE_WIREFRAME_ITEM:
  case MOVE_LOW_RES_WIREFRAME_ITEM:
  case MOVE_POINTS_ITEM:
  case MOVE_LOW_RES_POINTS_ITEM:
  case MOVE_BOUNDING_BOX_ITEM:
    type = SoGtkViewer::INTERACTIVE;
    break;

  default: assert(0); break;
  }

  this->setDrawStyle(type, val);
} // drawstyleActivated()
*/

// *************************************************************************

/*!
  \internal
  Slot.
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
    val = SoGtkMax(0.001f, SoGtkMin(179.999f, val));
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
  Qt slot.
*/

/*
void
SoGtkFullViewer::clippingToggled(
  bool flag )
{
  this->setAutoClipping(flag);
  this->setEnabledClippingWidgets(!flag);
} // clippingToggled()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

void
SoGtkFullViewer::increaseInteractiveCount(
  void )
{
  this->interactiveCountInc();
} // increaseInteractiveCount()

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

void
SoGtkFullViewer::decreaseInteractiveCount(
  void )
{
  this->interactiveCountDec();
} // decreaseInteractiveCount()

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::nearclippingwheelMoved(
  float val )
{
  SoCamera * cam = this->getCamera();
  if (!cam) return;

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->nearDistance = val;

  QString s;
  s.setNum(cam->nearDistance.getValue(), 'f', 3);
  this->nearclippingedit->setText(s);
} // nearclippingwheelMoved()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::farclippingwheelMoved(
  float val )
{
  SoCamera * cam = this->getCamera();
  if (!cam) return;

  // FIXME: cut off at 0.0? And against near clipping value? 990223 mortene.
  cam->farDistance = val;

  QString s;
  s.setNum(cam->farDistance.getValue(), 'f', 3);
  this->farclippingedit->setText(s);
} // farclippingwheelMoved()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::nearclipEditPressed(
  void )
{
  SoCamera * cam = this->getCamera();
  if (!cam) return;

  float val;
  if (sscanf(this->nearclippingedit->text(), "%f", &val) == 1) {
    // FIXME: sanity check on val? 990223 mortene.
    cam->nearDistance = val;
    this->nearclippingwheel->setValue(val);
  }
  else {
    QString s;
    s.setNum(cam->nearDistance.getValue(), 'f', 3);
    this->nearclippingedit->setText(s);
  }
} // nearclipEditPressed()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::farclipEditPressed(
  void )
{
  SoCamera * cam = this->getCamera();
  if (!cam) return;

  float val;
  if (sscanf(this->farclippingedit->text(), "%f", &val) == 1) {
    // FIXME: sanity check on val? 990223 mortene.
    cam->farDistance = val;
    this->farclippingwheel->setValue(val);
  }
  else {
    QString s;
    s.setNum(cam->farDistance.getValue(), 'f', 3);
    this->farclippingedit->setText(s);
  }
} // farclipEditPressed()
*/

// *************************************************************************

void
SoGtkFullViewer::drawstyleActivated(
  int )
{
}

// *************************************************************************

SbBool
SoGtkFullViewer::processSoEvent(
  const SoEvent * const event )
{
  if ( common->processSoEvent(event) ) return TRUE;
  return inherited::processSoEvent(event);
} // processSoEvent()

// *************************************************************************

/*!
*/

void
SoGtkFullViewer::interactbuttonClicked(
  void )
{
  if ( ! this->isViewing() ) {
    GtkWidget * button = findButton( "interact" );
    gtk_signal_handler_block_by_data( GTK_OBJECT(button), (gpointer) this );
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(button), TRUE );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(button), (gpointer) this );
    return;
  }
  this->setViewing( FALSE );
} // interactbuttonClicked()

/*!
*/

void
SoGtkFullViewer::interactbuttonClickedCB( // static
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->interactbuttonClicked();
} // interactbuttonClickedCB()

// *************************************************************************

/*!
*/

void
SoGtkFullViewer::viewbuttonClicked(
  void )
{
  if ( this->isViewing() ) {
    GtkWidget * button = findButton( "view" );
    gtk_signal_handler_block_by_data( GTK_OBJECT(button), (gpointer) this );
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(button), TRUE );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(button), (gpointer) this );
    return;
  }
  this->setViewing( TRUE );
} // viewbuttonClicked()

/*!
*/

void
SoGtkFullViewer::viewbuttonClickedCB( // static
  GtkWidget * w,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->viewbuttonClicked();
} // viewbuttonClickedCB()

// *************************************************************************

void
SoGtkFullViewer::helpbuttonClicked(
  void )
{
  this->openViewerHelpCard();
}

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

void
SoGtkFullViewer::homebuttonClicked(
  void )
{
  this->resetToHomePosition();
}

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

void
SoGtkFullViewer::sethomebuttonClicked(
  void )
{
  this->saveHomePosition();
}

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

void
SoGtkFullViewer::viewallbuttonClicked(
  void )
{
  this->viewAll();
}

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

void
SoGtkFullViewer::seekbuttonClicked(
  void )
{
  this->setSeekMode( this->isSeekMode() ? FALSE : TRUE );
}

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
*/

SbBool
SoGtkFullViewer::addPointer(
  const char * name,
  void * data )
{
  return TRUE;
} // addPointer()

/*!
*/

SbBool
SoGtkFullViewer::setCurrentPointer(
  const char * name )
{
  return TRUE;
} // setCurrentPointer()

/*!
*/

const char *
SoGtkFullViewer::getCurrentPointer(
  void ) const
{
  return NULL;
} // getCurrentPointer()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkFullViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

