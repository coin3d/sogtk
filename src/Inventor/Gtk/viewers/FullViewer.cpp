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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

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
#include <Inventor/Gtk/widgets/SoGtkPopupMenu.h>

#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>
#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>

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
  \class SoGtkFullViewer Inventor/Gtk/viewers/SoGtkFullViewer.h
  \brief The SoGtkFullViewer class adds decorations to the parent viewer class.

  \ingroup viewers

  TODO: more doc
  ...overview of what this class provides over parent class...
  ...keyboard combinations...mousebutton combinations...explain pan,
  rotate, zoom, dolly, seek...explain the fact that we're moving the
  camera, not the scene...
  ...viewer modes (examine vs interact) w/ explanation of what they are
  useful for...
*/

// *************************************************************************

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

  This is the form on the left side of the GL canvas.
  It is 30 pixels wide, and has the same height as the GL canvas.
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

  This variable 
*/

/*!
  \var GtkWidget * SoGtkFullViewer::bottomDecoration

  This is the form below the GL canvas.  It is 30 pixels high, and extends
  beyond the GL canvas by 30 pixels on both sides.
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

  This is the form containing the buttons and the wheel on the right
  side of the GL canvas.
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

  This variable contains the value of the thumb wheel on the right hand side
  of the GL canvas.  The value represents accumulated radians for the thumb
  wheel.

  This variable should only be accessed for reading the value, not for setting
  it.

  \sa setRightWheelValue
*/

/*!
  \var SoGtkPopupMenu * SoGtkFullViewer::prefmenu

  This variable contains the popup menu object or NULL if it has not been
  created yet.
*/

// *************************************************************************

class SoGtkFullViewerP {
public:
  SoGtkFullViewerP(SoGtkFullViewer * publ);
  ~SoGtkFullViewerP(void);

  GtkWidget * makePreferencesWindow(void);
  GtkWidget * makeSeekPreferences(GtkWidget * parent);
  GtkWidget * makeSeekDistancePreferences(GtkWidget * parent);
  GtkWidget * makeZoomPreferences(GtkWidget * parent);
  GtkWidget * makeAutoclipPreferences(GtkWidget * parent);
  GtkWidget * makeStereoPreferences(GtkWidget * parent);

  GtkWidget * viewerWidget, * canvasParent, * canvas;
  GtkWidget * interactbutton, * viewbutton, * seekbutton;

  SbBool decorations;

  SbBool menuEnabled;

  GtkWidget * mainLayout;
  void showDecorationWidgets(SbBool enable);

  GtkWidget * appButtonLayout;
  GtkWidget * appButtonForm;
  SbPList * appButtonList;
  void layoutAppButtons(GtkWidget * form);

  SbPList * viewerButtons;

  GtkWidget * prefwindow;
  SbString prefwindowtitle;

  GtkWidget * zoomslider;
  GtkWidget * zoomfield, * zoomrangefrom, * zoomrangeto;
  SbVec2f zoomrange;

  void setCameraZoom(const float zoom);
  float getCameraZoom(void);
  void setZoomSliderPosition(float zoom);
  void setZoomFieldString(float zoom);

  GtkWidget * seekdistancewheel;
  GtkWidget * seekdistancefield;

  GtkWidget * clippingtable;
  GtkWidget * nearclippinglabel, * farclippinglabel;
  GtkWidget * nearclippingwheel, * farclippingwheel;
  GtkWidget * nearclippingedit, * farclippingedit;

  void setEnabledClippingWidgets(SbBool flag);

  // Thumbwheels.
  void leftWheelPressed(void);
  void leftWheelChanged(float value);
  void leftWheelReleased(void);
  void rightWheelPressed(void);
  void rightWheelChanged(float value);
  void rightWheelReleased(void);
  void bottomWheelPressed(void);
  void bottomWheelChanged(float value);
  void bottomWheelReleased(void);

  // Pref sheet.
  static void preferencesDestroyed(GtkObject *, gpointer);

  //  seek settings
  static void seekAnimationTimeChanged(GtkEditable *, gpointer);
  static void seekDetailToggled(GtkToggleButton *, gpointer);
  static void seekDistanceWheelChanged(GtkWidget *, gpointer);
  static void seekDistanceEdit(GtkEditable *, gpointer);
  static void seekDistanceTypeToggle(GtkToggleButton *, gpointer);

  //  zoom settings
  static void zoomSliderMoved(GtkAdjustment *, gpointer);
  static void zoomFieldChanged(GtkEditable *, gpointer);
  static void zoomRangeChanged1(GtkEditable *, gpointer);
  static void zoomRangeChanged2(GtkEditable *, gpointer);

  //  clipping settings
  static void clippingToggled(GtkToggleButton *, gpointer);
  static void nearclippingwheelMoved(GtkWidget *, gpointer);
  static void farclippingwheelMoved(GtkWidget *, gpointer);
  static void nearclipEditPressed(GtkWidget *, gpointer);
  static void farclipEditPressed(GtkWidget *, gpointer);

  // Stereo settings
  static void stereoToggled(GtkToggleButton *, gpointer);

  // Generic Signal Handlers.
  static void increaseInteractiveCount(GtkWidget *, gpointer);
  static void decreaseInteractiveCount(GtkWidget *, gpointer);

  static SoGtkViewerButton SoGtkFullViewerButtons[];

  static void interactbuttonClickedCB(GtkWidget *, gpointer);
  static void viewbuttonClickedCB(GtkWidget *, gpointer);
  static void helpbuttonClickedCB(GtkWidget *, gpointer);
  static void homebuttonClickedCB(GtkWidget *, gpointer);
  static void sethomebuttonClickedCB(GtkWidget *, gpointer);
  static void viewallbuttonClickedCB(GtkWidget *, gpointer);
  static void seekbuttonClickedCB(GtkWidget *, gpointer);

  static void leftwheelPressedCB(GtkWidget *, gpointer);
  static void leftwheelMovedCB(GtkWidget *, gpointer);
  static void leftwheelReleasedCB(GtkWidget *, gpointer);
  static void bottomwheelPressedCB(GtkWidget *, gpointer);
  static void bottomwheelMovedCB(GtkWidget *, gpointer);
  static void bottomwheelReleasedCB(GtkWidget *, gpointer);
  static void rightwheelPressedCB(GtkWidget *, gpointer);
  static void rightwheelMovedCB(GtkWidget *, gpointer);
  static void rightwheelReleasedCB(GtkWidget *, gpointer);

private:
  SoGtkFullViewer * pub;

}; // class SoGtkFullViewerP

// *************************************************************************

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

inline float SbVec2fRange(const SbVec2f & vec) { return vec[1] - vec[0]; }

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkFullViewer);

// *************************************************************************

static const int VIEWERBORDER = 2;
static const int ZOOMSLIDERRESOLUTION = 200;

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
SoGtkFullViewerP::SoGtkFullViewerButtons[] = {
  { // interact button
    N_("interact"), "I",
    (GtkSignalFunc) SoGtkFullViewerP::interactbuttonClickedCB,
    pick_xpm
  },
  { // view
    N_("view"), "E",
    (GtkSignalFunc) SoGtkFullViewerP::viewbuttonClickedCB,
    view_xpm
  },
  { // help
    N_("help"), "?",
    (GtkSignalFunc) SoGtkFullViewerP::helpbuttonClickedCB,
    help_xpm
  },
  { // home
    N_("home"), "h",
    (GtkSignalFunc) SoGtkFullViewerP::homebuttonClickedCB,
    home_xpm
  },
  { // set home
    N_("set_home"), "H",
    (GtkSignalFunc) SoGtkFullViewerP::sethomebuttonClickedCB,
    set_home_xpm
  },
  { // view all
    N_("view_all"), "V",
    (GtkSignalFunc) SoGtkFullViewerP::viewallbuttonClickedCB,
    view_all_xpm
  },
  { // seek
    N_("seek"), "S",
    (GtkSignalFunc) SoGtkFullViewerP::seekbuttonClickedCB,
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
  SbBool build)
  : inherited(parent, name, embed, type, FALSE)
{
  this->pimpl = new SoGtkFullViewerP(this);

  this->leftDecoration = (GtkWidget *) NULL;
  this->leftWheel = (GtkWidget *) NULL;
  this->leftWheelLabel = (GtkWidget *) NULL;
  this->leftWheelStr = (char *) NULL;
  this->leftWheelVal = 0.0f;
  this->bottomDecoration = (GtkWidget *) NULL;
  this->bottomWheel = (GtkWidget *) NULL;
  this->bottomWheelLabel = (GtkWidget *) NULL;
  this->bottomWheelStr = (char *) NULL;
  this->bottomWheelVal = 0.0f;
  this->rightDecoration = (GtkWidget *) NULL;
  this->rightWheel = (GtkWidget *) NULL;
  this->rightWheelLabel = (GtkWidget *) NULL;
  this->rightWheelStr = (char *) NULL;
  this->rightWheelVal = 0.0f;

  this->setLeftWheelString(_("Motion Y"));
  this->setBottomWheelString(_("Motion X"));
  this->setRightWheelString(_("Motion Z"));

  this->setClassName("SoGtkFullViewer");

  this->prefmenu = (SoGtkPopupMenu *) NULL;

  PRIVATE(this)->menuEnabled = buildFlag & SoGtkFullViewer::BUILD_POPUP;
  PRIVATE(this)->decorations = buildFlag & SoGtkFullViewer::BUILD_DECORATION;

  if (! build) return;
  GtkWidget * viewer = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(viewer);
  // this->setSize(SbVec2s(500, 390));
}

/*!
  Destructor.
*/

SoGtkFullViewer::~SoGtkFullViewer(
  void)
{
  delete [] this->leftWheelStr;
  delete [] this->bottomWheelStr;
  delete [] this->rightWheelStr;
  delete this->pimpl;
}

// *************************************************************************

/*!
  Turn the viewer decorations on or off.

  \sa isDecoration()
*/

void
SoGtkFullViewer::setDecoration(
  const SbBool enable)
{
#if SOGTK_DEBUG
  if ((enable && this->isDecoration()) || (!enable && !this->isDecoration())) {
    SoDebugError::postWarning("SoGtkFullViewer::setDecoration",
                              "decorations already turned %s",
                              enable ? "on" : "off");
    return;
  }
#endif // SOGTK_DEBUG

  PRIVATE(this)->decorations = enable;
  if (PRIVATE(this)->viewerWidget)
    PRIVATE(this)->showDecorationWidgets(enable);
}

/*!
  Return \c TRUE if the viewer decorations are on, otherwise \c FALSE.

  \sa setDecoration()
*/

SbBool
SoGtkFullViewer::isDecoration(
  void) const
{
  return PRIVATE(this)->decorations;
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
  const SbBool on)
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
  PRIVATE(this)->menuEnabled = on;
}

/*!
  Return \c TRUE if the popup preferences menu is enabled,
  otherwise \c FALSE.

  \sa setPopupMenuEnabled()
*/

SbBool
SoGtkFullViewer::isPopupMenuEnabled(
  void) const
{
  return PRIVATE(this)->menuEnabled;
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
  void) const
{
  return PRIVATE(this)->appButtonForm;
}

/*!
  Add an application specific push button to the viewer decorations.
  Application buttons will be laid out in a vertical row from the
  upper left corner.

  The button will be added bottom-most.

  \sa insertAppPushButton(), removeAppPushButton(), getAppPushButtonParent()
*/

void
SoGtkFullViewer::addAppPushButton(
  GtkWidget * button)
{
  PRIVATE(this)->appButtonList->append(button);
  PRIVATE(this)->layoutAppButtons(this->getAppPushButtonParent());
}

/*!
  Insert an application specific push button to the viewer decorations
  at the given \c index.

  \sa addAppPushButton(), removeAppPushButton(), getAppPushButtonParent()
*/

void
SoGtkFullViewer::insertAppPushButton(
  GtkWidget * newButton,
  int index)
{
#if SOGTK_DEBUG
  if ((index < 0) || (index > this->lengthAppPushButton())) {
    SoDebugError::postWarning("SoGtkFullViewer::insertAppPushButton",
                              "index %d out of bounds", index);
    return;
  }
#endif // SOGTK_DEBUG
  PRIVATE(this)->appButtonList->insert(newButton, index);
  PRIVATE(this)->layoutAppButtons(this->getAppPushButtonParent());
}

/*!
  Remove one of the application specific buttons.

  \sa addAppPushButton(), insertAppPushButton()
*/

void
SoGtkFullViewer::removeAppPushButton(
  GtkWidget * oldButton)
{
  int idx = PRIVATE(this)->appButtonList->find(oldButton);

#if SOGTK_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoGtkFullViewer::removeAppPushButton",
                              "tried to remove non-existant button");
    return;
  }
#endif // SOGTK_DEBUG

  PRIVATE(this)->appButtonList->remove(idx);
  PRIVATE(this)->layoutAppButtons(this->getAppPushButtonParent());
}

/*!
  Return the index of a particular button that has been specified by
  the application, or -1 of the button has not been added.

  \sa addAppPushButton()
*/

int
SoGtkFullViewer::findAppPushButton(
  GtkWidget * button) const
{
  return PRIVATE(this)->appButtonList->find(button);
}

/*!
  Return number of application specific buttons added.

  \sa addAppPushButton(), insertAddAppPushButton()
*/

int
SoGtkFullViewer::lengthAppPushButton(
  void) const
{
  return PRIVATE(this)->appButtonList->getLength();
}

// *************************************************************************

/*!
  Returns the parent widget of the render area OpenGL canvas widget.

  \sa getGLWidget()
*/
GtkWidget *
SoGtkFullViewer::getRenderAreaWidget(void) const
{
  // We mirror SGI's original InventorXt toolkit, and do _not_ return
  // the OpenGL widget, but the widget above it (the one which
  // provides a border around the rendering canvas upon request).
  return PRIVATE(this)->canvas;
}

// *************************************************************************

// Documented in superclass. Overridden to correctly set the user
// interface indicators on the current state, namely the upper right
// push buttons indicating interact or view mode, the respective item
// on the popup menu and to grey out the seek mode activation button
// while in interact mode.
void
SoGtkFullViewer::setViewing(SbBool enable)
{
  if (enable == this->isViewing()) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkFullViewer::setViewing",
                               "view mode already %sset", enable ? "" : "un");
#endif // SOGTK_DEBUG
    return;
  }

  inherited::setViewing(enable);

  // FIXME: I believe we need to check that buttons have been built
  // before we attempt this. 20010821 mortene.
  // FIXME: shouldn't we change the state of the "view" and "pick"
  // buttons aswell? 20010821 mortene.
  gtk_widget_set_sensitive(PRIVATE(this)->seekbutton, enable ? TRUE : FALSE);
}

// *************************************************************************

// Documented in superclass.
void
SoGtkFullViewer::setCamera(SoCamera * camera)
{
  // Overridden from parent to make sure the user interface indicators
  // on the camera features in the preferences sheet are updated
  // correctly.

  inherited::setCamera(camera);

  if (PRIVATE(this)->prefwindow) {
    PRIVATE(this)->setZoomSliderPosition(PRIVATE(this)->getCameraZoom());
    PRIVATE(this)->setZoomFieldString(PRIVATE(this)->getCameraZoom());

    const gboolean enable = camera ? TRUE : FALSE;
    gtk_widget_set_sensitive(PRIVATE(this)->zoomslider, enable);
    gtk_widget_set_sensitive(PRIVATE(this)->zoomfield, enable); 
    gtk_widget_set_sensitive(PRIVATE(this)->zoomrangefrom, enable);
    gtk_widget_set_sensitive(PRIVATE(this)->zoomrangeto, enable); 
  }
}

// *************************************************************************

/*!
  Overloaded from parent class to make sure the preferences window
  will be hidden automatically whenever the viewer window is hidden.
*/

void
SoGtkFullViewer::hide(
  void)
{
  if (PRIVATE(this)->prefwindow)
    gtk_widget_hide(PRIVATE(this)->prefwindow);
  inherited::hide();
}

// *************************************************************************

/*!
  \internal

  Catch close events on the preferences window (to convert to hide events)
  and right mouse button presses (to pop up the preferences menu).
*/

SbBool
SoGtkFullViewer::eventFilter(
  GtkWidget * obj,
  GdkEvent * ev)
{
  if (inherited::eventFilter(obj, ev))
    return TRUE;
  return FALSE;
}

// *************************************************************************

// Documented in superclass.
GtkWidget *
SoGtkFullViewer::buildWidget(GtkWidget * parent)
{
  GtkWidget * root = gtk_vbox_new(FALSE, 0);
  g_return_val_if_fail(root != NULL, (GtkWidget *) NULL);
  GtkWidget * croot = gtk_hbox_new(FALSE, 0);
  g_return_val_if_fail(croot != NULL, (GtkWidget *) NULL);

  PRIVATE(this)->canvas = inherited::buildWidget(croot);

  PRIVATE(this)->viewerWidget = croot;
  PRIVATE(this)->canvasParent = parent;

  if (PRIVATE(this)->decorations) {
    this->buildDecoration(parent);
//    PRIVATE(this)->showDecorationWidgets(TRUE);
  }

  gtk_box_pack_start(GTK_BOX(croot), GTK_WIDGET(this->leftDecoration), FALSE, TRUE, FALSE);
  gtk_box_pack_start(GTK_BOX(croot), GTK_WIDGET(PRIVATE(this)->canvas), TRUE, TRUE, FALSE);
  gtk_box_pack_start(GTK_BOX(croot), GTK_WIDGET(this->rightDecoration), FALSE, TRUE, FALSE);
  gtk_box_pack_start(GTK_BOX(root), GTK_WIDGET(croot), TRUE, TRUE, FALSE);
  gtk_box_pack_start(GTK_BOX(root), GTK_WIDGET(this->bottomDecoration), FALSE, TRUE, FALSE);
  gtk_container_add(GTK_CONTAINER(parent), GTK_WIDGET(root));

  gtk_widget_show(GTK_WIDGET(this->leftDecoration));
  gtk_widget_show(GTK_WIDGET(PRIVATE(this)->canvas));
  gtk_widget_show(GTK_WIDGET(this->rightDecoration));
  gtk_widget_show(GTK_WIDGET(croot));
  gtk_widget_show(GTK_WIDGET(this->bottomDecoration));

  if (PRIVATE(this)->menuEnabled)
    this->buildPopupMenu();

  this->setSize(SbVec2s(500, 390));
  PRIVATE(this)->viewerWidget = root;
  return PRIVATE(this)->viewerWidget;
}

// *************************************************************************

/*!
  Build viewer decorations.
*/

void
SoGtkFullViewer::buildDecoration(
  GtkWidget * parent)
{
  this->leftDecoration   = this->buildLeftTrim(parent);
  this->bottomDecoration = this->buildBottomTrim(parent);
  this->rightDecoration  = this->buildRightTrim(parent);
}

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
  GtkWidget * parent)
{
  GtkWidget * trim = gtk_vbox_new(FALSE, TRUE);
  g_return_val_if_fail(trim != NULL, (GtkWidget *) NULL);
  gtk_widget_set_usize(GTK_WIDGET(trim), 30, 0);
  // set background color

#ifndef NO_THUMBWHEELS
  this->leftWheel = gtk_thumbwheel_new(1);
  g_return_val_if_fail(this->leftWheel != NULL, (GtkWidget *) NULL);
  gtk_misc_set_padding(GTK_MISC(this->leftWheel), 2, 2);

  gtk_signal_connect(GTK_OBJECT(this->leftWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::leftwheelPressedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(this->leftWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::leftwheelMovedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(this->leftWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::leftwheelReleasedCB), (gpointer) this);

  gtk_box_pack_end(GTK_BOX(trim), GTK_WIDGET(this->leftWheel), FALSE, FALSE, TRUE);
  gtk_widget_show(GTK_WIDGET(this->leftWheel));
#endif

  return trim;
}

// *************************************************************************

/*!
  Build decorations on the bottom of the render canvas. Overload this
  method in subclasses if you want your own decorations on the viewer window.
*/

GtkWidget *
SoGtkFullViewer::buildBottomTrim(
  GtkWidget * parent)
{
  GtkWidget * trim = gtk_hbox_new(FALSE, TRUE);
  g_return_val_if_fail(trim != NULL, (GtkWidget *) NULL);
  gtk_widget_set_usize(trim, 0, 30);

  this->leftWheelLabel = gtk_label_new(this->leftWheelStr);
  g_return_val_if_fail(this->leftWheelLabel != NULL, (GtkWidget *) NULL);
  gtk_misc_set_padding(GTK_MISC(this->leftWheelLabel), 3, 0);

  this->bottomWheelLabel = gtk_label_new(this->bottomWheelStr);
  g_return_val_if_fail(this->bottomWheelLabel != NULL, (GtkWidget *) NULL);
  gtk_misc_set_padding(GTK_MISC(this->bottomWheelLabel), 3, 0);

  this->rightWheelLabel = gtk_label_new(this->rightWheelStr);
  g_return_val_if_fail(this->rightWheelLabel != NULL, (GtkWidget *) NULL);
  gtk_misc_set_padding(GTK_MISC(this->rightWheelLabel), 3, 0);

#ifndef NO_THUMBWHEELS
  this->bottomWheel = gtk_thumbwheel_new(0);
  g_return_val_if_fail(this->bottomWheel != NULL, (GtkWidget *) NULL);
  gtk_misc_set_padding(GTK_MISC(this->bottomWheel), 2, 2);

  gtk_signal_connect(GTK_OBJECT(this->bottomWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::bottomwheelPressedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(this->bottomWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::bottomwheelMovedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(this->bottomWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::bottomwheelReleasedCB), (gpointer) this);
#endif

  gtk_box_pack_start(GTK_BOX(trim), GTK_WIDGET(this->leftWheelLabel), FALSE, TRUE, FALSE);
  gtk_box_pack_start(GTK_BOX(trim), GTK_WIDGET(this->bottomWheelLabel), FALSE, TRUE, FALSE);
#ifndef NO_THUMBWHEELS
  gtk_box_pack_start(GTK_BOX(trim), GTK_WIDGET(this->bottomWheel), FALSE, TRUE, TRUE);
#endif
  gtk_box_pack_end(GTK_BOX(trim), GTK_WIDGET(this->rightWheelLabel), FALSE, TRUE, FALSE);
  gtk_widget_show(GTK_WIDGET(this->leftWheelLabel));
  gtk_widget_show(GTK_WIDGET(this->bottomWheelLabel));
#ifndef NO_THUMBWHEELS
  gtk_widget_show(GTK_WIDGET(this->bottomWheel));
#endif
  gtk_widget_show(GTK_WIDGET(this->rightWheelLabel));

  return trim;
}

// *************************************************************************

/*!
  Build decorations on the right side of the render canvas. Overload this
  method in subclasses if you want your own decorations on the viewer window.
*/

GtkWidget *
SoGtkFullViewer::buildRightTrim(
  GtkWidget * parent)
{
  GtkWidget * trim = gtk_vbox_new(FALSE, TRUE);
  g_return_val_if_fail(trim != NULL, (GtkWidget *) NULL);
  gtk_widget_set_usize(trim, 30, 0);

  GtkWidget * buttons = this->buildViewerButtons(trim);
  g_return_val_if_fail(buttons != NULL, (GtkWidget *) NULL);
  gtk_widget_set_usize(buttons, 30, 0);
  gtk_widget_show(buttons);

#ifndef NO_THUMBWHEELS
  this->rightWheel = gtk_thumbwheel_new(1);
  g_return_val_if_fail(this->rightWheel != NULL, (GtkWidget *) NULL);
  gtk_misc_set_padding(GTK_MISC(this->rightWheel), 2, 2);

  gtk_signal_connect(GTK_OBJECT(this->rightWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::rightwheelPressedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(this->rightWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::rightwheelMovedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(this->rightWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewerP::rightwheelReleasedCB), (gpointer) this);
#endif

  gtk_box_pack_start(GTK_BOX(trim), GTK_WIDGET(buttons), FALSE, TRUE, TRUE);
#ifndef NO_THUMBWHEELS
  gtk_box_pack_end(GTK_BOX(trim), GTK_WIDGET(this->rightWheel), FALSE, TRUE, TRUE);

  gtk_widget_show(GTK_WIDGET(this->rightWheel));
#endif

  return trim;
}

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
  return PRIVATE(this)->appButtonForm;
}

// *************************************************************************

/*!
  Build and layout viewer specified button row.
*/

GtkWidget *
SoGtkFullViewer::buildViewerButtons(
  GtkWidget * parent)
{
  GtkWidget * buttons = gtk_vbox_new(FALSE, FALSE);

  this->createViewerButtons(buttons, PRIVATE(this)->viewerButtons);

  const int num = PRIVATE(this)->viewerButtons->getLength();
  for (int i = 0; i < num; i++) {
    GtkWidget * widget = (GtkWidget *) (*PRIVATE(this)->viewerButtons)[i];
    gtk_widget_set_usize(widget, 30, 30);
    gtk_box_pack_start(GTK_BOX(buttons), widget, TRUE, FALSE, FALSE);
    gtk_widget_show(widget);
  }
  gtk_widget_show(buttons);
  return buttons;
}

// *************************************************************************

// Documented in common/viewers/SoGuiFullViewer.cpp.in.
void
SoGtkFullViewer::createViewerButtons(GtkWidget * parent,
                                     SbPList * buttonlist)
{
  GtkTooltips *tooltips = gtk_tooltips_new ();

  GdkColormap *colormap = gtk_widget_get_colormap (parent);
  GdkBitmap *mask ;

  GSList *viewing_group = NULL ;
  const size_t buttons = sizeof(SoGtkFullViewerP::SoGtkFullViewerButtons) / sizeof(SoGtkViewerButton);
  for (size_t button = 0; button < buttons; button++) {
    GtkWidget * widget = (GtkWidget*) 0;
    switch (button) {
    case INTERACT_BUTTON:
      PRIVATE(this)->interactbutton = widget = gtk_radio_button_new(viewing_group);
      gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(widget),FALSE);
      viewing_group = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
        this->isViewing() ? FALSE : TRUE);
      break ;
    case EXAMINE_BUTTON:
      PRIVATE(this)->viewbutton = widget = gtk_radio_button_new(viewing_group);
      gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(widget),FALSE);
      viewing_group = gtk_radio_button_group(GTK_RADIO_BUTTON(widget));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),
        this->isViewing() ? TRUE : FALSE);
      break ;
    case SEEK_BUTTON:
      PRIVATE(this)->seekbutton = widget = gtk_button_new();
      gtk_widget_set_sensitive(widget,this->isViewing() ? TRUE : FALSE);
      break ;
    default:
      widget = gtk_button_new();
      break ;
    }

    GTK_WIDGET_UNSET_FLAGS (widget, GTK_CAN_FOCUS);
    gtk_tooltips_set_tip (tooltips, widget,
      _(SoGtkFullViewerP::SoGtkFullViewerButtons[button].keyword),
      (const gchar *) NULL);

    GdkPixmap * gdk_pixmap =
      gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) NULL, colormap,
                                            &mask, (GdkColor *) NULL,
                                            // FIXME: nasty cast, get
                                            // rid of it. 20020320 mortene.
                                            (gchar **)SoGtkFullViewerP::SoGtkFullViewerButtons[button].xpm_data);
    GtkWidget * label = gtk_pixmap_new(gdk_pixmap, mask);
    gtk_widget_show(label);

    gdk_pixmap_unref (gdk_pixmap);
    gdk_bitmap_unref (mask);

    gtk_container_add(GTK_CONTAINER(widget), GTK_WIDGET(label));
    if ((void *) SoGtkFullViewerP::SoGtkFullViewerButtons[button].pressed != NULL) {
      gtk_signal_connect(GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(SoGtkFullViewerP::SoGtkFullViewerButtons[button].pressed),
        (gpointer) this);
    }
    buttonlist->append(widget);
  }
}

// *************************************************************************

/*!
  Make a popup menu with preferences settings.
*/

void
SoGtkFullViewer::buildPopupMenu(
  void)
{
  this->prefmenu = this->setupStandardPopupMenu();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::openPopupMenu(
  const SbVec2s position)
{
  if (! this->prefmenu && PRIVATE(this)->menuEnabled)
    this->buildPopupMenu();
  if (this->prefmenu) {
    this->prepareMenu(this->prefmenu);
    this->prefmenu->popUp(this->getGLWidget(), position[0], position[1]);
  }
}

// *************************************************************************

/*!
  Overload this method in subclass viewers to append more widgets to
  the bottom of the preferences sheet window.
*/

GtkWidget *
SoGtkFullViewer::makeSubPreferences(
  GtkWidget *) // parent)
{
  return (GtkWidget *) NULL;
}

// *************************************************************************

/*!
  Set title of preferences sheet.
*/

void
SoGtkFullViewer::setPrefSheetString(
  const char * title)
{
  PRIVATE(this)->prefwindowtitle = title ? title : "";
  if (PRIVATE(this)->prefwindow)
    gtk_window_set_title (GTK_WINDOW (PRIVATE(this)->prefwindow),
      PRIVATE(this)->prefwindowtitle.getString());
}

// *************************************************************************

/*!
  Set label for the left thumbwheel.
*/

void
SoGtkFullViewer::setLeftWheelString(
  const char * const string)
{
  delete [] this->leftWheelStr;
  this->leftWheelStr = (char *) NULL;
  if (string)
    this->leftWheelStr = strcpy(new char [strlen(string)+1], string);
  if (this->leftWheelLabel)
    gtk_label_set_text(GTK_LABEL(this->leftWheelLabel),
      this->leftWheelStr ? this->leftWheelStr : "");
}

// *************************************************************************

/*!
  Set label of the bottom thumbwheel.
*/

void
SoGtkFullViewer::setBottomWheelString(
  const char * const string)
{
  delete [] this->bottomWheelStr;
  this->bottomWheelStr = (char *) NULL;
  if (string)
    this->bottomWheelStr = strcpy(new char [strlen(string)+1], string);
  if (this->bottomWheelLabel)
    gtk_label_set_text(GTK_LABEL(this->bottomWheelLabel),
      this->bottomWheelStr ? this->bottomWheelStr : "");
}

// *************************************************************************

/*!
  Set label of the right thumbwheel.
*/

void
SoGtkFullViewer::setRightWheelString(
  const char * const string)
{
  delete [] this->rightWheelStr;
  this->rightWheelStr = (char *) NULL;
  if (string)
    this->rightWheelStr = strcpy(new char [strlen(string)+1], string);
  if (this->rightWheelLabel)
    gtk_label_set_text(GTK_LABEL(this->rightWheelLabel),
      this->rightWheelStr ? this->rightWheelStr : "");
}

// *************************************************************************
// menu selections

/*!
  \internal
*/

void
SoGtkFullViewer::selectedViewing(void)
{
  this->setViewing(this->isViewing() ? FALSE : TRUE);
}

/*!
  \internal
*/

void
SoGtkFullViewer::selectedDecoration(void)
{
  this->setDecoration(this->isDecoration() ? FALSE : TRUE);
}

/*!
  \internal
*/

void
SoGtkFullViewer::selectedHeadlight(void)
{
  this->setHeadlight(this->isHeadlight() ? FALSE : TRUE);
}

/*!
  \internal
*/

void
SoGtkFullViewer::copyviewSelected(void)
{
  this->copyView(SbTime::getTimeOfDay());
}

/*!
  \internal
*/

void
SoGtkFullViewer::pasteviewSelected(void)
{
  this->pasteView(SbTime::getTimeOfDay());
}

/*!
  \internal
  SoGtk Slot.
*/

void
SoGtkFullViewer::selectedPrefs(void)
{
  if (! PRIVATE(this)->prefwindow) {
    PRIVATE(this)->prefwindow = PRIVATE(this)->makePreferencesWindow();
  }
  if (PRIVATE(this)->prefwindow) {
    gtk_widget_show(PRIVATE(this)->prefwindow);
  }
}

// *************************************************************************
// button selections

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::interactbuttonClicked(
  void)
{
  if (! this->isViewing()) {
    GtkWidget * button = PRIVATE(this)->interactbutton ;
    gtk_signal_handler_block_by_data(GTK_OBJECT(button), (gpointer) this);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_signal_handler_unblock_by_data(GTK_OBJECT(button), (gpointer) this);
    return;
  }
  this->setViewing(FALSE);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::viewbuttonClicked(
  void)
{

  if (this->isViewing()) {
    GtkWidget * button = PRIVATE(this)->viewbutton;
    gtk_signal_handler_block_by_data(GTK_OBJECT(button), (gpointer) this);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_signal_handler_unblock_by_data(GTK_OBJECT(button), (gpointer) this);
    return;
  }
  this->setViewing(TRUE);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::helpbuttonClicked(
  void)
{
  this->openViewerHelpCard();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::homebuttonClicked(
  void)
{
  this->resetToHomePosition();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::sethomebuttonClicked(
  void)
{
  this->saveHomePosition();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::viewallbuttonClicked(
  void)
{
  this->viewAll();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::seekbuttonClicked(
  void)
{
  this->setSeekMode(this->isSeekMode() ? FALSE : TRUE);
}

// *************************************************************************
//
//  Private implementation
//

/*
*/

SoGtkFullViewerP::SoGtkFullViewerP(
  SoGtkFullViewer * publ)
{
  this->pub = publ;

  this->viewerWidget = (GtkWidget *) NULL;
  this->canvas = (GtkWidget *) NULL;
  this->canvasParent = (GtkWidget *) NULL;

  this->zoomrange = SbVec2f(1.0f, 140.0f);

  this->mainLayout = (GtkWidget *) NULL;
  this->appButtonLayout = (GtkWidget *) NULL;

  this->prefwindow = (GtkWidget *) NULL;
  this->prefwindowtitle = _("Viewer Preference Sheet");

  this->menuEnabled = FALSE;
  this->decorations = FALSE;

  this->viewerButtons = new SbPList;
  this->appButtonList = new SbPList;
  this->appButtonForm = (GtkWidget *) NULL;

}

/*
*/

SoGtkFullViewerP::~SoGtkFullViewerP(
  void)
{
  delete this->viewerButtons;
  delete this->appButtonList;
}

// *************************************************************************

/*!
  \internal

  Show or hide decorations.
*/

void
SoGtkFullViewerP::showDecorationWidgets(SbBool enable)
{
//  if (this->mainLayout)
//    delete this->mainLayout;

  assert(this->viewerWidget);
  assert(this->canvasParent);

  if (enable) {
    gtk_widget_show(GTK_WIDGET(PUBLIC(this)->leftDecoration));
    gtk_widget_show(GTK_WIDGET(PUBLIC(this)->rightDecoration));
    gtk_widget_show(GTK_WIDGET(PUBLIC(this)->bottomDecoration));
  }
  else {
    gtk_widget_hide(GTK_WIDGET(PUBLIC(this)->leftDecoration));
    gtk_widget_hide(GTK_WIDGET(PUBLIC(this)->rightDecoration));
    gtk_widget_hide(GTK_WIDGET(PUBLIC(this)->bottomDecoration));
  }
}

// *************************************************************************

/*!
  Layout application specified buttons.
*/

void
SoGtkFullViewerP::layoutAppButtons(
  GtkWidget * form)
{
}

// *************************************************************************

// Documented in common/viewers/SoGuiFullViewer.cpp.in.
void
SoGtkFullViewer::createPrefSheet(void)
{
  PRIVATE(this)->prefwindow = PRIVATE(this)->makePreferencesWindow();
}

// *************************************************************************


/*!
  Create preferences sheet.
*/

GtkWidget *
SoGtkFullViewerP::makePreferencesWindow(
  void)
{
  this->prefwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (this->prefwindow),
    this->prefwindowtitle.getString());
  gtk_window_set_position (GTK_WINDOW (this->prefwindow),  GTK_WIN_POS_MOUSE);

  GtkWidget* form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (this->prefwindow), form);

  (void) makeSeekPreferences(form);
  (void) makeSeekDistancePreferences(form);
  (void) makeZoomPreferences(form);
  (void) makeAutoclipPreferences(form);
  (void) makeStereoPreferences(form);
  (void) PUBLIC(this)->makeSubPreferences(form);

  gtk_signal_connect (GTK_OBJECT (this->prefwindow), "destroy",
                      GTK_SIGNAL_FUNC(preferencesDestroyed),
                      (gpointer) PUBLIC(this));

  return this->prefwindow;
}

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::preferencesDestroyed(
  GtkObject * object,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  PRIVATE(viewer)->prefwindow = (GtkWidget *) NULL;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  seek-to-point functionality.
*/

GtkWidget *
SoGtkFullViewerP::makeSeekPreferences(
  GtkWidget * parent)
{
  GSList *rbg1 = (GSList *) NULL;

  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget *hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (form), hbox2, TRUE, TRUE, 0);

  GtkWidget *label6 = gtk_label_new (_("Seek animation time:"));
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
  sprintf(buffer, "%.2f", PUBLIC(this)->getSeekTime());
  gtk_entry_set_text (GTK_ENTRY (entry3), buffer);

  GtkWidget *label7 = gtk_label_new (_("seconds"));
  gtk_widget_show (label7);
  gtk_box_pack_start (GTK_BOX (hbox2), label7, FALSE, TRUE, 0);

  GtkWidget *hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (form), hbox3, TRUE, TRUE, 0);

  GtkWidget *label8 = gtk_label_new (_("Seek to:"));
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (hbox3), label8, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label8), 4, 0);

  GtkWidget *rb1 = gtk_radio_button_new_with_label (rbg1, _("point"));
  rbg1 = gtk_radio_button_group (GTK_RADIO_BUTTON (rb1));
  gtk_widget_show (rb1);
  gtk_box_pack_start (GTK_BOX (hbox3), rb1, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rb1),
    PUBLIC(this)->isDetailSeek());

  GtkWidget *rb2 = gtk_radio_button_new_with_label (rbg1, _("object"));
  rbg1 = gtk_radio_button_group (GTK_RADIO_BUTTON (rb2));
  gtk_widget_show (rb2);
  gtk_box_pack_start (GTK_BOX (hbox3), rb2, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rb1),
    ! PUBLIC(this)->isDetailSeek());

  gtk_signal_connect (GTK_OBJECT (entry3), "activate",
                      GTK_SIGNAL_FUNC (seekAnimationTimeChanged),
                      (gpointer) PUBLIC(this));

  gtk_signal_connect (GTK_OBJECT (rb1), "toggled",
                      GTK_SIGNAL_FUNC (seekDetailToggled),
                      (gpointer) PUBLIC(this));

  return form;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  seek-to-point functionality.
*/

GtkWidget *
SoGtkFullViewerP::makeSeekDistancePreferences(
  GtkWidget * parent)
{
  GSList * bg = (GSList *) NULL;

  GtkWidget * form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget * hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (form), hbox4, TRUE, TRUE, 0);

  GtkWidget * label4 = gtk_label_new (_("Seek distance:"));
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (hbox4), label4, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label4), 4, 0);

  this->seekdistancewheel = gtk_thumbwheel_new (0);
  gtk_thumbwheel_set_range_boundary_handling(
    GTK_THUMBWHEEL(this->seekdistancewheel),
    GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE);
  gtk_thumbwheel_set_value(GTK_THUMBWHEEL(this->seekdistancewheel),
    sqrt(PUBLIC(this)->getSeekDistance()));
  gtk_widget_show (this->seekdistancewheel);
  gtk_box_pack_start (GTK_BOX (hbox4), this->seekdistancewheel, FALSE, FALSE, 0);

  this->seekdistancefield = gtk_entry_new ();
  gtk_widget_show (this->seekdistancefield);
  gtk_widget_set_usize(this->seekdistancefield,64,0);
  gtk_entry_set_max_length(GTK_ENTRY(this->seekdistancefield), 6);
  gtk_box_pack_start (GTK_BOX (hbox4), this->seekdistancefield, FALSE, FALSE, 0);

  char buffer[16];
  sprintf(buffer, "%.2f", PUBLIC(this)->getSeekDistance());
  gtk_entry_set_text (GTK_ENTRY (this->seekdistancefield), buffer);

  GtkWidget *hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (form), hbox5, TRUE, FALSE, 0);

  GtkWidget *r1 = gtk_radio_button_new_with_label (bg, _("percentage"));
  bg = gtk_radio_button_group (GTK_RADIO_BUTTON (r1));
  gtk_widget_show (r1);
  gtk_box_pack_start (GTK_BOX (hbox5), r1, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r1),
    PUBLIC(this)->isSeekValuePercentage());

  GtkWidget *r2 = gtk_radio_button_new_with_label (bg, _("absolute"));
  bg = gtk_radio_button_group (GTK_RADIO_BUTTON (r2));
  gtk_widget_show (r2);
  gtk_box_pack_start (GTK_BOX (hbox5), r2, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (r2),
    ! PUBLIC(this)->isSeekValuePercentage());

  gtk_signal_connect (GTK_OBJECT (this->seekdistancefield), "activate",
                      GTK_SIGNAL_FUNC (seekDistanceEdit),
                      (gpointer) PUBLIC(this));
  gtk_signal_connect (GTK_OBJECT (this->seekdistancewheel), "value_changed",
                      GTK_SIGNAL_FUNC (seekDistanceWheelChanged),
                      (gpointer) PUBLIC(this));
  gtk_signal_connect (GTK_OBJECT (r1), "toggled",
                      GTK_SIGNAL_FUNC (seekDistanceTypeToggle),
                      (gpointer) PUBLIC(this));

  return form;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  camera zoom functionality.
*/

GtkWidget *
SoGtkFullViewerP::makeZoomPreferences(
  GtkWidget * parent)
{
  char buffer[16] ;

  GtkWidget * form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (parent), form);

  GtkWidget * hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (form), hbox1, TRUE, FALSE, 0);

  GtkWidget * label9 = gtk_label_new (_("Camera Zoom:"));
  gtk_widget_show (label9);
  gtk_box_pack_start (GTK_BOX (hbox1), label9, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label9), 0.0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label9), 4, 0);

  GtkObject * adj = gtk_adjustment_new (
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

  GtkWidget *label10 = gtk_label_new (_("Zoom slider ranges from:"));
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (hbox2), label10, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);
  gtk_misc_set_padding (GTK_MISC (label10), 4, 0);

  this->zoomrangefrom = gtk_entry_new();
  sprintf(buffer, "%.1f", this->zoomrange[0]);
  gtk_entry_set_text(GTK_ENTRY(this->zoomrangefrom), buffer);
  gtk_widget_set_usize(this->zoomrangefrom,64,20);
  gtk_widget_show (this->zoomrangefrom);
  gtk_box_pack_start (GTK_BOX (hbox2), this->zoomrangefrom, FALSE, FALSE, 0);

  GtkWidget *label11 = gtk_label_new (_("to:"));
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox2), label11, FALSE, FALSE, 0);

  this->zoomrangeto = gtk_entry_new ();
  sprintf(buffer, "%.1f", this->zoomrange[1]);
  gtk_entry_set_text(GTK_ENTRY(this->zoomrangeto), buffer);
  gtk_widget_set_usize(this->zoomrangeto,64,20);
  gtk_widget_show (this->zoomrangeto);
  gtk_box_pack_start (GTK_BOX (hbox2), this->zoomrangeto, FALSE, FALSE, 0);

  SoCamera *cam = PUBLIC(this)->getCamera();
  if (cam) {
    this->setZoomSliderPosition(this->getCameraZoom());
    this->setZoomFieldString(this->getCameraZoom());
  } else {
    gtk_widget_set_sensitive(this->zoomslider, FALSE);
    gtk_widget_set_sensitive(this->zoomfield, FALSE); 
    gtk_widget_set_sensitive(this->zoomrangefrom, FALSE);
    gtk_widget_set_sensitive(this->zoomrangeto, FALSE); 
  }

  gtk_signal_connect (GTK_OBJECT (this->zoomfield), "activate",
                      GTK_SIGNAL_FUNC (zoomFieldChanged),
                      (gpointer) PUBLIC(this));

  gtk_signal_connect (GTK_OBJECT (this->zoomrangefrom), "activate",
                      GTK_SIGNAL_FUNC (zoomRangeChanged1),
                      (gpointer) PUBLIC(this));

  gtk_signal_connect (GTK_OBJECT (this->zoomrangeto), "activate",
                      GTK_SIGNAL_FUNC (zoomRangeChanged2),
                      (gpointer) PUBLIC(this));

  gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
                      GTK_SIGNAL_FUNC (zoomSliderMoved),
                      (gpointer) PUBLIC(this));

  return form;
}

// *************************************************************************

/*!
  Enable or disable interaction through the near and far clipping
  widgets.
*/

void
SoGtkFullViewerP::setEnabledClippingWidgets(
  SbBool flag)
{
#if 1
  gtk_widget_set_sensitive(this->clippingtable, flag);
#else
// FIXME: This trips a bug in Gtk's interaction with gtkthumbwheel
// Could be a bug in gtk_table or in gtk_thumbwheel :)
  if (flag)
  {
    gtk_widget_show(this->clippingtable);
//    gtk_widget_show(this->nearclippinglabel);
//    gtk_widget_show(this->nearclippingwheel);
//    gtk_widget_show(this->nearclippingedit); 
//    gtk_widget_show(this->farclippinglabel); 
//    gtk_widget_show(this->farclippingwheel); 
//    gtk_widget_show(this->farclippingedit);  
  }
  else
  {
    gtk_widget_hide(this->clippingtable);
//    gtk_widget_hide(this->nearclippinglabel);
//    gtk_widget_hide(this->nearclippingwheel);
//    gtk_widget_hide(this->nearclippingedit); 
//    gtk_widget_hide(this->farclippinglabel); 
//    gtk_widget_hide(this->farclippingwheel); 
//    gtk_widget_hide(this->farclippingedit);  
  }
#endif
  SoCamera * cam = PUBLIC(this)->getCamera();
  if (!cam) return ;

  {
    char buffer[16] ;

    sprintf(buffer, "%.3f", cam->nearDistance.getValue());
    gtk_entry_set_text(GTK_ENTRY(this->nearclippingedit), buffer);

    sprintf(buffer, "%.3f", cam->farDistance.getValue());
    gtk_entry_set_text(GTK_ENTRY(this->farclippingedit), buffer);
  }
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  manual control of the near and far clippping planes.
*/

GtkWidget *
SoGtkFullViewerP::makeAutoclipPreferences(
  GtkWidget * dialog)
{
  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (dialog), form);

  GtkWidget *checkbutton1 = gtk_check_button_new_with_label (
    _("Auto clipping planes"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), 
     PUBLIC(this)->isAutoClipping());
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (form), checkbutton1, FALSE, FALSE, 0);

  this->clippingtable = gtk_table_new (2, 3, FALSE);
  gtk_box_pack_start (GTK_BOX (form), this->clippingtable, FALSE, FALSE, 0);
  gtk_widget_show(this->clippingtable);

  this->nearclippinglabel = gtk_label_new (
    _("near plane:"));
  gtk_label_set_justify (GTK_LABEL (this->nearclippinglabel), GTK_JUSTIFY_RIGHT); 
  gtk_misc_set_alignment (GTK_MISC (this->nearclippinglabel), 1, 0.5);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->nearclippinglabel, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show(this->nearclippinglabel);

  this->nearclippingwheel = gtk_thumbwheel_new (0);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->nearclippingwheel, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show(this->nearclippingwheel);

  this->nearclippingedit = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->nearclippingedit, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show(this->nearclippingedit);

  this->farclippinglabel = gtk_label_new (
    _("far plane:"));
  gtk_label_set_justify (GTK_LABEL (this->farclippinglabel), GTK_JUSTIFY_RIGHT); 
  gtk_misc_set_alignment (GTK_MISC (this->farclippinglabel), 1, 0.5);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->farclippinglabel, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show(this->farclippinglabel);

  this->farclippingwheel = gtk_thumbwheel_new (0);
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->farclippingwheel, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show(this->farclippingwheel);

  this->farclippingedit = gtk_entry_new ();
  gtk_table_attach (GTK_TABLE (this->clippingtable), this->farclippingedit, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_show(this->farclippingedit);

  gtk_signal_connect (GTK_OBJECT (checkbutton1), 
    "toggled", GTK_SIGNAL_FUNC (clippingToggled), PUBLIC(this));

  gtk_signal_connect(GTK_OBJECT(this->nearclippingwheel), 
    "attached", GTK_SIGNAL_FUNC (increaseInteractiveCount), PUBLIC(this));
  gtk_signal_connect(GTK_OBJECT(this->nearclippingwheel), 
    "value_changed", GTK_SIGNAL_FUNC (nearclippingwheelMoved), PUBLIC(this));
  gtk_signal_connect(GTK_OBJECT(this->nearclippingwheel), 
    "released", GTK_SIGNAL_FUNC (decreaseInteractiveCount), PUBLIC(this));

  gtk_signal_connect(GTK_OBJECT(this->farclippingwheel), 
    "attached", GTK_SIGNAL_FUNC (increaseInteractiveCount), PUBLIC(this));
  gtk_signal_connect(GTK_OBJECT(this->farclippingwheel), 
    "value_changed", GTK_SIGNAL_FUNC (farclippingwheelMoved), PUBLIC(this));
  gtk_signal_connect(GTK_OBJECT(this->farclippingwheel), 
    "released", GTK_SIGNAL_FUNC (decreaseInteractiveCount), PUBLIC(this));

  this->setEnabledClippingWidgets(! PUBLIC(this)->isAutoClipping());

  return form;
}

// *************************************************************************

/*!
  Create the UI representation of the preferences' settings for the
  stereo viewing controls.
*/

GtkWidget *
SoGtkFullViewerP::makeStereoPreferences(
  GtkWidget * dialog)
{
  GtkWidget *form = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form);
  gtk_container_add (GTK_CONTAINER (dialog), form);

  GtkWidget *checkbutton1 = gtk_check_button_new_with_label (
    _("Stereo Viewing"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), 
     PUBLIC(this)->isStereoViewing());
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (form), checkbutton1, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (checkbutton1), 
    "toggled", GTK_SIGNAL_FUNC (stereoToggled), PUBLIC(this));

  return form;
}

// *************************************************************************

/*!
  Set camera zoom value.
*/

void
SoGtkFullViewerP::setCameraZoom(
  const float val)
{
  SoCamera * cam = PUBLIC(this)->getCamera();
  if (cam == NULL) return; // can happen if scenegraph is empty

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
SoGtkFullViewerP::getCameraZoom(
  void)
{
  SoCamera * cam = PUBLIC(this)->getCamera();
  if (cam == NULL) return 0.0f; // can happen if scenegraph is empty

  SoType t = cam->getTypeId();

  if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
    return ((SoPerspectiveCamera *)cam)->heightAngle.getValue() /
      2.0f * 360.0f / M_PI;
  else if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
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
SoGtkFullViewerP::setZoomSliderPosition(
  float zoom)
{
  if (!this->prefwindow) return;

  float f = (zoom - this->zoomrange[0]) / SbVec2fRange(this->zoomrange);
  f = SoGtkClamp(f, 0.0f, 1.0f) * ZOOMSLIDERRESOLUTION;

  GtkAdjustment *adj = GTK_RANGE(this->zoomslider)->adjustment;
  gtk_adjustment_set_value(adj, f);
}

// *************************************************************************

/*!
  Set string in the camera zoom edit field.
*/

void
SoGtkFullViewerP::setZoomFieldString(
  float zoom)
{
  if (!this->prefwindow) return;

  char buffer[16];
  sprintf(buffer, "%.1f", zoom);
  gtk_entry_set_text(GTK_ENTRY(this->zoomfield), buffer);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::seekAnimationTimeChanged(
  GtkEditable * editable,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  char * s = gtk_editable_get_chars(editable, 0, -1);
  float val;
  if ((sscanf(s, "%f", &val) == 1) && (val >= 0.0f))
    viewer->setSeekTime(val);
  g_free(s);

  char buffer[16] ;
  sprintf(buffer, "%.2f", viewer->getSeekTime());
  gtk_entry_set_text(GTK_ENTRY(editable), buffer);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::seekDetailToggled(
  GtkToggleButton * button,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->setDetailSeek(gtk_toggle_button_get_active(button) ?
    TRUE : FALSE);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::seekDistanceWheelChanged(
  GtkWidget * wheel,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  gfloat val = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));

  if (val < 0.1f) {
    val = 0.1f;
    gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(viewer)->seekdistancewheel), val);
  }
  viewer->setSeekDistance(val * val);

  char buffer[16] ;
  sprintf(buffer, "%.2f", viewer->getSeekDistance());
  gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->seekdistancefield), buffer);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::seekDistanceEdit(
  GtkEditable * editable,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  char *s = gtk_editable_get_chars(editable,0,-1);
  float val;

  if ((sscanf(s, "%f", &val) == 1) && (val > 0.0f))
  {
    viewer->setSeekDistance(val);
    gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(viewer)->seekdistancewheel),
      sqrt(val));
  }
  g_free(s);

  /* else */
  {
    char buffer[16] ;
    sprintf(buffer, "%.2f", viewer->getSeekDistance());
    gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->seekdistancefield), buffer);
  }
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::seekDistanceTypeToggle(
  GtkToggleButton * button,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;

  viewer->setSeekValueAsPercentage(gtk_toggle_button_get_active(button) ?
    TRUE : FALSE);
}


// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::zoomSliderMoved(
  GtkAdjustment * adjustment,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;

  float f = adjustment->value / float(ZOOMSLIDERRESOLUTION);
  f = PRIVATE(viewer)->zoomrange[0] + f * SbVec2fRange(PRIVATE(viewer)->zoomrange);

  PRIVATE(viewer)->setCameraZoom(f);
  PRIVATE(viewer)->setZoomFieldString(f);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::zoomFieldChanged(
  GtkEditable * editable,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;

  float val;
  char *s = gtk_editable_get_chars(editable,0,-1);
  if (sscanf(s, "%f", &val) == 1) {
    val = SoGtkClamp(val, 0.001f, 179.999f);
    PRIVATE(viewer)->setCameraZoom(val);
    PRIVATE(viewer)->setZoomSliderPosition(val);
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf(buffer, "%.1f", PRIVATE(viewer)->getCameraZoom());
    gtk_entry_set_text(GTK_ENTRY(editable), buffer);
  }
}


// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::zoomRangeChanged1(
  GtkEditable * editable,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;

  float val;
  char *s = gtk_editable_get_chars(editable,0,-1);
  if (sscanf(s, "%f", &val) == 1) {
    if (val > 0.0f && val < 180.0f && val < PRIVATE(viewer)->zoomrange[1]) {
      PRIVATE(viewer)->zoomrange[0] = val;
      PRIVATE(viewer)->setZoomSliderPosition(PRIVATE(viewer)->getCameraZoom());
    }
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf(buffer, "%.1f", PRIVATE(viewer)->zoomrange[0]);
    gtk_entry_set_text(GTK_ENTRY(editable), buffer);
  }
}

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::zoomRangeChanged2(
  GtkEditable * editable,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer *) closure;

  float val;
  char *s = gtk_editable_get_chars(editable,0,-1);
  if (sscanf(s, "%f", &val) == 1) {
    if (val > 0.0f && val < 180.0f && val > PRIVATE(viewer)->zoomrange[0]) {
      PRIVATE(viewer)->zoomrange[1] = val;
      PRIVATE(viewer)->setZoomSliderPosition(PRIVATE(viewer)->getCameraZoom());
    }
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf(buffer, "%.1f", PRIVATE(viewer)->zoomrange[1]);
    gtk_entry_set_text(GTK_ENTRY(editable), buffer);
  }
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::clippingToggled(
  GtkToggleButton * toggle_button,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  bool flag = gtk_toggle_button_get_active (toggle_button);

  viewer->setAutoClipping(flag);
  PRIVATE(viewer)->setEnabledClippingWidgets(!flag);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::increaseInteractiveCount(
  GtkWidget * w,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure;
  viewer->interactiveCountInc();
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::decreaseInteractiveCount(
  GtkWidget * w,
  gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure;
  viewer->interactiveCountDec();
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::nearclippingwheelMoved(
  GtkWidget * w,
  gpointer closure)
{
  GtkThumbWheel * nearwheel = (GtkThumbWheel *) w;
  SoGtkFullViewer* viewer = (SoGtkFullViewer *) closure;

  assert(PRIVATE(viewer)->farclippingwheel != NULL);

  float val = gtk_thumbwheel_get_value(nearwheel);
  if (val < 0.001f) {
    val = 0.001f;
    gtk_thumbwheel_set_value(nearwheel, val);
  }
  float farval = gtk_thumbwheel_get_value(
    GTK_THUMBWHEEL(PRIVATE(viewer)->farclippingwheel));
  if (val >= farval) {
    val = farval - 0.001f;
    gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(viewer)->nearclippingwheel), val);
  }

  SoCamera * cam = viewer->getCamera();
  if (!cam) return;

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->nearDistance = val;

  char buffer[16] ;
  sprintf(buffer, "%.3f", cam->nearDistance.getValue());
  gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->nearclippingedit), buffer);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::farclippingwheelMoved(
  GtkWidget * w,
  gpointer closure)
{
  GtkThumbWheel * farwheel = (GtkThumbWheel *) w;
  SoGtkFullViewer* viewer = (SoGtkFullViewer *) closure;

  assert(PRIVATE(viewer)->nearclippingwheel != NULL);

  float val = gtk_thumbwheel_get_value(farwheel);
  float nearval = gtk_thumbwheel_get_value(
    GTK_THUMBWHEEL(PRIVATE(viewer)->nearclippingwheel));
  if (val < nearval) {
    val = nearval + 0.001f;
    gtk_thumbwheel_set_value(farwheel, val);
  }

  SoCamera * cam = viewer->getCamera();
  if (!cam) return ;

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->farDistance = val;

  char buffer[16] ;
  sprintf(buffer, "%.3f", cam->farDistance.getValue());
  gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->farclippingedit), buffer);
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::nearclipEditPressed(
  GtkWidget	*w,
  gpointer	closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure ;

  SoCamera * cam = viewer->getCamera();
  if (!cam) return;

  char *s =
    gtk_editable_get_chars(GTK_EDITABLE(PRIVATE(viewer)->nearclippingedit),0,-1);
  float val;
  if (sscanf(s, "%f", &val) == 1) {
    // FIXME: sanity check on val? 990223 mortene.
    cam->nearDistance = val;
    gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(viewer)->nearclippingwheel), val);
  }
  g_free(s);

  /* else */
  {
    char buffer[16] ;
    sprintf(buffer, "%.3f", cam->nearDistance.getValue());
    gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->nearclippingedit), buffer);
  }
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::farclipEditPressed(
  GtkWidget * w,
  gpointer closure)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure ;

  SoCamera * cam = viewer->getCamera();
  if (!cam) return;

  char *s = gtk_editable_get_chars(GTK_EDITABLE(PRIVATE(viewer)->farclippingedit),0,-1);
  float val;
  if (sscanf(s, "%f", &val) == 1) {
    // FIXME: sanity check on val? 990223 mortene.
    cam->farDistance = val;
    gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(viewer)->farclippingwheel), val);
  }
  g_free(s);

  /* else */
  {
    char buffer[16] ;
    sprintf(buffer, "%.3f", cam->farDistance.getValue());
    gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->farclippingedit), buffer);
  }
}

// *************************************************************************

/*!
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::stereoToggled(
  GtkToggleButton *toggle_button,
  gpointer         closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure ;
  bool flag = gtk_toggle_button_get_active (toggle_button);

  SOGTK_STUB();

  // FIXME: this is a dummy dialog
  SoGtk::createSimpleErrorDialog(GTK_WIDGET(toggle_button), 
    "Stereo Error Dialog", 
    _("Stereo Viewing can't be set on this machine."));
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::interactbuttonClickedCB(GtkWidget *w,
                                          gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;

  if (viewer->isViewing())
    viewer->setViewing(FALSE);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::viewbuttonClickedCB(GtkWidget * w,
                                      gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  if (!viewer->isViewing()) 
    viewer->setViewing(TRUE);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::helpbuttonClickedCB(GtkWidget *,
                                      gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->helpbuttonClicked();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::homebuttonClickedCB(GtkWidget *,
                                      gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->homebuttonClicked();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::sethomebuttonClickedCB(GtkWidget *,
                                         gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->sethomebuttonClicked();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::viewallbuttonClickedCB(GtkWidget *,
                                         gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->viewallbuttonClicked();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::seekbuttonClickedCB(GtkWidget *,
                                      gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  viewer->seekbuttonClicked();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::leftwheelPressedCB(GtkWidget *,
                                     gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelStart();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::leftwheelMovedCB(GtkWidget * wheel,
                                   gpointer closure)
{
  assert(closure != NULL);
  gfloat value = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelMotion(value);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::leftwheelReleasedCB(GtkWidget *,
                                      gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelFinish();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::bottomwheelPressedCB(GtkWidget *,
                                       gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelStart();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::bottomwheelMovedCB(GtkWidget * wheel,
                                     gpointer closure)
{
  assert(closure != NULL);
  gfloat value = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelMotion(value);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::bottomwheelReleasedCB(GtkWidget *,
                                        gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelFinish();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::rightwheelPressedCB(GtkWidget *,
                                      gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelStart();
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::rightwheelMovedCB(GtkWidget * wheel,
                                    gpointer closure)
{
  assert(closure != NULL);
  gfloat value = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelMotion(value);
}

/*!
  FIXME: write doc
*/

void
SoGtkFullViewerP::rightwheelReleasedCB(GtkWidget *,
                                       gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelFinish();
}

// *************************************************************************

// Documented in superclass.
void
SoGtkFullViewer::sizeChanged(const SbVec2s & size)
{
  // FIXME: nothing for SoGtk. Check SoQt and / or SoXt to see what
  // happens there -- perhaps we've forgotten something important?
  // 20020109 mortene.
  inherited::sizeChanged(size);
}

// *************************************************************************
