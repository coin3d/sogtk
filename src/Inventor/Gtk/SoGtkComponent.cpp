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
 *  LGPL, please contact SIM to aquire a Professional Edition License.
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
#endif // HAVE_CONFIG_H

#include <stdio.h>
#include <string.h>

#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/SoGtkComponent.h>
#include <Inventor/Gtk/SoGtkCursor.h>
#include <Inventor/Gtk/SoGtkGLWidget.h>
#include <Inventor/Gtk/SoGtkGraphEditor.h>
#include <Inventor/Gtk/SoGtkMaterialList.h>
#include <Inventor/Gtk/SoGtkRenderArea.h>
#include <Inventor/Gtk/SoGtkRoster.h>
#include <Inventor/Gtk/viewers/SoGtkConstrainedViewer.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>
#include <Inventor/Gtk/viewers/SoGtkFlyViewer.h>
#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>
#include <Inventor/Gtk/viewers/SoGtkViewer.h>
#include <Inventor/Gtk/viewers/SoGtkWalkViewer.h>
#include <Inventor/errors/SoDebugError.h>
#include <sogtkdefs.h>

// *************************************************************************

/*!
  \class SoGtkComponent SoGtkComponent.h Inventor/Gtk/SoGtkComponent.h
  \brief The SoGtkComponent class is the base class for all SoGtk GUI
  components.
*/

// *************************************************************************

// "Hidden" class for storing private data (to avoid cluttering up the
// header file, and to make it easier to keep ABI compatibility upon
// changes.)
class SoGtkComponentP {
public:
  SoGtkComponentP(SoGtkComponent * publ);
  ~SoGtkComponentP(void);

  static gint realizeHandlerCB(GtkObject * object, gpointer closure);
  static GdkCursor * getNativeCursor(GtkWidget * w,
                                     const SoGtkCursor::CustomCursor * cc);

  GtkWidget * widget;
  GtkWidget * parent;
  SbBool embedded;
  SbBool fullscreen;
  SbBool shelled;
  char * className;
  char * widgetName;
  char * captionText;
  char * iconText;
  SoGtkComponentCB * closeCB;
  void * closeCBdata;
  SbPList * visibilityChangeCBs;
  SbVec2s storeSize;

  static GdkCursor * arrowcursor;
  static GdkCursor * crosscursor;
  static GdkCursor * uparrowcursor;

  struct NonFull {
    gint x, y, w, h;
  };
  NonFull nonfull;

private:
  SoGtkComponent * pub;
  static SbDict * cursordict;
};

GdkCursor * SoGtkComponentP::arrowcursor = NULL;
GdkCursor * SoGtkComponentP::crosscursor = NULL;
GdkCursor * SoGtkComponentP::uparrowcursor = NULL;
SbDict * SoGtkComponentP::cursordict = NULL;

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

// *************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkComponent);

/*!
  \internal
  This method initializes the type system for all the component classes.
*/
void
SoGtkComponent::initClasses(void)
{
  SoGtkComponent::initClass();
  SoGtkGLWidget::initClass();
  SoGtkRenderArea::initClass();
  SoGtkViewer::initClass();
  SoGtkFullViewer::initClass();
  SoGtkExaminerViewer::initClass();
  SoGtkPlaneViewer::initClass();
  SoGtkConstrainedViewer::initClass();
  SoGtkWalkViewer::initClass();
  SoGtkFlyViewer::initClass();
  SoGtkMaterialList::initClass();
  SoGtkGraphEditor::initClass();
  SoGtkRoster::initClass();
} // initClasses()

// *************************************************************************

/*!
  Constructor.

  \a parent is the widget we'll build this component inside.  If \a
  parent is \a NULL, make a new toplevel window.

  \a name is mostly interesting for debugging purposes.

  \a embed specifies whether or not we should make a new toplevel
  window for the component even when we've got a non-NULL \a parent.
*/
SoGtkComponent::SoGtkComponent(GtkWidget * const parent,
                               const char * const name,
                               const SbBool embed)
{
  PRIVATE(this) = new SoGtkComponentP(this);

  PRIVATE(this)->parent = parent;

  if (name) {
    PRIVATE(this)->widgetName = strcpy(new char [strlen(name)+1], name);
  }

  this->setClassName("SoGtkComponent");

  if (parent == NULL || ! embed) {
    PRIVATE(this)->parent = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    PRIVATE(this)->shelled = TRUE;
    PRIVATE(this)->embedded = FALSE;
  }
  else if (parent == SoGtk::getTopLevelWidget()) {
    PRIVATE(this)->embedded = FALSE; // what if...?
  }
  else {
    PRIVATE(this)->embedded = TRUE;
  }

  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->parent), "event",
                     GTK_SIGNAL_FUNC(SoGtkComponent::eventHandler),
                     (gpointer) this);

  gtk_idle_add((GtkFunction) SoGtk::componentCreation, (gpointer) this);
}

/*!
  Destructor.
*/
SoGtkComponent::~SoGtkComponent()
{
  SoGtk::componentDestruction(this);

  // FIXME: this is tmp disabled as it causes a
  // segfault. Investigate. 20020107 mortene.
#if 0
  if (PRIVATE(this)->widget) { gtk_widget_destroy(PRIVATE(this)->widget); }
#endif

  delete PRIVATE(this);
}

// *************************************************************************

/*!
  Add a callback which will be called whenever the widget component
  changes visibility status (because of iconification or
  deiconification, for instance).

  \sa removeVisibilityChangeCallback(), isVisible()
*/
void
SoGtkComponent::addVisibilityChangeCallback(SoGtkComponentVisibilityCB * func,
                                            void * userData)
{
  if (! PRIVATE(this)->visibilityChangeCBs) {
    PRIVATE(this)->visibilityChangeCBs = new SbPList;
  }
  PRIVATE(this)->visibilityChangeCBs->append((void *) func);
  PRIVATE(this)->visibilityChangeCBs->append(userData);
}

/*!
  Remove one of the callbacks from the list of visibility notification
  callbacks.

  \sa addVisibilityChangeCallback(), isVisible()
*/
void
SoGtkComponent::removeVisibilityChangeCallback(SoGtkComponentVisibilityCB * func,
                                               void * userData)
{
#if SOGTK_DEBUG
  if (! PRIVATE(this)->visibilityChangeCBs) {
    SoDebugError::postWarning("SoGtkComponent::removeVisibilityChangeCallback",
                              "empty callback list");
    return;
  }
#endif // SOGTK_DEBUG

  int idx = PRIVATE(this)->visibilityChangeCBs->find((void *)func);
  if (idx != -1) {
    PRIVATE(this)->visibilityChangeCBs->remove(idx);
    PRIVATE(this)->visibilityChangeCBs->remove(idx);
  }

#if SOGTK_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoGtkComponent::removeVisibilityChangeCallback",
                              "tried to remove non-existant callback");
    return;
  }
#endif // SOGTK_DEBUG
}

// *************************************************************************

/*!
  Set class name of widget.

  \sa getClassName(), componentClassName()
*/
void
SoGtkComponent::setClassName(const char * const name)
{
  if (PRIVATE(this)->className) {
    delete [] PRIVATE(this)->className;
    PRIVATE(this)->className = NULL;
  }
  if (name) {
    PRIVATE(this)->className = strcpy(new char [strlen(name)+1], name);
  }
}

// *************************************************************************

/*!
  Set the core widget for this SoGtk component. It is important that
  subclasses get this correct, as the widget set here will be the
  widget returned from query methods.

  \sa baseWidget()
*/
void
SoGtkComponent::setBaseWidget(GtkWidget * widget)
{
  if (PRIVATE(this)->widget) {
    gtk_signal_disconnect_by_func(GTK_OBJECT(PRIVATE(this)->widget),
                                  GTK_SIGNAL_FUNC(SoGtkComponentP::realizeHandlerCB),
                                  (gpointer) this);
  }

  PRIVATE(this)->widget = NULL;

  if (! GTK_IS_WIDGET(widget)) {
#if SOGTK_DEBUG
    SoDebugError::postInfo("SoGtkComponent::setBaseWidget", "not a widget");
#endif
    return;
  }

  PRIVATE(this)->widget = widget;

  if (PRIVATE(this)->widget) {
    gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->widget), "realize",
                       GTK_SIGNAL_FUNC(SoGtkComponentP::realizeHandlerCB),
                       (gpointer) this);
  }
}

// *************************************************************************

static const char * gdk_event_name(GdkEventType type) {
  switch (type) {
  case GDK_NOTHING:           return "GDK_NOTHING";
  case GDK_DELETE:            return "GDK_DELETE";
  case GDK_DESTROY:           return "GDK_DESTROY";
  case GDK_EXPOSE:            return "GDK_EXPOSE";
  case GDK_MOTION_NOTIFY:     return "GDK_MOTION_NOTIFY";
  case GDK_BUTTON_PRESS:      return "GDK_BUTTON_PRESS";
  case GDK_2BUTTON_PRESS:     return "GDK_2BUTTON_PRESS";
  case GDK_3BUTTON_PRESS:     return "GDK_3BUTTON_PRESS";
  case GDK_BUTTON_RELEASE:    return "GDK_BUTTON_RELEASE";
  case GDK_KEY_PRESS:         return "GDK_KEY_PRESS";
  case GDK_KEY_RELEASE:       return "GDK_KEY_RELEASE";
  case GDK_ENTER_NOTIFY:      return "GDK_ENTER_NOTIFY";
  case GDK_LEAVE_NOTIFY:      return "GDK_LEAVE_NOTIFY";
  case GDK_FOCUS_CHANGE:      return "GDK_FOCUS_CHANGE";
  case GDK_CONFIGURE:         return "GDK_CONFIGURE";
  case GDK_MAP:               return "GDK_MAP";
  case GDK_UNMAP:             return "GDK_UNMAP";
  case GDK_PROPERTY_NOTIFY:   return "GDK_PROPERTY_NOTIFY";
  case GDK_SELECTION_CLEAR:   return "GDK_SELECTION_CLEAR";
  case GDK_SELECTION_REQUEST: return "GDK_SELECTION_REQUEST";
  case GDK_SELECTION_NOTIFY:  return "GDK_SELECTION_NOTIFY";
  case GDK_PROXIMITY_IN:      return "GDK_PROXIMITY_IN";
  case GDK_PROXIMITY_OUT:     return "GDK_PROXIMITY_OUT";
  case GDK_DRAG_ENTER:        return "GDK_DRAG_ENTER";
  case GDK_DRAG_LEAVE:        return "GDK_DRAG_LEAVE";
  case GDK_DRAG_MOTION:       return "GDK_DRAG_MOTION";
  case GDK_DRAG_STATUS:       return "GDK_DRAG_STATUS";
  case GDK_DROP_START:        return "GDK_DROP_START";
  case GDK_DROP_FINISHED:     return "GDK_DROP_FINISHED";
  case GDK_CLIENT_EVENT:      return "GDK_CLIENT_EVENT";
  case GDK_VISIBILITY_NOTIFY: return "GDK_VISIBILITY_NOTIFY";
  case GDK_NO_EXPOSE:         return "GDK_NO_EXPOSE";
  default:
    break;
  }
  return _("<unknown>");
}

/*!
  \internal

  Helps us detect changes in size (base widget and parent widget) and
  visibility status.
*/
SbBool
SoGtkComponent::eventFilter(GtkWidget * obj, GdkEvent * ev)
{
  assert(GTK_IS_WIDGET(obj) && "something in gtk is fishy");

  GtkWidget * widget = GTK_WIDGET(obj);

  if (widget != this->baseWidget() &&
      !gtk_widget_is_ancestor(GTK_WIDGET(this->baseWidget()),
                              GTK_WIDGET(widget))) {
    return FALSE;
  }

  switch (ev->type) {
  case GDK_KEY_PRESS:
  case GDK_KEY_RELEASE:
    return FALSE;

  case GDK_CONFIGURE:
    if(GTK_WIDGET_REALIZED(this->baseWidget())) {
      GdkEventConfigure * event = (GdkEventConfigure *) ev;
      SbVec2s size(event->width, event->height);
      PRIVATE(this)->storeSize.setValue(event->width, event->height);
      this->sizeChanged(size);
    }
    break;

  case GDK_DELETE:
    if (PRIVATE(this)->closeCB) {
      PRIVATE(this)->closeCB(PRIVATE(this)->closeCBdata, this);
    }
    // FIXME: should we perhaps *not* just go on and quit the
    // application if there's a closeCB installed? Investigate what
    // the original SGI InventorXt does. 20020107 mortene.
    SoGtk::exitMainLoop();
    break;

  default:
    break;
  }

  return TRUE;
}

// Static callback, just forwards control to
// SoGtkComponent::eventFilter() member-function.
gint
SoGtkComponent::eventHandler(GtkWidget * object,
                             GdkEvent * event,
                             gpointer closure)
{
  SoGtkComponent * const component = (SoGtkComponent *) closure;
  component->eventFilter(object, event);
  return FALSE;
}

// *************************************************************************

/*!
  Returns the visibility status of the component. If any parent
  widgets of this component is hidden, returns \c FALSE.

  Note that a widget which is just obscured by other windows on the
  desktop is not hidden in this sense, and \c TRUE will be returned.

  \sa show(), hide()
*/
SbBool
SoGtkComponent::isVisible(void)
{
  if (! this->baseWidget()) { return FALSE; }
  // FIXME - return true visibility state. 200????? larsa.
  // Close, but probably still incomplete
  return GTK_WIDGET_DRAWABLE(this->baseWidget()) ? TRUE : FALSE;
}

/*!
  This will show the widget, deiconifiying and raising it if
  necessary.

  \sa hide(), isVisible()
*/
void
SoGtkComponent::show(void)
{
  if(! this->baseWidget() || ! GTK_IS_WIDGET(this->baseWidget())) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkComponent::show",
                              this->baseWidget() ?
                              "not a widget" : "no widget");
#endif // SOGTK_DEBUG
    return;
  }
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * widget = this->baseWidget();
  assert(parent != NULL);
  assert(widget != NULL);

  if (! widget->parent)
    gtk_container_add(GTK_CONTAINER(parent), widget);

  if (PRIVATE(this)->storeSize != SbVec2s(-1, -1) && GTK_IS_WINDOW(parent)) {
    gtk_window_set_default_size(GTK_WINDOW(parent),
                                PRIVATE(this)->storeSize[0],
                                PRIVATE(this)->storeSize[1]);
  }

  if (PRIVATE(this)->shelled) {
    if (! GTK_WIDGET_REALIZED(this->baseWidget())) {
      gtk_widget_show(widget);
    }
    gtk_widget_show(parent);
  }
  else {
    gtk_widget_show(widget);
  }

  this->sizeChanged(PRIVATE(this)->storeSize);
}

/*!
  This will hide the component.

  \sa show(), isVisible()
*/
void
SoGtkComponent::hide(void)
{
#if SOGTK_DEBUG
  if (! this->baseWidget() || ! GTK_IS_WIDGET(this->baseWidget())) {
    SoDebugError::postWarning("SoGtkComponent::hide",
                              this->baseWidget() ?
                              "not a widget" : "no widget");
    return;
  }
#endif // SOGTK_DEBUG
  if (PRIVATE(this)->shelled) {
    gtk_widget_hide(GTK_WIDGET(this->getParentWidget()));
  }
  else {
    gtk_widget_hide(GTK_WIDGET(this->baseWidget()));
  }
}

// *************************************************************************

/*!
  Returns a pointer to the component's window system widget.

  \sa getShellWidget(), getParentWidget()
*/
GtkWidget *
SoGtkComponent::getWidget(void) const
{
  return PRIVATE(this)->widget;
}

/*!
  An SoGtkComponent may be composed of any number of widgets in
  parent-children relationships in a tree structure with any depth.
  This method will return the root widget in that tree.

  \sa setBaseWidget()
*/
GtkWidget *
SoGtkComponent::baseWidget(void) const
{
  return PRIVATE(this)->widget;
}

// *************************************************************************

/*!
  Returns \c TRUE if this component is a toplevel shell, i.e. it has a
  window representation on the desktop.

  \sa getShellWidget()
*/
SbBool
SoGtkComponent::isTopLevelShell(void) const
{
#if SOGTK_DEBUG
  if(! PRIVATE(this)->widget) {
    SoDebugError::postWarning("SoGtkComponent::isTopLevelShell",
                              "Called while no GtkWidget has been set.");
    return FALSE;
  }
#endif // SOGTK_DEBUG

  if (PRIVATE(this)->widget->parent == 0)
  {
    // FIXME: Dunno if this can happen. 200????? larsa.
    SoDebugError::postWarning("SoGtkComponent::isTopLevelShell",
                              "No parent.");
  }
  else {
    if (PRIVATE(this)->widget->parent == gtk_widget_get_toplevel(PRIVATE(this)->widget)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  return FALSE;
}

// *************************************************************************

/*!
  Returns the toplevel shell widget for this component. The toplevel
  shell is the desktop window which contains the component.

  \sa isTopLevelShell(), getWidget()
*/
GtkWidget *
SoGtkComponent::getShellWidget(void) const
{
  if (! PRIVATE(this)->widget) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkComponent::getShellWidget", "no widget");
#endif // SOGTK_DEBUG
    return NULL;
  }
  return PRIVATE(this)->widget; // FIXME
}

/*!
  Returns the widget which is the parent (i.e. contains) this
  component's base widget.

  \sa getWidget(), baseWidget(), isTopLevelShell()
*/
GtkWidget *
SoGtkComponent::getParentWidget(void) const
{
  return PRIVATE(this)->parent;
}

// *************************************************************************

/*!
  Set the window title of this component. This will not work unless
  the component is a toplevel shell.

  \sa getTitle(), setIconTitle(), isTopLevelShell()
*/

void
SoGtkComponent::setTitle(const char * const title)
{
  if (PRIVATE(this)->captionText) {
    delete [] PRIVATE(this)->captionText;
    PRIVATE(this)->captionText = NULL;
  }
  if (title) {
    PRIVATE(this)->captionText = strcpy(new char [strlen(title)+1], title);
  }
  if (! PRIVATE(this)->embedded) {
    gtk_window_set_title(GTK_WINDOW(PRIVATE(this)->parent), title ? title : "");
  }
}

/*!
  Returns the window title. The component should be a toplevel shell
  if you call this method.

  \sa setTitle(), isTopLevelShell()
*/
const char *
SoGtkComponent::getTitle(void) const
{
  if (PRIVATE(this)->captionText == NULL) {
    return this->getDefaultTitle();
  }
  return PRIVATE(this)->captionText;
}

// *************************************************************************

/*!
  Sets the window's title when it is iconfied. The component you use
  this method on should be a toplevel shell.

  \sa getIconTitle(), setTitle(), isTopLevelShell()
*/
void
SoGtkComponent::setIconTitle(const char * const title)
{
  if (PRIVATE(this)->iconText) {
    delete [] PRIVATE(this)->iconText;
    PRIVATE(this)->iconText = NULL;
  }
  if (title) {
    PRIVATE(this)->iconText = strcpy(new char [strlen(title)+1], title);
  }
  if (PRIVATE(this)->widget) {
    GtkWidget * window = gtk_widget_get_toplevel(PRIVATE(this)->widget);
    assert(window != NULL);
    gdk_window_set_icon_name((GTK_WIDGET(PRIVATE(this)->parent))->window, title ? title : "");
#if 0
    gdk_window_set_icon_name(window->window, title ? title : "");
#endif
  }
}

/*!
  Returns the title the window has when iconfied. The component should
  be a toplevel shell if you use this method.

  \sa setIconTitle(), isTopLevelShell()
*/
const char *
SoGtkComponent::getIconTitle(void) const
{
  return PRIVATE(this)->iconText;
}

// *************************************************************************

/*!
  Returns name of the widget.
*/
const char *
SoGtkComponent::getWidgetName(void) const
{
  return PRIVATE(this)->widgetName;
}

// *************************************************************************

/*!
  Returns class name of widget.
*/
const char *
SoGtkComponent::getClassName(void) const
{
  return (const char *) PRIVATE(this)->className;
}

// *************************************************************************

/*!
  Returns the default name of an SoGtkComponent widget. Should be
  overloaded by subclasses.
*/
const char *
SoGtkComponent::getDefaultWidgetName(void) const
{
  static char defaultWidgetName[] = "SoGtkComponent";
  return defaultWidgetName;
}

/*!
  Returns the default window caption string of this component. Should
  be overloaded by subclasses.
*/
const char *
SoGtkComponent::getDefaultTitle(void) const
{
  static char defaultTitle[] = N_("SoGtk Component");
  return _(defaultTitle);
}

/*!
  Returns the default icon title of this component. Should be
  overloaded by subclasses.
*/
const char *
SoGtkComponent::getDefaultIconTitle(void) const
{
  static char defaultIconTitle[] = N_("Component");
  return _(defaultIconTitle);
}

// *************************************************************************

/*!
  Resize the component widget.

  \sa getSize()
*/
void
SoGtkComponent::setSize(const SbVec2s size)
{
  if (size[0] <= 0 || size[1] <= 0) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkComponent::setSize",
                              "Invalid size setting: <%d, %d>.",
                              size[0], size[1]);
#endif // SOGTK_DEBUG
    return;
  }

  if (! PRIVATE(this)->embedded) {
    if (PRIVATE(this)->parent) {
      GtkRequisition req = { size[0], size[1] };
      gtk_widget_size_request(GTK_WIDGET(PRIVATE(this)->parent), &req);
    }
  }
  else if (PRIVATE(this)->widget) {
    GtkRequisition req = { size[0], size[1] };
    gtk_widget_size_request(GTK_WIDGET(PRIVATE(this)->widget), &req);
  }

  PRIVATE(this)->storeSize = size;
  this->sizeChanged(size);
}

/*!
  Returns the component widget size.

  \sa setSize()
*/
SbVec2s
SoGtkComponent::getSize(void) const
{
  return PRIVATE(this)->storeSize;
}

// *************************************************************************

/*!
  Override to detect when the base widget in the component changes its
  dimensions (an operation which is usually triggered by the user).
*/
void
SoGtkComponent::sizeChanged(const SbVec2s & size)
{
  // nada
}

// *************************************************************************

/*!
  Open a dialog providing help about use of this component.

  NB: no help system has been implemented yet, so for the time being
  this will only pop up an error message.
*/
void
SoGtkComponent::openHelpCard(const char * card)
{
  SoGtk::createSimpleErrorDialog(PRIVATE(this)->widget, _("Not Implemented"),
                                 _("The help card system has not been implemented yet."));
}

// *************************************************************************

/*!
  Set up a callback function to use when the component gets closed. A
  component must be a toplevel shell for this to have any effect.

  For toplevel shells with no close callback set, the window will
  simply be hidden. The typical action to take in the callback would
  be to delete the component.

  \sa isTopLevelShell()
*/
void
SoGtkComponent::setWindowCloseCallback(SoGtkComponentCB * func,
                                       void * const data)
{
  // FIXME: make list instead of one slot that is overwritten?
  // 200????? larsa.

  PRIVATE(this)->closeCB = func;
  PRIVATE(this)->closeCBdata = data;
}

// *************************************************************************

/*!
  Finds and returns the SoGtkComponent corresponding to the given
  GtkWidget, if any. If no SoGtkComponent is the "owner" of the
  widget, \c NULL will be returned.
*/

SoGtkComponent *
SoGtkComponent::getComponent(GtkWidget * const widget)
{
  SOGTK_STUB();

//  for (int i = 0; i < SoGtkComponent::soGtkCompList->getLength(); i++) {
//    SoGtkComponent * c = (SoGtkComponent *)((*SoGtkComponent::soGtkCompList)[i]);
//    if (c->getWidget() == widget) return c;
//  }

  return NULL;
}

// *************************************************************************

/*!
  FIXME: write doc
*/
void
SoGtkComponent::afterRealizeHook(void)
{
#if SOGTK_DEBUG && 0
  SoDebugError::postInfo("SoGtkComponent::afterRealizeHook", "[invoked]");
#endif
  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->widget), "event",
                     GTK_SIGNAL_FUNC(SoGtkComponent::eventHandler),
                     (gpointer) this);
  gtk_window_set_title(GTK_WINDOW(PRIVATE(this)->parent), this->getTitle());
}

// *************************************************************************
//
//  Private implementation
//

/*
  initialize private memory
*/

SoGtkComponentP::SoGtkComponentP(SoGtkComponent * publ)
{
  this->pub = publ;

  this->widget = NULL;
  this->parent = NULL;
  this->embedded = FALSE;
  this->fullscreen = FALSE;
  this->shelled = FALSE;
  this->widget = NULL;
  this->closeCB = NULL;
  this->closeCBdata = NULL;
  this->visibilityChangeCBs = NULL;
  this->storeSize.setValue(-1, -1);

  this->className = NULL;
  this->widgetName = NULL;
  this->captionText = NULL;
  this->iconText = NULL;
}

/*
  free privately allocated memory
*/
SoGtkComponentP::~SoGtkComponentP()
{
  delete this->visibilityChangeCBs;
  delete [] this->widgetName;
  delete [] this->className;
  delete [] this->captionText;
  delete [] this->iconText;
}

// *************************************************************************

/*
  event handler for realize events, used for invoking afterRealizeHook()
*/
gint
SoGtkComponentP::realizeHandlerCB(GtkObject * object,
                                  gpointer closure)
{
  assert(closure != NULL);
  SoGtkComponent * const component = (SoGtkComponent *) closure;
  GtkWidget * widget = component->baseWidget();
  assert(GTK_WIDGET_REALIZED(widget));
  if (PRIVATE(component)->storeSize != SbVec2s(-1, -1)) {
    GtkRequisition req = {
      PRIVATE(component)->storeSize[0],
      PRIVATE(component)->storeSize[1] };
    gtk_widget_size_request(widget, &req);
  }
  SbVec2s size(widget->allocation.width, widget->allocation.height);
  component->sizeChanged(size);
  component->afterRealizeHook();
  return FALSE;
}

// *************************************************************************

/*!
  Toggle full screen mode for this component, if possible.

  Returns \c FALSE if operation failed.  This might happen if the
  toolkit doesn't support attempts at making the component cover the
  complete screen or if the component is not a toplevel window.
*/
SbBool
SoGtkComponent::setFullScreen(const SbBool onoff)
{
  if (onoff == PRIVATE(this)->fullscreen) { return TRUE; }

  GtkWidget * w = SoGtk::getTopLevelWidget();
  if (!GTK_WIDGET_REALIZED(GTK_WIDGET(w))) {
    gtk_widget_realize(GTK_WIDGET(w));
  }

  GdkWindow * gdk_window = GTK_WIDGET(w)->window;

  if (onoff) {
    // Store current window position and geometry for later resetting.
    { 
      // FIXME: does this work as expected if setFullScreen() is
      // called before the window has been realized?
      // Investigate. 20020107 mortene.
      gdk_window_get_root_origin(gdk_window,
                                 &PRIVATE(this)->nonfull.x,
                                 &PRIVATE(this)->nonfull.y);
      gdk_window_get_size(gdk_window,
                          &PRIVATE(this)->nonfull.w,
                          &PRIVATE(this)->nonfull.h);
    }

#ifdef HAVE_XINERAMA
    // FIXME: from the galeon sourcecode, we've gleamed that if the
    // user has Xinerama (ie multi-headed X11 server) installed, there
    // need to be some code here to (quote:) "adjust the area and
    // positioning of the fullscreen window so it's not occupying any
    // dead area or covering all of the heads".
    //
    // Can't fix and test properly before we lay our hands on a
    // multi-head display system.
    //
    // (Note that the HAVE_XINERAMA define is just a dummy at the
    // moment, there's no configure-check for it yet.)
    // 
    // 20020107 mortene.
#endif // HAVE_XINERAMA

    gdk_window_hide(gdk_window);
    gdk_window_set_decorations(gdk_window, (GdkWMDecoration) 0);
    gdk_window_show(gdk_window);
    gdk_window_move_resize(gdk_window, 0, 0,
                           gdk_screen_width(), gdk_screen_height());
  }
  else {
    gdk_window_hide(gdk_window);
    gdk_window_set_decorations(gdk_window, GDK_DECOR_ALL);
    gdk_window_show(gdk_window);

    // Restore initial position and size.
    gdk_window_move_resize(gdk_window,
                           PRIVATE(this)->nonfull.x,
                           PRIVATE(this)->nonfull.y,
                           PRIVATE(this)->nonfull.w,
                           PRIVATE(this)->nonfull.h);
  }

  // FIXME: is this really necessary? 20020107 mortene.
  PRIVATE(this)->storeSize.setValue(PRIVATE(this)->nonfull.w,
                                    PRIVATE(this)->nonfull.h);

  PRIVATE(this)->fullscreen = onoff;
  return TRUE;
}

/*!
  Returns flag indicated whether or not this widget / component is
  currently in full screen mode.
*/
SbBool
SoGtkComponent::isFullScreen(void) const
{
  return PRIVATE(this)->fullscreen;
}

// Converts from the common generic cursor format to a Win32 HCURSOR
// instance.
GdkCursor *
SoGtkComponentP::getNativeCursor(GtkWidget * w,
                                 const SoGtkCursor::CustomCursor * cc)
{
  if (SoGtkComponentP::cursordict == NULL) { // first call, initialize
    SoGtkComponentP::cursordict = new SbDict; // FIXME: mem leak. 20011121 mortene.
  }

  void * qc;
  SbBool b = SoGtkComponentP::cursordict->find((unsigned long)cc, qc);
  if (b) { return (GdkCursor *)qc; }

  GtkStyle * style = w->style;
  GdkColor fg = style->black;
  GdkColor bg = style->white;

  GdkPixmap * bitmap =
    gdk_bitmap_create_from_data(NULL, (const gchar *)cc->bitmap,
                                cc->dim[0], cc->dim[1]);
  GdkPixmap *mask =
    gdk_bitmap_create_from_data(NULL, (const gchar *)cc->mask,
                                cc->dim[0], cc->dim[1]);

  // FIXME: plug memleak. 20011126 mortene.
  GdkCursor * cursor =
    gdk_cursor_new_from_pixmap(bitmap, mask, &fg, &bg,
                               cc->hotspot[0], cc->hotspot[1]);
  gdk_pixmap_unref(bitmap);
  gdk_pixmap_unref(mask);

  SoGtkComponentP::cursordict->enter((unsigned long)cc, cursor);
  return cursor;
}

/*!
  Sets the cursor for this component.
*/
void
SoGtkComponent::setComponentCursor(const SoGtkCursor & cursor)
{
  SoGtkComponent::setWidgetCursor(this->getWidget(), cursor);
}

/*!
  Set cursor for a native widget in the underlying toolkit.
*/
void
SoGtkComponent::setWidgetCursor(GtkWidget * w, const SoGtkCursor & cursor)
{

  if (GTK_WIDGET_NO_WINDOW(w)) {
    if (SOGTK_DEBUG) {
      // FIXME: This should not happen, but there seems to be a bug in
      // SoGtk's event handling causing this. 20001219 RC.
      static SbBool first = TRUE;
      if (first) {
        SoDebugError::postWarning("SoGtkComponent::setWidgetCursor",
                                  "widget %x: NO WINDOW\n", (int) w);
        first = FALSE;
      }
    }
    return;
  }

  if (w->window == (GdkWindow *)NULL) {
    if (SOGTK_DEBUG) {
      // FIXME: This should not happen, but there seems to be a bug in
      // SoGtk's event handling causing this. 20001219 RC.
      static SbBool first = TRUE;
      if (first) {
        SoDebugError::postWarning("SoGtkComponent::setWidgetCursor",
                                  "widget %x: widget->window == 0\n", (int) w);
        first = FALSE;
      }
    }
    return;
  }

  if (cursor.getShape() == SoGtkCursor::CUSTOM_BITMAP) {
    const SoGtkCursor::CustomCursor * cc = &cursor.getCustomCursor();
    gdk_window_set_cursor(w->window, SoGtkComponentP::getNativeCursor(w, cc));
  }
  else {
    switch (cursor.getShape()) {
    case SoGtkCursor::DEFAULT:
      if (!SoGtkComponentP::arrowcursor) {
        // FIXME: plug memleak with gdk_cursor_destroy(). 20011126 mortene.
        SoGtkComponentP::arrowcursor = gdk_cursor_new(GDK_TOP_LEFT_ARROW);
      }
      gdk_window_set_cursor(w->window, SoGtkComponentP::arrowcursor);
      break;

    case SoGtkCursor::BUSY:
      SOGTK_STUB();
      break;

    case SoGtkCursor::CROSSHAIR:
      if (!SoGtkComponentP::crosscursor) {
        // FIXME: plug memleak. 20011126 mortene.
        SoGtkComponentP::crosscursor = gdk_cursor_new(GDK_CROSSHAIR);
      }
      gdk_window_set_cursor(w->window, SoGtkComponentP::crosscursor);
      break;

    case SoGtkCursor::UPARROW:
      if (!SoGtkComponentP::uparrowcursor) {
        // FIXME: plug memleak. 20011126 mortene.
        SoGtkComponentP::uparrowcursor = gdk_cursor_new(GDK_SB_UP_ARROW);
      }
      gdk_window_set_cursor(w->window, SoGtkComponentP::uparrowcursor);
      break;

    default:
      assert(FALSE && "unsupported cursor shape type");
      break;
    }
  }
}

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkComponentRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG
