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

// this stops IRIX build from crashing
// #define NO_THUMBWHEELS

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

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
#include <Inventor/Gtk/viewers/SoGtkFullViewerP.h>

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
};

// *************************************************************************

/*!
  Constructor. See parent class for explanation of arguments.

  Subclasses will probably usually call with the \c build flag
  set to \c FALSE to be able to do delayed building of the OpenGL
  canvas after other setup tasks has been performed.
*/

SoGtkFullViewer::SoGtkFullViewer(GtkWidget * parent,
                                 const char * name,
                                 SbBool embed,
                                 SoGtkFullViewer::BuildFlag buildFlag,
                                 SoGtkViewer::Type type,
                                 SbBool build)
  : inherited(parent, name, embed, type, FALSE)
{
  PRIVATE(this) = new SoGtkFullViewerP(this);

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

SoGtkFullViewer::~SoGtkFullViewer()
{
  delete [] this->leftWheelStr;
  delete [] this->bottomWheelStr;
  delete [] this->rightWheelStr;
  delete PRIVATE(this);
}

// *************************************************************************

/*!
  Turn the viewer decorations on or off.

  \sa isDecoration()
*/

void
SoGtkFullViewer::setDecoration(const SbBool enable)
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
SoGtkFullViewer::isDecoration(void) const
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
SoGtkFullViewer::setPopupMenuEnabled(const SbBool on)
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
SoGtkFullViewer::isPopupMenuEnabled(void) const
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
SoGtkFullViewer::getAppPushButtonParent(void) const
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
SoGtkFullViewer::addAppPushButton(GtkWidget * button)
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
SoGtkFullViewer::insertAppPushButton(GtkWidget * newButton,
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
SoGtkFullViewer::removeAppPushButton(GtkWidget * oldButton)
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
SoGtkFullViewer::findAppPushButton(GtkWidget * button) const
{
  return PRIVATE(this)->appButtonList->find(button);
}

/*!
  Return number of application specific buttons added.

  \sa addAppPushButton(), insertAddAppPushButton()
*/

int
SoGtkFullViewer::lengthAppPushButton(void) const
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
SoGtkFullViewer::buildDecoration(GtkWidget * parent)
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
SoGtkFullViewer::buildLeftTrim(GtkWidget * parent)
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
SoGtkFullViewer::buildBottomTrim(GtkWidget * parent)
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
SoGtkFullViewer::buildRightTrim(GtkWidget * parent)
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
SoGtkFullViewer::buildViewerButtons(GtkWidget * parent)
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
SoGtkFullViewer::buildPopupMenu(void)
{
  this->prefmenu = PRIVATE(this)->setupStandardPopupMenu();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFullViewer::openPopupMenu(const SbVec2s position)
{
  if (! this->prefmenu && PRIVATE(this)->menuEnabled)
    this->buildPopupMenu();
  if (this->prefmenu) {
    PRIVATE(this)->prepareMenu(this->prefmenu);
    this->prefmenu->popUp(this->getGLWidget(), position[0], position[1]);
  }
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
SoGtkFullViewer::setBottomWheelString(const char * const string)
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
SoGtkFullViewer::setRightWheelString(const char * const string)
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

void
SoGtkFullViewerP::selectedViewing(void)
{
  PUBLIC(this)->setViewing(PUBLIC(this)->isViewing() ? FALSE : TRUE);
}

void
SoGtkFullViewerP::selectedDecoration(void)
{
  PUBLIC(this)->setDecoration(PUBLIC(this)->isDecoration() ? FALSE : TRUE);
}

void
SoGtkFullViewerP::selectedHeadlight(void)
{
  PUBLIC(this)->setHeadlight(PUBLIC(this)->isHeadlight() ? FALSE : TRUE);
}

void
SoGtkFullViewerP::copyviewSelected(void)
{
  PUBLIC(this)->copyView(SbTime::getTimeOfDay());
}

void
SoGtkFullViewerP::pasteviewSelected(void)
{
  PUBLIC(this)->pasteView(SbTime::getTimeOfDay());
}

// *************************************************************************
// button selections

void
SoGtkFullViewerP::interactbuttonClicked(void)
{
  if (!PUBLIC(this)->isViewing()) {
    GtkWidget * button = this->interactbutton ;
    gtk_signal_handler_block_by_data(GTK_OBJECT(button), (gpointer) this);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_signal_handler_unblock_by_data(GTK_OBJECT(button), (gpointer) this);
    return;
  }
  PUBLIC(this)->setViewing(FALSE);
}

void
SoGtkFullViewerP::viewbuttonClicked(void)
{
  if (PUBLIC(this)->isViewing()) {
    GtkWidget * button = this->viewbutton;
    gtk_signal_handler_block_by_data(GTK_OBJECT(button), (gpointer) this);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_signal_handler_unblock_by_data(GTK_OBJECT(button), (gpointer) this);
    return;
  }
  PUBLIC(this)->setViewing(TRUE);
}

void
SoGtkFullViewerP::helpbuttonClicked(void)
{
  PUBLIC(this)->openViewerHelpCard();
}

void
SoGtkFullViewerP::homebuttonClicked(void)
{
  PUBLIC(this)->resetToHomePosition();
}

void
SoGtkFullViewerP::sethomebuttonClicked(void)
{
  PUBLIC(this)->saveHomePosition();
}

void
SoGtkFullViewerP::viewallbuttonClicked(void)
{
  PUBLIC(this)->viewAll();
}

void
SoGtkFullViewerP::seekbuttonClicked(void)
{
  PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

// *************************************************************************
//
//  Private implementation
//

/*
 */

SoGtkFullViewerP::SoGtkFullViewerP(SoGtkFullViewer * publ)
  : SoGuiFullViewerP(publ)
{
  this->viewerWidget = (GtkWidget *) NULL;
  this->canvas = (GtkWidget *) NULL;
  this->canvasParent = (GtkWidget *) NULL;

  this->mainLayout = (GtkWidget *) NULL;
  this->appButtonLayout = (GtkWidget *) NULL;

  this->menuEnabled = FALSE;
  this->decorations = FALSE;

  this->viewerButtons = new SbPList;
  this->appButtonList = new SbPList;
  this->appButtonForm = (GtkWidget *) NULL;
}

/*
 */

SoGtkFullViewerP::~SoGtkFullViewerP()
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
SoGtkFullViewerP::layoutAppButtons(GtkWidget * form)
{
}

// *************************************************************************

/*!
  Set camera zoom value.
*/

void
SoGtkFullViewerP::setCameraZoom(const float val)
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
SoGtkFullViewerP::getCameraZoom(void)
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
  \internal
  Gtk Signal Handler.
*/

void
SoGtkFullViewerP::increaseInteractiveCount(GtkWidget * w,
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
SoGtkFullViewerP::decreaseInteractiveCount(GtkWidget * w,
                                           gpointer closure)
{
  SoGtkFullViewer *viewer = (SoGtkFullViewer*) closure;
  viewer->interactiveCountDec();
}

// *************************************************************************

void
SoGtkFullViewerP::interactbuttonClickedCB(GtkWidget *w, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;

  if (viewer->isViewing())
    viewer->setViewing(FALSE);
}

void
SoGtkFullViewerP::viewbuttonClickedCB(GtkWidget * w, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  if (!viewer->isViewing()) 
    viewer->setViewing(TRUE);
}

void
SoGtkFullViewerP::helpbuttonClickedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  PRIVATE(viewer)->helpbuttonClicked();
}

void
SoGtkFullViewerP::homebuttonClickedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  PRIVATE(viewer)->homebuttonClicked();
}

void
SoGtkFullViewerP::sethomebuttonClickedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  PRIVATE(viewer)->sethomebuttonClicked();
}

void
SoGtkFullViewerP::viewallbuttonClickedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  PRIVATE(viewer)->viewallbuttonClicked();
}

void
SoGtkFullViewerP::seekbuttonClickedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * const viewer = (SoGtkFullViewer *) closure;
  PRIVATE(viewer)->seekbuttonClicked();
}

// *************************************************************************

void
SoGtkFullViewerP::leftwheelPressedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelStart();
}

void
SoGtkFullViewerP::leftwheelMovedCB(GtkWidget * wheel, gpointer closure)
{
  assert(closure != NULL);
  gfloat value = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelMotion(value);
}

void
SoGtkFullViewerP::leftwheelReleasedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->leftWheelFinish();
}

void
SoGtkFullViewerP::bottomwheelPressedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelStart();
}

void
SoGtkFullViewerP::bottomwheelMovedCB(GtkWidget * wheel, gpointer closure)
{
  assert(closure != NULL);
  gfloat value = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelMotion(value);
}

void
SoGtkFullViewerP::bottomwheelReleasedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->bottomWheelFinish();
}

void
SoGtkFullViewerP::rightwheelPressedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelStart();
}

void
SoGtkFullViewerP::rightwheelMovedCB(GtkWidget * wheel, gpointer closure)
{
  assert(closure != NULL);
  gfloat value = gtk_thumbwheel_get_value(GTK_THUMBWHEEL(wheel));
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelMotion(value);
}

void
SoGtkFullViewerP::rightwheelReleasedCB(GtkWidget *, gpointer closure)
{
  assert(closure != NULL);
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) closure;
  viewer->rightWheelFinish();
}

void
SoGtkFullViewerP::setThumbWheelValue(void * wheel, float val)
{
  gtk_thumbwheel_set_value(GTK_THUMBWHEEL(wheel), val);
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
