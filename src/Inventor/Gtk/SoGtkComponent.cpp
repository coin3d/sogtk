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
#endif

#include <stdio.h>

#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/SoGtkComponent.h>
#include <Inventor/Gtk/SoGtkGLWidget.h>
#include <Inventor/Gtk/SoGtkRenderArea.h>
#include <Inventor/Gtk/viewers/SoGtkViewer.h>
// #include <Inventor/Gtk/viewers/SoGtkCustomViewer.h>
// #include <Inventor/Gtk/viewers/SoGtkFullViewer.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>
#include <Inventor/Gtk/SoGtkMaterialList.h>
#include <Inventor/Gtk/SoGtkGraphEditor.h>
#include <Inventor/Gtk/SoGtkRoster.h>

// *************************************************************************

/*!
  \class SoGtkComponent SoGtkComponent.h Inventor/Gtk/SoGtkComponent.h
  \brief The SoGtkComponent class is the base class for all GUI components.
*/

// *************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkComponent);

void
SoGtkComponent::initClasses( // static
  void )
{
  SoGtkComponent::initClass();
  SoGtkGLWidget::initClass();
  SoGtkRenderArea::initClass();
  SoGtkViewer::initClass();
//  SoGtkCustomViewer::initClass();
  SoGtkFullViewer::initClass();
  SoGtkExaminerViewer::initClass();
//  SoGtkPlaneViewer::initClass();
  SoGtkMaterialList::initClass();
  SoGtkGraphEditor::initClass();
  SoGtkRoster::initClass();
} // initClasses()

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

/*!
  Constructor.

  \a parent is the widget we'll build this component inside.  If \a parent
  is \a NULL, make a new toplevel window.

  \a name is mostly interesting for debugging purposes.

  \a embed specifies whether or not we should make a new
  toplevel window for the component even when we've got a non-NULL
  \a parent.
*/

SoGtkComponent::SoGtkComponent(
  GtkWidget * const parent,
  const char * const name,
  const SbBool embed )
{
  this->parent = parent;
  this->shelled = FALSE;
  this->realized = FALSE;
  this->widget = NULL;
  this->closeCB = NULL;
  this->closeCBdata = NULL;
  this->visibilityChangeCBs = NULL;

  this->className = NULL;
  this->widgetName = NULL;
  this->captionText = NULL;
  this->iconText = NULL;

  if ( name )
    this->widgetName = strcpy( new char [strlen(name)+1], name );

  this->setClassName( "SoGtkComponent" );

  this->storeSize.setValue(-1, -1);

  if ( parent == NULL || ! embed ) {
    this->parent = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    this->shelled = TRUE;
    this->embedded = FALSE;
  } else if ( parent == SoGtk::getTopLevelWidget() ) {
    this->embedded = FALSE; // what if...?
  } else {
    this->embedded = TRUE;
  }
  gtk_signal_connect( GTK_OBJECT(this->parent), "event",
    GTK_SIGNAL_FUNC(SoGtkComponent::eventHandler), (gpointer) this );

  gtk_idle_add( (GtkFunction) SoGtk::componentCreation, (gpointer) this );
} // SoGtkComponent()

/*!
  Destructor.
*/

SoGtkComponent::~SoGtkComponent( // virtual
  void )
{
  SoGtk::componentDestruction( this );
  if ( this->widget )
    gtk_widget_destroy( this->widget );
  delete this->visibilityChangeCBs;
  delete [] this->widgetName;
  delete [] this->className;
  delete [] this->captionText;
  delete [] this->iconText;
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
  if ( this->className ) {
    delete [] this->className;
    this->className = NULL;
  }
  if ( name )
    this->className = strcpy( new char [strlen(name)+1], name );
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
  // SoDebugError::postInfo( "SoGtkComponent::setBaseWidget", "widget = 0x%08x", widget );
  if ( this->widget ) {
    gtk_signal_disconnect_by_func( GTK_OBJECT(this->widget),
      GTK_SIGNAL_FUNC(SoGtkComponent::realizeHandlerCB), (gpointer) this );
  }

  this->widget = NULL;

  if ( ! GTK_IS_WIDGET(widget) ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkComponent::setBaseWidget", "not a widget" );
#endif
    return;
  }

  this->widget = widget;

  if ( this->widget ) {
    gtk_signal_connect( GTK_OBJECT(this->widget), "event",
      GTK_SIGNAL_FUNC(SoGtkComponent::eventHandler), (gpointer) this );
    gtk_signal_connect( GTK_OBJECT(this->widget), "realize",
      GTK_SIGNAL_FUNC(SoGtkComponent::realizeHandlerCB), (gpointer) this );
  }
} // setBaseWidget()

// *************************************************************************

static const char * gdk_event_name( GdkEventType type ) {
  switch ( type ) {
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
  return "<unknown>";
} /* gdk_event_name() */

/*!
  \internal

  Helps us detect changes in size (base widget and parent widget)
  and visibility status.
*/

SbBool
SoGtkComponent::eventFilter( // virtual
  GtkObject * obj,
  GdkEvent * ev )
{
  // SoDebugError::postInfo( "SoGtkComponent::eventFilter", "[invoked]" );
  if ( ! GTK_IS_WIDGET(obj) ) {
    SoDebugError::postInfo( "SoGtkComponent::eventFilter", "huh?" );
    return FALSE;
  }

  GtkWidget * widget = GTK_WIDGET(obj);

  if ( widget != this->baseWidget() &&
       ! gtk_widget_is_ancestor( GTK_WIDGET(this->baseWidget()), GTK_WIDGET(widget) ) )
    return FALSE;

  // fprintf( stderr, "widget: %p  ", widget );

  switch ( ev->type ) {
  case GDK_KEY_PRESS:
  case GDK_KEY_RELEASE:
    return FALSE;

  case GDK_CONFIGURE:
    // fprintf( stderr, "GDK_CONFIGURE\n" );
    if ( this->realized ) {
      GdkEventConfigure * event = (GdkEventConfigure *) ev;
      SbVec2s size( event->width, event->height );
      this->sizeChanged( size );
    }
    break;

  default:
    // fprintf( stderr, "event %s (%d)\n", gdk_event_name(ev->type), ev->type );
    break;
  } // switch ( ev->type )

  return TRUE;
} // eventFilter()

/*!
  static wrapper around eventFilter
*/

gint
SoGtkComponent::eventHandler( // static, private
  GtkObject * object,
  GdkEvent * event,
  gpointer closure )
{
  assert( closure != NULL );
//  SoDebugError::postInfo( "SoGtkComponent::eventHandler",
//    "[invoked (event %d)]", event->type );
  SoGtkComponent * const component = (SoGtkComponent *) closure;
  component->eventFilter( object, event );
  return FALSE;
} // eventHandler()

/*!
*/

gint
SoGtkComponent::realizeHandler( // private
  GtkObject * object )
{
  assert( ! this->realized );
  this->realized = TRUE;
  if ( this->storeSize != SbVec2s(-1, -1) ) {
    GtkRequisition req = { this->storeSize[0], this->storeSize[1] };
    gtk_widget_size_request( this->baseWidget(), &req );
  }
  SbVec2s size( this->baseWidget()->allocation.width, this->baseWidget()->allocation.height );
  this->sizeChanged( size );
  this->afterRealizeHook();
  return FALSE;
} // realizeHandler()

/*!
*/

gint
SoGtkComponent::realizeHandlerCB( // static, private
  GtkObject * object,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkComponent * const component = (SoGtkComponent *) closure;
  return component->realizeHandler( object );
} // realizeHandlerCB()

// *************************************************************************

/*!
  Returns the visibility status of the component.  If any parent widgets of
  this component is hidden, returns \a FALSE.

  Note that a widget which is just obscured by other windows on the
  desktop is not hidden in this sense, and \a TRUE will be returned.

  \sa show(), hide()
*/

SbBool
SoGtkComponent::isVisible(
  void )
{
  if ( ! this->baseWidget() )
    return FALSE;
  // FIXME - return true visibility state 
  // Close, but probably still incomplete
  return GTK_WIDGET_DRAWABLE(this->baseWidget()) ? TRUE : FALSE;
} // isVisible()

/*!
  This will show the widget, deiconifiying and raising it if
  necessary.

  \sa hide(), isVisible()
*/

void
SoGtkComponent::show( // virtual
  void )
{
  // SoDebugError::postInfo( "SoGtkComponent::show", "[invoked]" );
  if( ! this->baseWidget() || ! GTK_IS_WIDGET(this->baseWidget()) ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning( "SoGtkComponent::show",
      this->baseWidget() ? "not a widget" : "no widget" );
#endif // SOGTK_DEBUG
    return;
  }
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * widget = this->baseWidget();
  assert( parent != NULL );
  assert( widget != NULL );

  if ( ! widget->parent )
    gtk_container_add( GTK_CONTAINER(parent), widget );

  if ( this->storeSize != SbVec2s( -1, -1 ) && GTK_IS_WINDOW(parent) )
    gtk_window_set_default_size( GTK_WINDOW(parent), this->storeSize[0], this->storeSize[1] );

  if ( this->shelled ) {
    if ( ! this->realized ) {
      gtk_signal_connect( GTK_OBJECT(this->getParentWidget()), "event",
        GTK_SIGNAL_FUNC(SoGtkComponent::eventHandler), (gpointer) this );
      gtk_widget_show( widget );
    }
    gtk_widget_show( parent );
  } else {
    gtk_widget_show( widget );
  }
  // fprintf( stderr, "widget shown\n" );
} // show()

/*!
  This will hide the component.

  \sa show(), isVisible()
*/

void
SoGtkComponent::hide( // virtual
  void )
{
#if SOGTK_DEBUG
  if ( ! this->baseWidget() || ! GTK_IS_WIDGET(this->baseWidget()) ) {
    SoDebugError::postWarning( "SoGtkComponent::hide",
      this->baseWidget() ? "not a widget" : "no widget" );
    return;
  }
#endif // SOGTK_DEBUG
  if ( this->shelled )
    gtk_widget_hide( GTK_WIDGET(this->getParentWidget()) );
  else
    gtk_widget_hide( GTK_WIDGET(this->baseWidget()) );
} // hide()

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
  if ( ! this->widget ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning( "SoGtkComponent::getShellWidget", "no widget" );
#endif // SOGTK_DEBUG
    return NULL;
  }
  return this->widget; // FIXME
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
  if ( this->captionText ) {
    delete [] this->captionText;
    this->captionText = NULL;
  }
  if ( title )
    this->captionText = strcpy( new char [strlen(title)+1], title );
  if ( ! this->embedded )
    gtk_window_set_title( GTK_WINDOW(this->parent), title ? title : "" );
} // setTitle()

/*!
  Returns the window title. The component should be a toplevel shell if
  you call this method.

  \sa setTitle(), isTopLevelShell()
*/

const char *
SoGtkComponent::getTitle(
  void ) const
{
  if ( this->captionText == NULL )
    return this->getDefaultTitle();
  return this->captionText;
} // getTitle()

// *************************************************************************

/*!
  Sets the window's title when it is iconfied. The component you use this
  method on should be a toplevel shell.

  \sa getIconTitle(), setTitle(), isTopLevelShell()
*/

void
SoGtkComponent::setIconTitle(
  const char * const title )
{
  if ( this->iconText ) {
    delete [] this->iconText;
    this->iconText = NULL;
  }
  if ( title )
    this->iconText = strcpy( new char [strlen(title)+1], title );
//  if ( this->widget ) {
//    GtkWidget * window = gtk_widget_get_toplevel( this->widget );
//    assert( window != NULL );
//    gtk_window_set_icon_title( GTK_WINDOW(window), title ? title : "" );
//  }
} // setIconTitle()

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
  // SoDebugError::postInfo( "SoGtkComponent::setSize", "[invoked (%d, %d)]", size[0], size[1] );
  if ( size[0] <= 0 || size[1] <= 0 ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkComponent::setSize",
      "Invalid size setting: <%d, %d>.", size[0], size[1]);
#endif // SOGTK_DEBUG
    return;
  }

  this->storeSize = size;

  if ( ! this->embedded ) {
    if ( this->parent ) {
      GtkRequisition req = { size[0], size[1] };
      gtk_widget_size_request( GTK_WIDGET(this->parent), &req );
    }
  } else {
    if ( this->widget ) {
      GtkRequisition req = { size[0], size[1] };
      gtk_widget_size_request( GTK_WIDGET(this->widget), &req );
    }
  }
  if ( this->realized )
    this->sizeChanged( size );

//  SoDebugError::postInfo( "SoGtkComponent::setSize", "[exit]" );
} // setSize()

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

void
SoGtkComponent::sizeChanged( // virtual, protected
  const SbVec2s size )
{
  // nada
} // sizeChanged()

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
  // FIXME
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
  Finds and returns the \a SoGtkComponent corresponding to the given
  \a QWidget, if any. If no SoGtkComponent is the "owner" of the widget,
  \a NULL will be returned.
*/

SoGtkComponent *
SoGtkComponent::getComponent(
  GtkWidget * const widget )
{
//  for (int i = 0; i < SoGtkComponent::soGtkCompList->getLength(); i++) {
//    SoGtkComponent * c = (SoGtkComponent *)((*SoGtkComponent::soGtkCompList)[i]);
//    if ( c->getWidget() == widget ) return c;
//  }

  return NULL;
} // getComponent()

// *************************************************************************

/*!
*/

void
SoGtkComponent::afterRealizeHook( // virtual, protected
  void )
{
  // SoDebugError::postInfo( "SoGtkComponent::afterRealizeHook", "[invoked]" );
  gtk_signal_connect( GTK_OBJECT(this->widget), "event",
    GTK_SIGNAL_FUNC(SoGtkComponent::eventHandler), (gpointer) this );
} // afterRealizeHook()

// *************************************************************************
