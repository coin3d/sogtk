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

#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG

#include <Inventor/Gtk/SoGtkComponent.h>

// *************************************************************************

/*!
  \class SoGtkComponent SoGtkComponent.h Inventor/Gtk/SoGtkComponent.h
  \brief The SoGtkComponent class is the base class for all GUI components.
*/

// *************************************************************************

/*!
  \fn virtual const char * SoGtkComponent::componentClassName(void) const

  Returns class name of the component.
*/

/*!
  \fn virtual void SoGtkComponent::sizeChanged(const SbVec2s size)

  Overload to detect when the base widget in the component changes its
  dimensions (an operation which is usually triggered by the user).
*/

/*!
  \var GtkWidget * SoGtkComponent::widget

  Pointer to the Gtk widget. This member must be set from all component
  classes which inherits SoGtkComponent directly.
*/

// *************************************************************************

SbPList SoGtkComponent::soGtkCompList;
SbPList SoGtkComponent::gtkWidgetList;

// *************************************************************************

/*!
  Constructor.

  \a parent is the widget we'll build this component inside. If \a parent is
  \a NULL, make a new toplevel window.

  \a name is mostly interesting for debugging purposes.

  \a buildInsideParent specifies whether or not we should make a new
  toplevel window for the component even when we've got a non-NULL
  \a parent.
*/

SoGtkComponent::SoGtkComponent(
  GtkWidget * const parent,
  const char * const name,
  const SbBool buildInsideParent )
{
  this->parent = parent;
  this->widget = NULL;
  this->closeCB = NULL;
  this->closeCBdata = NULL;
  this->visibilityChangeCBs = NULL;

  if ( name ) this->widgetName = name;
  this->className = "SoGtkComponent";

  this->storeSize.setValue(-1, -1);

  this->buildInside = buildInsideParent;
} // SoGtkComponent()

// *************************************************************************

/*!
  Destructor.
*/

SoGtkComponent::~SoGtkComponent( // virtual
  void )
{
  // Link us out of the static QWidget<->SoGtkComponent "correlation"
  // lists.
  int idx = SoGtkComponent::gtkWidgetList.find( this->widget );
  assert(idx != -1);
  SoGtkComponent::gtkWidgetList.remove(idx);
  SoGtkComponent::soGtkCompList.remove(idx);

  delete this->visibilityChangeCBs;

  // If we've got a toplevel widget on our hands it won't
  // automatically be deallocated (there's no real parent widget).
//  if (this->widget && ! this->widget->parentWidget()) delete this->widget;
} // ~SoGtkComponent()

// *************************************************************************

/*!
  Add a callback which will be called whenever the widget component
  changes visibility status (because of iconification or
  deiconification, for instance).

  \sa removeVisibilityChangeCallback(), isVisible()
*/

void
SoGtkComponent::addVisibilityChangeCallback(
  SoGtkComponentVisibilityCB * func,
  void * userData )
{
  if (!this->visibilityChangeCBs) this->visibilityChangeCBs = new SbPList;
  this->visibilityChangeCBs->append((void *)func);
  this->visibilityChangeCBs->append(userData);
} // addVisibilityCallback()

// *************************************************************************

/*!
  Remove one of the callbacks from the list of visibility notification
  callbacks.

  \sa addVisibilityChangeCallback(), isVisible()
*/

void
SoGtkComponent::removeVisibilityChangeCallback(
  SoGtkComponentVisibilityCB * func,
  void * userData )
{
#if SOGTK_DEBUG
  if (!this->visibilityChangeCBs) {
    SoDebugError::postWarning("SoGtkComponent::removeVisibilityChangeCallback",
                              "empty callback list");
    return;
  }
#endif // SOGTK_DEBUG

  int idx = this->visibilityChangeCBs->find((void *)func);
  if (idx != -1 ) {
    this->visibilityChangeCBs->remove(idx);
    this->visibilityChangeCBs->remove(idx);
  }

#if SOGTK_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning(
      "SoGtkComponent::removeVisibilityChangeCallback",
      "tried to remove non-existant callback" );
    return;
  }
#endif // SOGTK_DEBUG
} // removeVisibilityCallback()

// *************************************************************************

/*!
  Set class name of widget.

  \sa getClassName(), componentClassName()
*/

void
SoGtkComponent::setClassName(
  const char * const name )
{
  this->className = name;
} // setClassName()

// *************************************************************************

/*!
  Set the core widget for this SoGtk component. It is important that
  subclasses get this correct, as the widget set here will be the widget
  returned from query methods.

  \sa baseWidget()
*/

void
SoGtkComponent::setBaseWidget(
  GtkWidget * widget )
{
  assert(widget);

//  if (this->parent) this->parent->removeEventFilter(this);
//  if (this->widget) this->widget->removeEventFilter(this);

  this->widget = widget;
//  this->parent = w->parentWidget();

#if 0 // debug
  SoDebugError::postInfo("SoGtkComponent::setBaseWidget",
                         "widget: %p, parent: %p", widget, this->parent);
#endif // debug


#if 0 // debug
  if (!this->captiontext.isNull()) {
    SoDebugError::postInfo("SoGtkComponent::setBaseWidget",
                           "setCaption('%s')",
                           (const char *)this->captiontext);
  }
#endif // debug
//  if (this->captiontext.isNull()) this->captiontext = this->getDefaultTitle();
//  this->getShellWidget()->setCaption(this->captiontext);

//  if (this->icontext.isNull()) this->icontext = this->getDefaultIconTitle();
//  this->getShellWidget()->setIconText(this->icontext);

//  if (this->widgetname.isNull())
//    this->widgetname = this->getDefaultWidgetName();
//  this->widget->setName(this->widgetname);

  // Need this to auto-detect resize events.
//  if (this->parent) this->parent->installEventFilter(this);
//  this->widget->installEventFilter(this);
#if 0 // debug
  SoDebugError::postInfo("SoGtkComponent::setBaseWidget",
                         "installeventfilter, widget: %p", this->widget);
#endif // debug
} // setBaseWidget()

// *************************************************************************

/*!
  \internal

  Helps us detect changes in size (base widget and parent widget)
  and visibility status.
*/

SbBool
SoGtkComponent::eventFilter( // virtual
  GtkObject * object,
  GdkEvent * event )
{
/*
#if 0 // debug
  SoDebugError::postInfo("SoGtkComponent::eventFilter", "obj: %p", obj);
#endif // debug

#if 0 // debug
  switch (e->type()) {
  case Event_MouseButtonPress:
//      SoDebugError::postInfo("SoGtkComponent::eventFilter", "button press");
    break;
  case Event_MouseButtonRelease:
//      SoDebugError::postInfo("SoGtkComponent::eventFilter", "button release");
    break;
  case Event_MouseButtonDblClick:
//      SoDebugError::postInfo("SoGtkComponent::eventFilter", "dbl click");
    break;
  case Event_MouseMove:
//      SoDebugError::postInfo("SoGtkComponent::eventFilter", "mousemove");
    break;
  case Event_Paint:
    SoDebugError::postInfo("SoGtkComponent::eventFilter", "paint");
    break;
  case Event_Resize:
    SoDebugError::postInfo("SoGtkComponent::eventFilter", "resize");
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
    SoDebugError::postInfo("SoGtkComponent::eventFilter", "type %d", e->type());
    break;
  }
#endif // debug

  // Remove event filter if unknown Gtk widget.
  // FIXME: this code should probably be superfluous if everything
  // else works 100%? 990216 mortene.
  if ((obj != this->widget) && (obj != this->parent)) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkComponent::eventFilter",
                              "had to do a removeEventFilter()");
#endif // SOGTK_DEBUG
    obj->removeEventFilter(this);
    return FALSE;
  }

  // Detect resize events.
  if (e->type() == Event_Resize) {
    QResizeEvent * r = (QResizeEvent *)e;

    if (obj == (QObject *)this->parent) {
#if SOGTKCOMP_RESIZE_DEBUG  // debug
      SoDebugError::postInfo("SoGtkComponent::eventFilter",
                             "resize %p: (%d, %d)",
                             this->widget,
                             r->size().width(), r->size().height());
#endif // debug
      this->widget->resize(r->size());
    }
    else if (obj == (QObject *)this->widget) {
      this->storesize.setValue(r->size().width(), r->size().height());
      this->sizeChanged(this->storesize);
    }
    else
      assert(0);

  }
  // Detect visibility changes.
  else if (e->type() == Event_Show || e->type() == Event_Hide) {
    if (this->visibilitychangeCBs) {
      for (int i=0; i < this->visibilitychangeCBs->getLength()/2; i++) {
        SoGtkComponentVisibilityCB * cb =
          (SoGtkComponentVisibilityCB *)(*(this->visibilitychangeCBs))[i*2+0];
        void * userdata = (*(this->visibilitychangeCBs))[i*2+1];
        cb(userdata, e->type() == Event_Show ? TRUE : FALSE);
      }
    }
  }
*/
  return FALSE;
} // eventFilter()

// *************************************************************************

/*!
  This function \e must be called by subclasses after the component's
  widget has been otherwise initialized.
*/

void
SoGtkComponent::subclassInitialized(
  void )
{
#if SOGTK_DEBUG
  if(!this->widget) {
    SoDebugError::postWarning("SoGtkComponent::subclassInitialized",
                              "Called while no GtkWidget has been set.");
    return;
  }
#endif // SOGTK_DEBUG

//  GtkObject::connect(this->widget, SIGNAL(destroyed()),
//                   this, SLOT(widgetClosed()));
} // subclassInitialized()

// *************************************************************************

/*!
  This will show the widget, deiconifiying and raising it if
  necessary.

  \sa hide(), isVisible()
*/

void
SoGtkComponent::show( // virtual
  void )
{
#if SOGTK_DEBUG
  if(!this->widget) {
    SoDebugError::postWarning("SoGtkComponent::show",
                              "Called while no QWidget has been set.");
    return;
  }
#endif // SOGTK_DEBUG

#if SOGTKCOMP_RESIZE_DEBUG  // debug
  SoDebugError::postInfo("SoGtkComponent::show-1",
                         "resizing %p: (%d, %d)",
                         this->widget,
                         this->storesize[0], this->storesize[1]);
#endif // debug

//  this->widget->resize(this->storesize[0], this->storesize[1]);

#if SOGTKCOMP_RESIZE_DEBUG  // debug
  SoDebugError::postInfo("SoGtkComponent::show-2",
                         "resized %p: (%d, %d)",
                         this->widget,
                         this->widget->size().width(),
                         this->widget->size().height());
#endif // debug

  GtkWidget * window = gtk_widget_get_toplevel( this->widget );
  if ( ! GTK_IS_WINDOW(window) ) {
    // top widget not been hooked to window yet
    gtk_container_add( GTK_CONTAINER(this->parent), this->widget );
    if ( this->storeSize[0] != -1 ) {
      window = gtk_widget_get_toplevel( this->widget );
      if ( GTK_IS_WINDOW(window) ) {
        gtk_window_set_default_size( GTK_WINDOW(window),
          this->storeSize[0], this->storeSize[1] );
      }
    }
  }
  gtk_widget_show( this->widget );

#if SOGTKCOMP_RESIZE_DEBUG  // debug
  SoDebugError::postInfo("SoGtkComponent::show-3",
                         "showed %p: (%d, %d)",
                         this->widget,
                         this->widget->size().width(),
                         this->widget->size().height());
#endif // debug

//  this->widget->raise();
#if SOGTKCOMP_RESIZE_DEBUG  // debug
  SoDebugError::postInfo("SoGtkComponent::show-4",
                         "raised %p: (%d, %d)",
                         this->widget,
                         this->widget->size().width(),
                         this->widget->size().height());
#endif // debug
} // show()

// *************************************************************************

/*!
  This will hide the widget.

  \sa show(), isVisible()
*/

void
SoGtkComponent::hide( // virtual
  void )
{
#if SOGTK_DEBUG
  if(!this->widget) {
    SoDebugError::postWarning("SoGtkComponent::hide",
                              "Called while no QWidget has been set.");
    return;
  }
#endif // SOGTK_DEBUG

  gtk_widget_hide( this->widget );
} // hide()

// *************************************************************************

/*!
  Returns visibility status on the widget. If any parents of this widget
  or this widget itself is hidden, returns \a FALSE.

  Note that a widget which is just obscured by other windows on the
  desktop is not hidden in this sense, and \a TRUE will be returned.

  \sa show(), hide()
*/

SbBool
SoGtkComponent::isVisible(
  void )
{
  if ( ! this->widget )
    return FALSE;

//  return this->widget->isVisible() && this->widget->isVisibleToTLW();
  return TRUE;
} // isVisible()

// *************************************************************************

/*!
  Returns a pointer to the component's window system widget.

  \sa getShellWidget(), getParentWidget()
*/

GtkWidget *
SoGtkComponent::getWidget(
  void ) const
{
  return this->widget;
} // getWidget()

// *************************************************************************

/*!
  An SoGtkComponent may be composed of any number of widgets in
  parent-children relationships in a tree structure with any depth.
  This method will return the root widget in that tree.

  \sa setBaseWidget()
*/

GtkWidget *
SoGtkComponent::baseWidget(
  void ) const
{
  return this->widget;
} // baseWidget()

// *************************************************************************

/*!
  Returns \a TRUE if this component is a toplevel shell, i.e. it has a
  window representation on the desktop.

  \sa getShellWidget()
*/

SbBool
SoGtkComponent::isTopLevelShell(
  void ) const
{
#if SOGTK_DEBUG
  if(!this->widget) {
    SoDebugError::postWarning("SoGtkComponent::isTopLevelShell",
                              "Called while no QWidget has been set.");
    return FALSE;
  }
#endif // SOGTK_DEBUG

//  return this->widget->isTopLevel();
  return TRUE;
} // isTopLevelShell()

// *************************************************************************

/*!
  Returns the toplevel shell widget for this component. The toplevel shell
  is the desktop window which contains the component.

  \sa isTopLevelShell(), getWidget()
*/

GtkWidget *
SoGtkComponent::getShellWidget(
  void ) const
{
#if SOGTK_DEBUG
  if (!this->widget) {
    SoDebugError::postWarning("SoGtkComponent::getShellWidget",
                              "Called while no QWidget has been set.");
    return NULL;
  }
#endif // SOGTK_DEBUG

//  return this->widget->topLevelWidget();
  return this->widget;
} // getShellWidget()
 
// *************************************************************************

/*!
  Returns the widget which is the parent (i.e. contains) this component's
  base widget.

  \sa getWidget(), baseWidget(), isTopLevelShell()
*/

GtkWidget *
SoGtkComponent::getParentWidget(
  void ) const
{
  return this->parent;
} // getParentWidget()

// *************************************************************************

/*!
  Set the window title of this component. This will not work unless the
  component is a toplevel shell.

  \sa getTitle(), setIconTitle(), isTopLevelShell()
*/

void
SoGtkComponent::setTitle(
  const char * const title )
{
  this->captionText = title;
  if ( this->widget ) {
    GtkWidget * window = gtk_widget_get_toplevel( this->widget );
    assert( window != NULL );
    gtk_window_set_title( GTK_WINDOW(window), title );
  }
} // setTitle()

// *************************************************************************

/*!
  Returns the window title. The component should be a toplevel shell if
  you call this method.

  \sa setTitle(), isTopLevelShell()
*/

const char *
SoGtkComponent::getTitle(
  void ) const
{
  return this->captionText.getString();
} // getTitle()

// *************************************************************************

/*!
  Sets the window's title when it is iconfied. The component you use this
  method on should be a toplevel shell.

  \sa getIconTitle(), setTitle(), isTopLevelShell()
*/

void
SoGtkComponent::setIconTitle(
  const char * const newIconTitle )
{
  this->iconText = newIconTitle;
//  if (this->widget) this->getShellWidget()->setIconText(newIconTitle);
} // setIconTitle()

// *************************************************************************

/*!
  Returns the title the window has when iconfied. The component should be
  a toplevel shell if you use this method.

  \sa setIconTitle(), isTopLevelShell()
*/

const char *
SoGtkComponent::getIconTitle(
  void ) const
{
//  return this->icontext.isNull() ? nullstring : (const char *)this->icontext;
  return "";
} // getIconTitle()

// *************************************************************************

/*!
  Returns name of the widget.
*/

const char *
SoGtkComponent::getWidgetName(
  void ) const
{
//   return
//    this->widgetname.isNull() ? nullstring : (const char *)this->widgetname;
  return "";
} // getWidgetName()

// *************************************************************************

/*!
  Returns class name of widget.
*/

const char *
SoGtkComponent::getClassName(
  void ) const
{
  return "";
//  return (const char *) this->classname;
} // getClassName()

// *************************************************************************

/*!
  Returns the default name of an SoGtkComponent widget. Should be overloaded
  by subclasses.
*/

const char *
SoGtkComponent::getDefaultWidgetName( // virtual
  void ) const
{
  static char defaultWidgetName[] = "SoGtkComponent";
  return defaultWidgetName;
} // getDefaultWidgetName()

// *************************************************************************

/*!
  Returns the default window caption string of this component. Should be
  overloaded by subclasses.
*/

const char *
SoGtkComponent::getDefaultTitle( // virtual
  void ) const
{
  static char defaultTitle[] = "Gtk Component";
  return defaultTitle;
}

// *************************************************************************

/*!
  Returns the default icon title of this component. Should be overloaded
  by subclasses.
*/

const char *
SoGtkComponent::getDefaultIconTitle( // virtual
  void ) const
{
  static char defaultIconTitle[] = "Gtk Comp";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
  Resize the component widget.

  \sa getSize()
*/

void
SoGtkComponent::setSize(
  const SbVec2s size )
{
#if SOGTK_DEBUG
  if((size[0] <= 0) || (size[1] <= 0)) {
    SoDebugError::postWarning("SoGtkComponent::setSize",
                              "Invalid size setting: <%d, %d>.",
                              size[0], size[1]);
    return;
  }
#endif // SOGTK_DEBUG

#if SOGTKCOMP_RESIZE_DEBUG  // debug
  SoDebugError::postInfo("SoGtkComponent::setSize",
                         "resize %p: (%d, %d)",
                         this->widget,
                         size[0], size[1]);
#endif // debug

  this->storeSize = size;
  if ( this->widget ) {
    GtkWidget * window = gtk_widget_get_toplevel( GTK_WIDGET(this->widget) );
    if ( GTK_IS_WINDOW(window) ) {
      gtk_window_set_default_size( GTK_WINDOW(window), size[0], size[1] );
    }
    this->sizeChanged( size );
  }
} // setSize()

// *************************************************************************

/*!
  Returns the component widget size.

  \sa setSize()
*/

SbVec2s
SoGtkComponent::getSize(
  void ) const
{
  return this->storeSize;
} // getSize()

// *************************************************************************

/*!
  Open a dialog providing help about use of this component.

  NB: no help system has been implemented yet, so for the time being this
  will only pop up an error message.
*/

void
SoGtkComponent::openHelpCard(
  const char * card )
{
  // FIXME: code MiA. 990222 mortene.
//  QMessageBox::warning(NULL, "SoGtk",
//                       "The help functionality has not been "
//                       "implemented.");
} // openHelpCard()

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
SoGtkComponent::setWindowCloseCallback(
  SoGtkComponentCB * func,
  void * const data )
{
  this->closeCB = func;
  this->closeCBdata = data;
} // setWindowCloseCallback()

// *************************************************************************

/*!
  \internal

  SLOT for when the user clicks/selects window decoration close.
*/

//void
//SoGtkComponent::widgetClosed(void)
//{
//  if (this->closeCB) this->closeCB(this->closeCBdata, this);
//} // widgetClosed()

// *************************************************************************

/*!
  Finds and returns the \a SoGtkComponent corresponding to the given
  \a QWidget, if any. If no SoGtkComponent is the "owner" of the widget,
  \a NULL will be returned.
*/

SoGtkComponent *
SoGtkComponent::getComponent(
  GtkWidget * const widget )
{
  int idx = SoGtkComponent::gtkWidgetList.find( widget );
  if ( idx != -1 )
    return (SoGtkComponent *) SoGtkComponent::soGtkCompList[idx];
  else
    return NULL;
} // getComponent()

// *************************************************************************
