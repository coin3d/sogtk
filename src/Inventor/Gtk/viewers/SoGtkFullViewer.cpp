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

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#include <assert.h>

#include <gtk/gtk.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkbutton.h>

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

// *************************************************************************

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

  this->zoomRange = SbVec2f( 1.0f, 140.0f );

  this->mainLayout = NULL;
  this->appButtonLayout = NULL;

  this->prefmenu = NULL;
  this->prefWindow = NULL;
  this->prefWindowTitle = "Viewer Preference Sheet";

  this->menuEnabled = buildFlag & SoGtkFullViewer::BUILD_POPUP;
  this->decorations = buildFlag & SoGtkFullViewer::BUILD_DECORATION;

  this->menuTitle = "Viewer Menu";

  this->viewerButtons = new SbPList;
  this->appButtonList = new SbPList;
  this->appButtonForm = NULL;

  this->setClassName( "SoGtkFullViewer" );

  if ( build ) {
    GtkWidget * viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    // this->setSize( SbVec2s( 500, 390 ) );
  }

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

  inherited::setViewing( enable );

//  if (this->prefmenu)
//    this->prefMenu->setItemChecked(EXAMINING_ITEM, enable);
//  VIEWERBUTTON(EXAMINE_BUTTON)->setOn(enable);
//  VIEWERBUTTON(INTERACT_BUTTON)->setOn(enable ? FALSE : TRUE);
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
//  if ( this->prefMenu )
//    this->prefMenu->setItemChecked( HEADLIGHT_ITEM, enable );
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

  if (this->prefWindow) {
    this->setZoomSliderPosition(this->getCameraZoom());
    this->setZoomFieldString(this->getCameraZoom());

    const SbBool enable = camera ? TRUE : FALSE;
//    this->zoomSlider->setEnabled(enable);
//    this->zoomField->setEnabled(enable);
//    this->zoomRangeFrom->setEnabled(enable);
//    this->zoomRangeTo->setEnabled(enable);
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
//  if ( this->prefWindow )
//    this->prefWindow->hide();
} // hide()

// *************************************************************************

/*!
  \internal

  Catch close events on the preferences window (to convert to hide
  events) and right mouse button presses (to pop up the
  preferences menu).
*/

SbBool
SoGtkFullViewer::eventFilter(
  GtkObject * obj,
  GdkEvent * event )
{
  return FALSE;
}

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

struct SoGtkViewerButton {
  char * keyword;
  char * label;
  GtkSignalFunc clicked;
  char ** xpm_data;
  GtkWidget * bwidget;
  GtkWidget * lwidget;
};

struct SoGtkViewerButton
SoGtkFullViewer::SoGtkFullViewerButtons[] = {
  { // interact button
    "interact",
    "I",
    (GtkSignalFunc) SoGtkFullViewer::interactbuttonClickedCB,
    pick_xpm,
    NULL, NULL
  },
  { // view
    "view",
    "E",
    (GtkSignalFunc) SoGtkFullViewer::viewbuttonClickedCB,
    view_xpm,
    NULL, NULL
  },
  { // help
    "help",
    "?",
    (GtkSignalFunc) SoGtkFullViewer::helpbuttonClickedCB,
    help_xpm,
    NULL, NULL
  },
  { // home
    "home",
    "h",
    (GtkSignalFunc) SoGtkFullViewer::homebuttonClickedCB,
    home_xpm,
    NULL, NULL
  },
  { // set home
    "set_home",
    "H",
    (GtkSignalFunc) SoGtkFullViewer::sethomebuttonClickedCB,
    set_home_xpm,
    NULL, NULL
  }, 
  { // view all
    "view_all",
    "V",
    (GtkSignalFunc) SoGtkFullViewer::viewallbuttonClickedCB,
    view_all_xpm,
    NULL, NULL
  },
  { // seek
    "seek",
    "S",
    (GtkSignalFunc) SoGtkFullViewer::seekbuttonClickedCB,
    seek_xpm,
    NULL, NULL
  }
}; // SoGtkFullViewerButtons[]

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
  int button = 0;
  const int buttons = sizeof(SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);
  while ( button < buttons ) {
    GtkWidget * widget = gtk_button_new();
    SoGtkFullViewerButtons[button].bwidget = widget;

    GtkWidget * label = gtk_label_new( SoGtkFullViewerButtons[button].label );
    gtk_widget_show( label );
    SoGtkFullViewerButtons[button].lwidget = label;

    gtk_container_add( GTK_CONTAINER(widget), GTK_WIDGET(label) );
    if ( SoGtkFullViewerButtons[button].clicked != NULL )
      gtk_signal_connect( GTK_OBJECT(widget), "clicked",
        GTK_SIGNAL_FUNC(SoGtkFullViewerButtons[button].clicked),
        (gpointer) this );
    buttonlist->append( widget );
    button++;
  } // while ( button < buttons )
} // createViewerButtons()

/*!
*/

GtkWidget *
SoGtkFullViewer::findButton( // private
  const char * const keyword )
{
  const int buttons = sizeof(SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);
  for ( int i = 0; i < buttons; i++ )
    if ( strcmp( keyword, SoGtkFullViewerButtons[i].keyword ) == 0 )
      return SoGtkFullViewerButtons[i].bwidget;
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
//  this->prefmenu = common->buildStandardPopupMenu();
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
  SOGTK_STUB();
}

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
  this->prefWindowTitle = title ? title : "";
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
  the Help button. Default implementation does nothing.
*/

void
SoGtkFullViewer::openViewerHelpCard(void)
{
  SOGTK_STUB();
}

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
  return NULL;
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
  return NULL;
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
  return NULL;
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
  return NULL;
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
  Update the Qt slider representing the camera zoom.
*/
void
SoGtkFullViewer::setZoomSliderPosition(float zoom)
{
  if (!this->prefWindow) return;

  float f =
    (zoom - this->zoomRange[0]) / (this->zoomRange[1] - this->zoomRange[0]);
  f = SbMax(0.0f, SbMin(f, 1.0f)) * ZOOMSLIDERRESOLUTION;

//  this->zoomSlider->setValue(f);
}

// *************************************************************************
/*!
  Set string in the camera zoom edit field.
*/
void
SoGtkFullViewer::setZoomFieldString(float zoom)
{
  if (!this->prefWindow) return;

/*
  QString s;
  s.setNum(zoom, 'f', 1);
  this->zoomField->setText(s);
*/
}

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::interactbuttonToggled(bool flag)
{
  this->setViewing(!flag);
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::viewbuttonToggled(bool flag)
{
  this->setViewing(flag);
}
*/

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
  Qt slot.
*/

/*
void
SoGtkFullViewer::selectedPrefs(void)
{
  if (!this->prefwindow) this->prefwindow = this->makePreferencesWindow();
  this->prefwindow->show();
  this->prefwindow->raise();
} // selectedPrefs()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekAnimationTimeChanged(
  const char * s )
{
  float val;
  if ((sscanf(s, "%f", &val) == 1) && (val >= 0.0f)) this->setSeekTime(val);
} // seekAnimationTimeChanged()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekAnimationTimeChanged(
  const QString & s )
{
  bool ok;
  float val = s.toFloat(&ok);
  if (ok && (val >= 0.0f)) this->setSeekTime(val);
} // seekAnimationTimeChanged()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekDetailToggled(
  int id )
{
  if (id == 0) this->setDetailSeek(TRUE);
  else if (id == 1) this->setDetailSeek(FALSE);
  else assert(0);
} // seekDetailToggle()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekDistanceWheelChanged(
  float val )
{
  if (val < 0.1f) {
    val = 0.1f;
    this->seekdistancewheel->setValue( val );
  }

  this->setSeekDistance(val * val);

  QString s;
  s.setNum(this->getSeekDistance(), 'f', 2);
  this->seekdistancefield->setText(s);
} // seekDistanceWheelChanged()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekDistanceEdit(
  void )
{
  float val;
  if ((sscanf(this->seekdistancefield->text(), "%f", &val) == 1) &&
      (val > 0.0f)) {
    this->setSeekDistance(val);
    this->seekdistancewheel->setValue(sqrt(val));
  }
  else {
    QString s;
    s.setNum(this->getSeekDistance(), 'f', 2);
    this->seekdistancefield->setText(s);
  }
} // seekDistanceEdit()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekDistanceTypeToggle(
  int id )
{
  this->setSeekValueAsPercentage(id == 0 ? TRUE : FALSE);
} // seekDistanceTypeToggle()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::zoomSliderMoved(
  int val )
{
  float f = val / float(ZOOMSLIDERRESOLUTION);
  f = this->zoomrange[0] + f * (this->zoomrange[1] - this->zoomrange[0]);

  this->setCameraZoom(f);
  this->setZoomFieldString(f);
} // zoomSliderMoved()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::zoomFieldChanged(
  void )
{
  float val;
  if (sscanf(this->zoomfield->text(), "%f", &val) == 1) {
    val = QMIN(0.001f, QMAX(179.999f, val));
    this->setCameraZoom(val);
    this->setZoomSliderPosition(val);
  }
  else {
    QString s;
    s.setNum(this->getCameraZoom(), 'f', 1);
    this->zoomfield->setText(s);
  }
} // zoomFieldChanged()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::zoomRangeChanged1(
  void )
{
  float val;
  if (sscanf(this->zoomrangefrom->text(), "%f", &val) == 1) {
    if (val > 0.0f && val < 180.0f && val < this->zoomrange[1]) {
      this->zoomrange[0] = val;
      this->setZoomSliderPosition(this->getCameraZoom());
    }
  }
  else {
    QString s;
    s.setNum(this->zoomrange[0], 'f', 1);
    this->zoomrangefrom->setText(s);
  }
} // zoomRangeChanged1()
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::zoomRangeChanged2(
  void )
{
  float val;
  if (sscanf(this->zoomrangeto->text(), "%f", &val) == 1) {
    if (val > 0.0f && val < 180.0f && val > this->zoomrange[0]) {
      this->zoomrange[1] = val;
      this->setZoomSliderPosition(this->getCameraZoom());
    }
  }
  else {
    QString s;
    s.setNum(this->zoomrange[1], 'f', 1);
    this->zoomrangeto->setText(s);
  }
} // zoomRangeChanged2()
*/

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
SoGtkFullViewer::selectedPrefs(
  void )
{
}

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
  if ( this->isViewing() )
    this->setViewing( FALSE );
  GtkWidget * interact_button = findButton( "interact" );
  GtkWidget * view_button = findButton( "view" );
}

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

void
SoGtkFullViewer::viewbuttonClicked(
  void )
{
  if ( ! this->isViewing() )
    this->setViewing( TRUE );
  GtkWidget * interact_button = findButton( "interact" );
  GtkWidget * view_button = findButton( "view" );
}

void
SoGtkFullViewer::viewbuttonClickedCB( // static
  GtkWidget *,
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

#if SOGTK_DEBUG
static const char * getSoGtkFullViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

