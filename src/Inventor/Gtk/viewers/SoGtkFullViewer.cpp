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

static const char rcsid[] =
  "$Id$";

#include <assert.h>

#include <gtk/gtk.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtklabel.h>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>
#include <Inventor/Gtk/widgets/SoAnyPopupMenu.h>

#include <Inventor/Gtk/viewers/SoAnyFullViewer.h>
#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>

// Button icons.
#include "icons/pick.xpm"
#include "icons/view.xpm"
#include "icons/help.xpm"
#include "icons/home.xpm"
#include "icons/set_home.xpm"
#include "icons/view_all.xpm"
#include "icons/seek.xpm"

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
  SbBool buildInsideParent,
  SoGtkFullViewer::BuildFlag buildFlag,
  SoGtkViewer::Type type,
  SbBool buildNow )
: inherited( parent, name, buildInsideParent, type, FALSE )
, common( new SoAnyFullViewer( this ) )
{
  this->viewerWidget = NULL;
  this->canvas = NULL;
  this->canvasParent = NULL;

  char axisindicator[] = { 'Y', 'X', 'Z' };
  for (int i = FIRSTDECORATION; i <= LASTDECORATION; i++) {
//    this->wheelStrings[i] = "Motion ";
//    this->wheelStrings[i] += axisindicator[i - FIRSTDECORATION];
    this->decorForm[i] = NULL;
    this->wheelLabels[i] = NULL;
  }

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

//  this->setSize( SbVec2s( 500, 390 ) );

  if ( buildNow )
    this->setBaseWidget( this->buildWidget( this->getParentWidget() ) );

  // why isn't this working?
//  this->setSize( SbVec2s( 500, 390 ) );

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
} // ~SoGtkFullViewer()

// *************************************************************************

/*!
  Turn the viewer decorations on or off.

  \sa isDecoration()
*/

void
SoGtkFullViewer::setDecoration(
  const SbBool on )
{
#if SOGTK_DEBUG
  if ((on && this->isDecoration()) || (!on && !this->isDecoration())) {
    SoDebugError::postWarning("SoGtkFullViewer::setDecoration",
                              "decorations already turned %s",
                              on ? "on" : "off");
    return;
  }
#endif // SOGTK_DEBUG

  this->decorations = on;
  if ( this->prefmenu )
    this->prefmenu->SetMenuItemMarked( DECORATION_ITEM, on );
  if ( this->viewerWidget )
    this->showDecorationWidgets(on);
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
  SbBool on )
{
  if ((on && this->isViewing()) || (!on && !this->isViewing())) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkFullViewer::setViewing",
                              "view mode already %s", on ? "on" : "off");
#endif // SOGTK_DEBUG
    return;
  }

  inherited::setViewing(on);
//  if (this->prefmenu)
//    this->prefMenu->setItemChecked(EXAMINING_ITEM, on);
//  VIEWERBUTTON(EXAMINE_BUTTON)->setOn(on);
//  VIEWERBUTTON(INTERACT_BUTTON)->setOn(on ? FALSE : TRUE);
//  VIEWERBUTTON(SEEK_BUTTON)->setEnabled(on);
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
  inherited::setBufferingType(type);

  if (this->prefmenu) {
/*
    QMenuData * m;
    this->prefMenu->findItem(AS_IS_ITEM, &m);
    assert(m);

    m->setItemChecked(SINGLE_BUFFER_ITEM,
                      type == SoGtkViewer::BUFFER_SINGLE ? TRUE : FALSE);
    m->setItemChecked(DOUBLE_BUFFER_ITEM,
                      type == SoGtkViewer::BUFFER_DOUBLE ? TRUE : FALSE);
    m->setItemChecked(INTERACTIVE_BUFFER_ITEM,
                      type == SoGtkViewer::BUFFER_INTERACTIVE ? TRUE : FALSE);
*/
  }
}

// *************************************************************************

/*!
  Overloaded from parent to make sure the user interface indicators on
  the camera features in the preferences sheet are updated correctly.
*/

void
SoGtkFullViewer::setCamera(SoCamera * newCamera)
{
  inherited::setCamera(newCamera);

  if (this->prefWindow) {
    this->setZoomSliderPosition(this->getCameraZoom());
    this->setZoomFieldString(this->getCameraZoom());

    SbBool on = newCamera ? TRUE : FALSE;
//    this->zoomSlider->setEnabled(on);
//    this->zoomField->setEnabled(on);
//    this->zoomRangeFrom->setEnabled(on);
//    this->zoomRangeTo->setEnabled(on);
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
/*
#if 0 // debug
  switch (e->type()) {
  case Event_MouseButtonPress:
//      SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "button press");
    break;
  case Event_MouseButtonRelease:
//      SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "button release");
    break;
  case Event_MouseButtonDblClick:
//      SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "dbl click");
    break;
  case Event_MouseMove:
//      SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "mousemove");
    break;
  case Event_Paint:
    SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "paint");
    break;
  case Event_Resize:
    SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "resize");
    break;
  case Event_FocusIn:
  case Event_FocusOut:
  case Event_Enter:
  case Event_Leave:
  case Event_Move:
  case Event_LayoutHint:
  case Event_ChildInserted:
  case Event_ChildRemoved:
    // ignored
    break;
  default:
    SoDebugError::postInfo("SoGtkFullViewer::eventFilter", "type %d", e->type());
    break;
  }
#endif // debug

  inherited::eventFilter( obj, e );

  // Convert dblclick events to press events to get the "correct"
  // sequence of two press+release pairs under Qt 1.xx and Qt
  // 2.xx. (It has been confirmed with the Trolls that this Qt
  // behavior is actually a feature, not a bug.)
#if QT_VERSION < 200
  int eventtype = e->type();
#else // Qt 2.0
  GdkEvent::Type eventtype = e->type();
#endif // Qt 2.0
  eventtype = (eventtype == Event_MouseButtonDblClick ?
               Event_MouseButtonPress : eventtype);

  // Catch close events to avoid anything actually being destroyed.
  if (obj == this->prefwindow) {
    if (eventtype == Event_Close) {
      ((QCloseEvent *)e)->ignore();
      this->prefwindow->hide();
      return TRUE;
    }
  }

  // Show the popup menu when we detect rmb pressed on top of the
  // render area canvas.
  if (this->menuenabled && obj == this->getRenderAreaWidget() &&
      eventtype == Event_MouseButtonPress) {
    QMouseEvent * me = (QMouseEvent *)e;
    if (me->button() == RightButton) {
      if (!this->prefmenu) this->buildPopupMenu();
#if (QT_VERSION > 140)
      this->prefmenu->popup(me->globalPos());
#else
      this->prefmenu->popup(me->pos());
#endif
    }
  }

*/
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
  GtkWidget * croot = gtk_hbox_new( FALSE, 0 ); 
  g_return_val_if_fail( root != NULL, NULL );
  g_return_val_if_fail( croot != NULL, NULL );

  this->canvas = inherited::buildWidget( croot );


  this->viewerWidget = croot;
  this->canvasParent = parent;

  if ( this->decorations ) {
    this->buildDecoration( parent );
//    this->showDecorationWidgets( TRUE );
  }

  gtk_box_pack_start(
    GTK_BOX(croot), this->decorForm[LEFTDECORATION], FALSE, TRUE, FALSE );
  gtk_box_pack_start(
    GTK_BOX(croot), this->canvas, TRUE, TRUE, FALSE );
  gtk_box_pack_start(
    GTK_BOX(croot), this->decorForm[RIGHTDECORATION], FALSE, TRUE, FALSE );
  gtk_box_pack_start(
    GTK_BOX(root), GTK_WIDGET(croot), TRUE, TRUE, FALSE );
  gtk_box_pack_start(
    GTK_BOX(root), GTK_WIDGET(this->decorForm[BOTTOMDECORATION]), FALSE, TRUE, FALSE );
  gtk_container_add( GTK_CONTAINER(parent), GTK_WIDGET(root) );

  gtk_widget_show( this->decorForm[ LEFTDECORATION ] );
  gtk_widget_show( this->canvas );
  gtk_widget_show( this->decorForm[ RIGHTDECORATION ] );
  gtk_widget_show( croot );

  gtk_widget_show( this->decorForm[ BOTTOMDECORATION ] );

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
  this->decorForm[ LEFTDECORATION ]    = this->buildLeftTrim( parent );
  this->decorForm[ BOTTOMDECORATION ]  = this->buildBottomTrim( parent );
  this->decorForm[ RIGHTDECORATION ]   = this->buildRightTrim( parent );
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
  gtk_widget_set_usize( trim, 30, 0 );
  // set background color

  GtkWidget * wheel = gtk_thumbwheel_new( 1 );
  g_return_val_if_fail( wheel != NULL, NULL );

  gtk_box_pack_end( GTK_BOX(trim), wheel, FALSE, FALSE, TRUE );
  gtk_widget_show( wheel );

  return trim;
/*
  QGridLayout * gl = new QGridLayout(w, 3, 1, 2, -1 ); // , 0, -1);
  gl->addWidget(this->buildAppButtons(w), 0, 0);

  QtThumbwheel * t = this->wheels[LEFTDECORATION] =
    new QtThumbwheel(QtThumbwheel::Vertical, w);
//  t->adjustSize();
//  QSize rect = t->size();
//  rect.setWidth( 30 );
  t->setFixedSize( QSize( 24, 88 ) );
  QObject::connect(t, SIGNAL(wheelMoved(float)),
                   this, SLOT(leftWheelChanged(float)));
  QObject::connect(t, SIGNAL(wheelPressed()),
                   this, SLOT(leftWheelPressed()));
  QObject::connect(t, SIGNAL(wheelReleased()),
                   this, SLOT(leftWheelReleased()));

  this->wheelvalues[LEFTDECORATION] = t->value();

  gl->addWidget(t, 2, 0, AlignBottom|AlignHCenter);
  gl->activate();
*/
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

  GtkWidget * l1, * l2, * l3;
  l1 = gtk_label_new( "Roty" );
  g_return_val_if_fail( l1 != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(l1), 3, 0 );
  l2 = gtk_label_new( "Rotx" );
  g_return_val_if_fail( l2 != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(l2), 3, 0 );
  l3 = gtk_label_new( "Dolly" );
  g_return_val_if_fail( l3 != NULL, NULL );
  gtk_misc_set_padding( GTK_MISC(l3), 3, 0 );

  GtkWidget * wheel = gtk_thumbwheel_new( 0 );
  g_return_val_if_fail( wheel != NULL, NULL );

  gtk_box_pack_start( GTK_BOX(trim), l1, FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(trim), l2, FALSE, TRUE, FALSE );
  gtk_box_pack_start( GTK_BOX(trim), wheel, FALSE, TRUE, TRUE );
  gtk_box_pack_end( GTK_BOX(trim), l3, FALSE, TRUE, FALSE );
  gtk_widget_show( l1 );
  gtk_widget_show( l2 );
  gtk_widget_show( wheel );
  gtk_widget_show( l3 );

  return trim;
/*
  GtkWidget * w = new GtkWidget(parent);
  w->setFixedHeight( 30 );

  int alignments[] = { AlignLeft|AlignTop, AlignRight|AlignVCenter,
                       AlignRight|AlignTop, };
  for (int i = FIRSTDECORATION; i <= LASTDECORATION; i++) {
    this->wheellabels[i] = new QLabel(this->wheelstrings[i], w);
    this->wheellabels[i]->adjustSize();
    this->wheellabels[i]->setAlignment(alignments[i - FIRSTDECORATION]);
  }

  QtThumbwheel * t = this->wheels[BOTTOMDECORATION] =
    new QtThumbwheel(QtThumbwheel::Horizontal, w);
  t->setFixedSize( QSize( 88, 24 ) );
//  t->adjustSize();
//  t->setFixedSize(t->size());
  QObject::connect(t, SIGNAL(wheelMoved(float)),
                   this, SLOT(bottomWheelChanged(float)));
  QObject::connect(t, SIGNAL(wheelPressed()),
                   this, SLOT(bottomWheelPressed()));
  QObject::connect(t, SIGNAL(wheelReleased()),
                   this, SLOT(bottomWheelReleased()));

  this->wheelvalues[BOTTOMDECORATION] = t->value();

  QGridLayout * gl = new QGridLayout(w, 1, 5);
  gl->addWidget(this->wheellabels[RIGHTDECORATION], 0, 4, AlignRight);
  gl->addWidget(this->wheellabels[LEFTDECORATION], 0, 0, AlignLeft);
  gl->addWidget(this->wheellabels[BOTTOMDECORATION], 0, 1);
  gl->addWidget(t, 0, 2);
  gl->activate();
*/
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
  // set background color

  GtkWidget * wheel = gtk_thumbwheel_new( 1 );
  g_return_val_if_fail( wheel != NULL, NULL );

  gtk_box_pack_end( GTK_BOX(trim), wheel, FALSE, TRUE, TRUE );
  gtk_widget_show( wheel );

  return trim;
/*
  t->setRangeBoundaryHandling( QtThumbwheel::ACCUMULATE );

  QObject::connect(t, SIGNAL(wheelMoved(float)),
                   this, SLOT(rightWheelChanged(float)));
  QObject::connect(t, SIGNAL(wheelPressed()),
                   this, SLOT(rightWheelPressed()));
  QObject::connect(t, SIGNAL(wheelReleased()),
                   this, SLOT(rightWheelReleased()));

  this->wheelvalues[RIGHTDECORATION] = t->value();
*/
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
SoGtkFullViewer::buildViewerButtons(GtkWidget * parent)
{
/*
  GtkWidget * w = new GtkWidget(parent);
  this->createViewerButtons(w, this->viewerbuttons);

  assert(this->viewerbuttons->getLength() != 0);
  QGridLayout * l =
    new QGridLayout(w, this->viewerbuttons->getLength(), 1);

  for (int i=0; i < this->viewerbuttons->getLength(); i++) {
    QButton * b = VIEWERBUTTON(i);
    b->setFixedSize( QSize( 26, 26 ) ); // b->size());
    if (i>0) { assert(VIEWERBUTTON(i)->width()==VIEWERBUTTON(i-1)->width()); }
    l->addWidget(b, i, 0);
  }

  l->activate();
  return w;
*/
  return NULL;
}

// *************************************************************************

/*!
  Set up the viewer buttons with pixmaps and event connections.
*/

void
SoGtkFullViewer::createViewerButtons(GtkWidget * parent, SbPList * buttonlist)
{
/*
  for (int i=0; i <= SEEK_BUTTON; i++) {
    QPushButton * p = new QPushButton(parent);

    switch (i) {
    case INTERACT_BUTTON:
      p->setToggleButton(TRUE);
      p->setPixmap(QPixmap((const char **)pick_xpm));
      p->setOn(this->isViewing() ? FALSE : TRUE);
      QObject::connect(p, SIGNAL(toggled(bool)),
                       this, SLOT(interactbuttonToggled(bool)));
      break;
    case EXAMINE_BUTTON:
      p->setToggleButton(TRUE);
      p->setPixmap(QPixmap((const char **)view_xpm));
      p->setOn(this->isViewing());
      QObject::connect(p, SIGNAL(toggled(bool)),
                       this, SLOT(viewbuttonToggled(bool)));
      break;
    case HELP_BUTTON:
      QObject::connect(p, SIGNAL(clicked()), this, SLOT(helpbuttonClicked()));
      p->setPixmap(QPixmap((const char **)help_xpm));
      break;
    case HOME_BUTTON:
      QObject::connect(p, SIGNAL(clicked()), this, SLOT(homebuttonClicked()));
      p->setPixmap(QPixmap((const char **)home_xpm));
      break;
    case SET_HOME_BUTTON:
      QObject::connect(p, SIGNAL(clicked()),
                       this, SLOT(sethomebuttonClicked()));
      p->setPixmap(QPixmap((const char **)set_home_xpm));
      break;
    case VIEW_ALL_BUTTON:
      QObject::connect(p, SIGNAL(clicked()),
                       this, SLOT(viewallbuttonClicked()));
      p->setPixmap(QPixmap((const char **)view_all_xpm));
      break;
    case SEEK_BUTTON:
      QObject::connect(p, SIGNAL(clicked()), this, SLOT(seekbuttonClicked()));
      p->setPixmap(QPixmap((const char **)seek_xpm));
      break;
    default:
      assert(0);
      break;
    }

    p->adjustSize();
    buttonlist->append(p);
  }
*/
}

// *************************************************************************
/*!
  Make a popup menu with preferences settings.
*/
void
SoGtkFullViewer::buildPopupMenu(
  void )
{
/*
  this->prefmenu = common->buildStandardPopupMenu();
*/
}

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
  Build the sub-popupmenu with miscellaneous functions.
*/

GtkWidget *
SoGtkFullViewer::buildFunctionsSubmenu(
  GtkWidget * popup )
{
/*
  QPopupMenu * m = new QPopupMenu(popup);

  m->insertItem("Help", this, SLOT(helpbuttonClicked()), 0, HELP_ITEM);
  m->insertItem("Home", this, SLOT(homebuttonClicked()), 0, HOME_ITEM);
  m->insertItem("Set Home", this, SLOT(sethomebuttonClicked()), 0,
                SET_HOME_ITEM);
  m->insertItem("View All", this, SLOT(viewallbuttonClicked()), 0,
                VIEW_ALL_ITEM);
  m->insertItem("Seek", this, SLOT(seekbuttonClicked()), 0, SEEK_ITEM);

  m->insertSeparator();

  m->insertItem("Copy View", this, SLOT(copyviewSelected()), 0,
                COPY_VIEW_ITEM);
  m->insertItem("Paste View", this, SLOT(pasteviewSelected()), 0,
                PASTE_VIEW_ITEM);

  return m;
*/
  return NULL;
}

// *************************************************************************

/*!
  Build the sub-popupmenu with the drawstyle settings.
*/

GtkWidget *
SoGtkFullViewer::buildDrawStyleSubmenu(
  GtkWidget * popup )
{
/*
  QPopupMenu * m = new QPopupMenu(popup);

  m->insertItem("as is", AS_IS_ITEM);
  m->insertItem("hidden line", HIDDEN_LINE_ITEM);
  m->insertItem("no texture", NO_TEXTURE_ITEM);
  m->insertItem("low resolution", LOW_RESOLUTION_ITEM);
  m->insertItem("wireframe", WIREFRAME_ITEM);
  m->insertItem("points", POINTS_ITEM);
  m->insertItem("bounding box (no depth)", BOUNDING_BOX_ITEM);
  m->insertSeparator();

  m->insertItem("move same as still", MOVE_SAME_AS_STILL_ITEM);
  m->insertItem("move no texture", MOVE_NO_TEXTURE_ITEM);
  m->insertItem("move low res", MOVE_LOW_RES_ITEM);
  m->insertItem("move wireframe", MOVE_WIREFRAME_ITEM);
  m->insertItem("move low res wireframe (no depth)",
                MOVE_LOW_RES_WIREFRAME_ITEM);
  m->insertItem("move points", MOVE_POINTS_ITEM);
  m->insertItem("move low res points (no depth)", MOVE_LOW_RES_POINTS_ITEM);
  m->insertItem("move bounding box (no depth)", MOVE_BOUNDING_BOX_ITEM);
  m->insertSeparator();

  m->insertItem("single buffer", SINGLE_BUFFER_ITEM);
  m->insertItem("double buffer", DOUBLE_BUFFER_ITEM);
  m->insertItem("interactive buffer", INTERACTIVE_BUFFER_ITEM);

  QObject::connect(m, SIGNAL(activated(int)), SLOT(drawstyleActivated(int)));

  return m;
*/
  return NULL;
}

// *************************************************************************

/*!
  Overload this method in subclass viewers to append more widgets to
  the bottom of the preferences sheet window.
*/

GtkWidget *
SoGtkFullViewer::makeSubPreferences(
  GtkWidget * /*parent*/ )
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
SoGtkFullViewer::leftWheelStart(
  void )
{
  this->interactiveCountInc();
}

// *************************************************************************

/*!
  Called repeatedly as the user drags the thumbwheel in the left frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa leftWheelStart(), leftWheelFinish()
  \sa bottomWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::leftWheelMotion(
  float value )
{
  this->wheelValues[LEFTDECORATION] = value;
}

// *************************************************************************

/*!
  Called as the user let go of the thumbwheel in the left frame after
  a drag operation. Overload this method in subclassed viewers to provide
  your own functionality on the thumbwheel.

  \sa leftWheelStart(), leftWheelMotion()
  \sa bottomWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::leftWheelFinish(
  void )
{
  this->interactiveCountDec();
}

// *************************************************************************

/*!
  Get current value of the left thumbwheel.

  \sa leftWheelMotion()
*/

float
SoGtkFullViewer::getLeftWheelValue(
  void ) const
{
  return this->wheelValues[LEFTDECORATION];
}

// *************************************************************************

/*!
  Called when the user start to drag the thumbwheel in the bottom frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa bottomWheelMotion(), bottomWheelFinish()
  \sa leftWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::bottomWheelStart(
  void )
{
  this->interactiveCountInc();
}

// *************************************************************************

/*!
  Called repeatedly as the user drags the thumbwheel in the bottom frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa bottomWheelStart(), bottomWheelFinish()
  \sa leftWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::bottomWheelMotion(
  float value )
{
  this->wheelValues[BOTTOMDECORATION] = value;
}

// *************************************************************************

/*!
  Called as the user let go of the thumbwheel in the bottom frame after
  a drag operation. Overload this method in subclassed viewers to provide
  your own functionality on the thumbwheel.

  \sa bottomWheelStart(), bottomWheelMotion()
  \sa leftWheelStart(), rightWheelStart()
*/

void
SoGtkFullViewer::bottomWheelFinish(
  void )
{
  this->interactiveCountDec();
}

// *************************************************************************

/*!
  Get current value of the bottom thumbwheel.

  \sa bottomWheelMotion()
*/

float
SoGtkFullViewer::getBottomWheelValue(
  void ) const
{
  return this->wheelValues[ BOTTOMDECORATION ];
}

// *************************************************************************

/*!
  Called when the user start to drag the thumbwheel in the right frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa rightWheelMotion(), rightWheelFinish()
  \sa leftWheelStart(), bottomWheelStart()
*/

void
SoGtkFullViewer::rightWheelStart(
  void )
{
  this->interactiveCountInc();
}

// *************************************************************************

/*!
  Called repeatedly as the user drags the thumbwheel in the right frame.
  Overload this method in subclassed viewers to provide your own
  functionality on the thumbwheel.

  \sa rightWheelStart(), rightWheelFinish()
  \sa leftWheelStart(), bottomWheelStart()
*/

void
SoGtkFullViewer::rightWheelMotion(
  float value )
{
  this->wheelValues[ RIGHTDECORATION ] = value;
}

// *************************************************************************

/*!
  Called as the user let go of the thumbwheel in the right frame after
  a drag operation. Overload this method in subclassed viewers to provide
  your own functionality on the thumbwheel.

  \sa rightWheelStart(), rightWheelMotion()
  \sa leftWheelStart(), bottomWheelStart()
*/

void
SoGtkFullViewer::rightWheelFinish(
  void )
{
  this->interactiveCountDec();
}

// *************************************************************************

/*!
  Get current value of the right thumbwheel.

  \sa rightWheelMotion()
*/

float
SoGtkFullViewer::getRightWheelValue(
  void ) const
{
  return this->wheelValues[ RIGHTDECORATION ];
}

// *************************************************************************
// These are all private slots for catching Qt events.

void SoGtkFullViewer::leftWheelPressed(void) { this->leftWheelStart(); }
void SoGtkFullViewer::leftWheelChanged(float v) { this->leftWheelMotion(v); }
void SoGtkFullViewer::leftWheelReleased(void) { this->leftWheelFinish(); }
void SoGtkFullViewer::bottomWheelPressed(void) { this->bottomWheelStart(); }
void SoGtkFullViewer::bottomWheelChanged(float v) { this->bottomWheelMotion(v);}
void SoGtkFullViewer::bottomWheelReleased(void) { this->bottomWheelFinish(); }
void SoGtkFullViewer::rightWheelPressed(void) { this->rightWheelStart(); }
void SoGtkFullViewer::rightWheelChanged(float v) { this->rightWheelMotion(v); }
void SoGtkFullViewer::rightWheelReleased(void) { this->rightWheelFinish(); }

// *************************************************************************

/*!
  Set label of the left thumbwheel.
*/

void
SoGtkFullViewer::setLeftWheelString(
  const char * str )
{
/*
  this->wheelStrings[LEFTDECORATION] = str ? str : "";
  QLabel * l = this->wheellabels[LEFTDECORATION];
  if (l) l->setText(this->wheelstrings[LEFTDECORATION]);
*/
}

// *************************************************************************

/*!
  Set label of the bottom thumbwheel.
*/

void
SoGtkFullViewer::setBottomWheelString(const char * str)
{
/*
  this->wheelstrings[BOTTOMDECORATION] = str ? str : "";
  QLabel * l = this->wheellabels[BOTTOMDECORATION];
  if (l) l->setText(this->wheelstrings[BOTTOMDECORATION]);
*/
}

// *************************************************************************

/*!
  Set label of the right thumbwheel.
*/

void
SoGtkFullViewer::setRightWheelString(const char * str)
{
/*
  this->wheelstrings[RIGHTDECORATION] = str ? str : "";
  QLabel * l = this->wheellabels[RIGHTDECORATION];
  if (l) l->setText(this->wheelstrings[RIGHTDECORATION]);
*/
}

// *************************************************************************

/*!
  Overload this method to provide functionality when the user clicks
  the Help button. Default implementation does nothing.
*/

void
SoGtkFullViewer::openViewerHelpCard(void)
{
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
/*
  delete this->appButtonLayout;
  this->appButtonLayout = NULL;

  int nrbuttons = this->appbuttonlist->getLength();
  if (nrbuttons == 0) return;

  this->appbuttonlayout = new QGridLayout(form, nrbuttons, 1);

  for (int i=0; i < nrbuttons; i++) {
    GtkWidget * button = (GtkWidget *)((*(this->appbuttonlist))[i]);
    button->setFixedSize(button->size());
    button->show();
    ((QGridLayout *)this->appbuttonlayout)->addWidget(button, i, 0);
  }

  this->appbuttonlayout->activate();
*/
}

// *************************************************************************

/*!
  Create preferences sheet.
*/

GtkWidget *
SoGtkFullViewer::makePreferencesWindow(
  void )
{
/*
  GtkWidget * top = new GtkWidget(NULL);
  top->setCaption(this->prefwindowtitle.getString());
  top->setIconText(this->prefwindowtitle.getString());

  QVBoxLayout * layout = new QVBoxLayout(top, 10);

  GtkWidget * w;
  w = this->makeSeekPreferences(top);
  if (w) layout->addWidget(w, w->height());
  w = this->makeSeekDistancePreferences(top);
  if (w) layout->addWidget(w, w->height());
  w = this->makeZoomPreferences(top);
  if (w) layout->addWidget(w, w->height());
  w = this->makeAutoclipPreferences(top);
  if (w) layout->addWidget(w, w->height());

  w = this->makeSubPreferences(top);
  if (w) layout->addWidget(w, w->height());

  layout->activate();
  top->adjustSize();
  return top;
*/
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
/*
  // FIXME: it'd be nice to show separation in the prefs sheet, but
  // this code doesn't work for some reason (because of the
  // layout objects?). 990221 mortene.
#if 0
  QFrame * w = new QFrame(parent);
  w->setFrameStyle(QFrame::Box | QFrame::Plain);
  w->setLineWidth(1);
#else
  GtkWidget * w = new GtkWidget(parent);
#endif

  // Initialize objects keeping track of geometry data.

  QSize tmpsize(0, 0);
  QSize totalsize(0, 0);

  QVBoxLayout * toplayout = new QVBoxLayout(w);

  // First, do the three widgets on the uppermost row (label,
  // lineedit, label).

  QLabel * l1 = new QLabel("Seek animation time:", w);
  l1->adjustSize();
  expandSize(tmpsize, l1->size(), LayoutHorizontal);

  QLineEdit * le = new QLineEdit(w);
#if QT_VERSION < 200 // Qt 2.xx
  QObject::connect(le, SIGNAL(textChanged(const char *)),
                   this, SLOT(seekAnimationTimeChanged(const char *)));
#else // Qt 1.xx
  QObject::connect(le, SIGNAL(textChanged(const QString &)),
                   this, SLOT(seekAnimationTimeChanged(const QString &)));
#endif // Qt 1.xx
  QString s;
  s.setNum(this->getSeekTime(), 'f', 2);
  le->setText(s);
  le->adjustSize();
  expandSize(tmpsize, le->size(), LayoutHorizontal);

  QLabel * l2 = new QLabel("seconds", w);
  l2->adjustSize();
  expandSize(tmpsize, l2->size(), LayoutHorizontal);

  // Layout row 1.
  QBoxLayout * layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(l1, l1->width());
  layout->addWidget(le, le->width());
  layout->addWidget(l2, l2->width());
  expandSize(totalsize, tmpsize, LayoutVertical);


  // Do the three widgets on the second row (label, radiobutton,
  // radiobutton).

  tmpsize = QSize(0, 0);

  QLabel * l3 = new QLabel("Seek to:", w);
  l3->adjustSize();
  expandSize(tmpsize, l3->size(), LayoutHorizontal);

  QButtonGroup * bg = new QButtonGroup;
  QRadioButton * r1 = new QRadioButton("point", w);
  r1->setChecked(this->isDetailSeek());
  r1->adjustSize();
  expandSize(tmpsize, r1->size(), LayoutHorizontal);
  bg->insert(r1);

  QRadioButton * r2 = new QRadioButton("object", w);
  r2->setChecked(!this->isDetailSeek());
  r2->adjustSize();
  expandSize(tmpsize, r2->size(), LayoutHorizontal);
  bg->insert(r2);

  bg->setExclusive(TRUE);
  QObject::connect(bg, SIGNAL(clicked(int)),
                   this, SLOT(seekDetailToggled(int)));

  // Layout row 2.
  layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(l3, l3->width());
  layout->addWidget(r1, r1->width());
  layout->addWidget(r2, r2->width());
  expandSize(totalsize, tmpsize, LayoutVertical);

  w->resize(totalsize);
  toplayout->activate();
  return w;
*/
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
/*
  GtkWidget * w = new GtkWidget(parent);

  // Initialize objects keeping track of geometry data.

  QSize tmpsize(0, 0);
  QSize totalsize(0, 0);

  QVBoxLayout * toplayout = new QVBoxLayout(w);

  // First, do the three widgets on the uppermost row (label,
  // thumbwheel, lineedit).

  QLabel * l = new QLabel("Seek distance:", w);
  l->adjustSize();
  expandSize(tmpsize, l->size(), LayoutHorizontal);

  this->seekdistancewheel = new QtThumbwheel(QtThumbwheel::Horizontal, w);
  this->seekdistancewheel->setRangeBoundaryHandling( QtThumbwheel::ACCUMULATE );
  this->seekdistancewheel->setValue(sqrt(this->getSeekDistance()));
  this->seekdistancewheel->adjustSize();
  expandSize(tmpsize, this->seekdistancewheel->size(),
                             LayoutHorizontal);
  QObject::connect(this->seekdistancewheel, SIGNAL(wheelMoved(float)),
                   this, SLOT(seekDistanceWheelChanged(float)));

  this->seekdistancefield = new QLineEdit(w);
  QString s;
  s.setNum(this->getSeekDistance(), 'f', 2);
  this->seekdistancefield->setText(s);
  this->seekdistancefield->adjustSize();
  expandSize(tmpsize, this->seekdistancefield->size(),
                             LayoutHorizontal);
  QObject::connect(this->seekdistancefield, SIGNAL(returnPressed()),
                   this, SLOT(seekDistanceEdit()));

  // Layout row 1.
  QBoxLayout * layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(l, l->width());
  layout->addWidget(this->seekdistancewheel, this->seekdistancewheel->width());
  layout->addWidget(this->seekdistancefield, this->seekdistancefield->width());
  expandSize(totalsize, tmpsize, LayoutVertical);


  // Do the two widgets on the second row (radiobutton, radiobutton).

  tmpsize = QSize(0, 0);

  QButtonGroup * bg = new QButtonGroup;
  QRadioButton * r1 = new QRadioButton("percentage", w);
  r1->setChecked(this->isSeekValuePercentage());
  r1->adjustSize();
  expandSize(tmpsize, r1->size(), LayoutHorizontal);
  bg->insert(r1);

  QRadioButton * r2 = new QRadioButton("absolute", w);
  r2->setChecked(!this->isSeekValuePercentage());
  r2->adjustSize();
  expandSize(tmpsize, r2->size(), LayoutHorizontal);
  bg->insert(r2);

  bg->setExclusive(TRUE);
  QObject::connect(bg, SIGNAL(clicked(int)),
                   this, SLOT(seekDistanceTypeToggle(int)));

  // Layout row 2.
  layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(r1, r1->width());
  layout->addWidget(r2, r2->width());
  expandSize(totalsize, tmpsize, LayoutVertical);

  w->resize(totalsize);
  toplayout->activate();

  return w;
*/
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
/*
  GtkWidget * w = new GtkWidget(parent);

  // Initialize objects keeping track of geometry data.

  QSize tmpsize(0, 0);
  QSize totalsize(0, 0);

  QVBoxLayout * toplayout = new QVBoxLayout(w);

  // First, do the three widgets on the uppermost row (label,
  // slider, lineedit).

  QLabel * l1 = new QLabel("Camera zoom", w);
  l1->adjustSize();
  expandSize(tmpsize, l1->size(), LayoutHorizontal);

  this->zoomslider = new QSlider(0, ZOOMSLIDERRESOLUTION,
                                 1, 0, QSlider::Horizontal, w);
  this->zoomslider->adjustSize();
  expandSize(tmpsize, this->zoomslider->size(), LayoutHorizontal);
  QObject::connect(this->zoomslider, SIGNAL(sliderPressed()),
                   this, SLOT(increaseInteractiveCount()));
  QObject::connect(this->zoomslider, SIGNAL(valueChanged(int)),
                   this, SLOT(zoomSliderMoved(int)));
  QObject::connect(this->zoomslider, SIGNAL(sliderReleased()),
                   this, SLOT(decreaseInteractiveCount()));

  this->zoomfield = new QLineEdit(w);
  this->zoomfield->adjustSize();
  expandSize(tmpsize, this->zoomfield->size(), LayoutHorizontal);
  QObject::connect(this->zoomfield, SIGNAL(returnPressed()),
                   this, SLOT(zoomFieldChanged()));


  // Layout row 1.
  QBoxLayout * layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(l1, l1->width());
  layout->addWidget(this->zoomslider, this->zoomslider->width());
  layout->addWidget(this->zoomfield, this->zoomfield->width());
  expandSize(totalsize, tmpsize, LayoutVertical);


  // Do the four widgets on the second row (label, lineedit, label,
  // lineedit).

  tmpsize = QSize(0, 0);

  QLabel * l2 = new QLabel("Zoom slider ranges from:", w);
  l2->adjustSize();
  expandSize(tmpsize, l2->size(), LayoutHorizontal);

  this->zoomrangefrom = new QLineEdit(w);
  QString s;
  s.setNum(this->zoomrange[0], 'f', 1);
  this->zoomrangefrom->setText(s);
  this->zoomrangefrom->adjustSize();
  expandSize(tmpsize, this->zoomrangefrom->size(), LayoutHorizontal);
  QObject::connect(this->zoomrangefrom, SIGNAL(returnPressed()),
                   this, SLOT(zoomRangeChanged1()));

  QLabel * l3 = new QLabel("to:", w);
  l3->adjustSize();
  expandSize(tmpsize, l3->size(), LayoutHorizontal);

  this->zoomrangeto = new QLineEdit(w);
  s.setNum(this->zoomrange[1], 'f', 1);
  this->zoomrangeto->setText(s);
  this->zoomrangeto->adjustSize();
  expandSize(tmpsize, this->zoomrangeto->size(), LayoutHorizontal);
  QObject::connect(this->zoomrangeto, SIGNAL(returnPressed()),
                   this, SLOT(zoomRangeChanged2()));


  // Layout row 2.
  layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(l2, l2->width());
  layout->addWidget(this->zoomrangefrom, this->zoomrangefrom->width());
  layout->addWidget(l3, l3->width());
  layout->addWidget(this->zoomrangeto, this->zoomrangeto->width());
  expandSize(totalsize, tmpsize, LayoutVertical);


  SoCamera * cam = this->getCamera();
  if (cam) {
    this->setZoomSliderPosition(this->getCameraZoom());
    this->setZoomFieldString(this->getCameraZoom());
  }
  else {
    this->zoomslider->setEnabled(FALSE);
    this->zoomfield->setEnabled(FALSE);
    this->zoomrangefrom->setEnabled(FALSE);
    this->zoomrangeto->setEnabled(FALSE);
  }

  w->resize(totalsize);
  toplayout->activate();

  return w;
*/
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
/*
  this->nearclippinglabel->setEnabled(flag);
  this->nearclippingwheel->setEnabled(flag);
  this->nearclippingedit->setEnabled(flag);
  this->farclippinglabel->setEnabled(flag);
  this->farclippingwheel->setEnabled(flag);
  this->farclippingedit->setEnabled(flag);

  SoCamera * cam = this->getCamera();
  if (!cam) return;

  this->nearclippingwheel->setValue(cam->nearDistance.getValue());
  this->farclippingwheel->setValue(cam->farDistance.getValue());

  QString s;
  s.setNum(cam->nearDistance.getValue(), 'f', 3);
  this->nearclippingedit->setText(s);
  s.setNum(cam->farDistance.getValue(), 'f', 3);
  this->farclippingedit->setText(s);
*/
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
/*
  GtkWidget * w = new GtkWidget(dialog);

  // Initialize objects keeping track of geometry data.

  QSize totalsize(0, 0);

  QVBoxLayout * toplayout = new QVBoxLayout(w);

  // First, do the checkbutton widget.

  QCheckBox * c = new QCheckBox("Auto clipping planes", w);
  c->adjustSize();
  c->setChecked(this->isAutoClipping());
  QObject::connect(c, SIGNAL(toggled(bool)),
                   this, SLOT(clippingToggled(bool)));

  // Layout row 1.
  toplayout->addWidget(c, c->height());
  expandSize(totalsize, c->size(), LayoutVertical);

  // Do the three widgets on the second row (label, thumbwheel,
  // lineedit).

  QSize tmpsize = QSize(0, 0);

  this->nearclippinglabel = new QLabel("near plane", w);
  this->nearclippinglabel->adjustSize();
  expandSize(tmpsize, this->nearclippinglabel->size(), LayoutHorizontal);

  this->nearclippingwheel = new QtThumbwheel(QtThumbwheel::Horizontal, w);
  this->nearclippingwheel->adjustSize();
  this->nearclippingwheel->setEnabled( false );
  expandSize(tmpsize, this->nearclippingwheel->size(), LayoutHorizontal);
  QObject::connect(this->nearclippingwheel, SIGNAL(wheelPressed()),
                   this, SLOT(increaseInteractiveCount()));
  QObject::connect(this->nearclippingwheel, SIGNAL(wheelMoved(float)),
                   this, SLOT(nearclippingwheelMoved(float)));
  QObject::connect(this->nearclippingwheel, SIGNAL(wheelReleased()),
                   this, SLOT(decreaseInteractiveCount()));

  this->nearclippingedit = new QLineEdit(w);
  this->nearclippingedit->adjustSize();
  expandSize(tmpsize, this->nearclippingedit->size(), LayoutHorizontal);
  QObject::connect(this->nearclippingedit, SIGNAL(returnPressed()),
                   this, SLOT(nearclipEditPressed()));


  // Layout row 2.
  QBoxLayout * layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(this->nearclippinglabel, this->nearclippinglabel->width());
  layout->addWidget(this->nearclippingwheel, this->nearclippingwheel->width());
  layout->addWidget(this->nearclippingedit, this->nearclippingedit->width());
  expandSize(totalsize, tmpsize, LayoutVertical);


  // Do the three widgets on the third row (label, thumbwheel,
  // lineedit).

  tmpsize = QSize(0, 0);

  this->farclippinglabel = new QLabel("far plane", w);
  this->farclippinglabel->adjustSize();
  expandSize(tmpsize,
             QSize(QMAX(this->nearclippinglabel->width(),
                        this->farclippinglabel->width()),
                   this->farclippinglabel->height()),
             LayoutHorizontal);

  this->farclippingwheel = new QtThumbwheel(QtThumbwheel::Horizontal, w);
  this->farclippingwheel->adjustSize();
  this->farclippingwheel->setEnabled( false );
  expandSize(tmpsize, this->farclippingwheel->size(), LayoutHorizontal);
  QObject::connect(this->farclippingwheel, SIGNAL(wheelPressed()),
                   this, SLOT(increaseInteractiveCount()));
  QObject::connect(this->farclippingwheel, SIGNAL(wheelMoved(float)),
                   this, SLOT(farclippingwheelMoved(float)));
  QObject::connect(this->farclippingwheel, SIGNAL(wheelReleased()),
                   this, SLOT(decreaseInteractiveCount()));

  this->farclippingedit = new QLineEdit(w);
  this->farclippingedit->adjustSize();
  expandSize(tmpsize, this->farclippingedit->size(), LayoutHorizontal);
  QObject::connect(this->farclippingedit, SIGNAL(returnPressed()),
                   this, SLOT(farclipEditPressed()));

  // Layout row 3.
  layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(this->farclippinglabel,
                    QMAX(this->nearclippinglabel->width(),
                         this->farclippinglabel->width()));
  layout->addWidget(this->farclippingwheel, this->farclippingwheel->width());
  layout->addWidget(this->farclippingedit, this->farclippingedit->width());
  expandSize(totalsize, tmpsize, LayoutVertical);

  this->setEnabledClippingWidgets(!this->isAutoClipping());

  w->resize(totalsize);
  toplayout->activate();

  return w;
*/
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
SoGtkFullViewer::helpbuttonClicked()
{
  this->openViewerHelpCard();
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::homebuttonClicked()
{
  this->resetToHomePosition();
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::sethomebuttonClicked()
{
  this->saveHomePosition();
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::viewallbuttonClicked()
{
  this->viewAll();
}
*/

// *************************************************************************

/*!
  \internal
  Qt slot.
*/

/*
void
SoGtkFullViewer::seekbuttonClicked()
{
  this->setSeekMode(this->isSeekMode() ? FALSE : TRUE);
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
SoGtkFullViewer::seekbuttonClicked(
  void )
{
}

void
SoGtkFullViewer::drawstyleActivated(
  int )
{
}

// *************************************************************************
