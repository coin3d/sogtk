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
#include "config.h"
#endif

#include <gtk/gtk.h>

#include <Inventor/SbTime.h>
#include <Inventor/SbPList.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/SoGtkObject.h>

// *************************************************************************

/*!
  \mainpage

  %SoGtk is a C++ GUI toolkit for using Coin with GTK+.  It can also be
  used on top of Open Inventor from SGI and TGS.  The API is based on the
  InventorXt API originally from SGI.

  The corresponding documentation for Coin is
  located at http://www.coin3d.org/coin-docs/
*/

/*!
  \class SoGtk SoGtk.h Inventor/Gtk/SoGtk.h
  \brief The SoGtk class takes care of Gtk initialization and event dispatching.
  \ingroup misc

  \sa SoGtkComponent
*/

// *************************************************************************

GtkWidget * SoGtk::mainWidget = (GtkWidget *) NULL;
SbPList * SoGtk::components = (SbPList *) NULL;
SbPList * SoGtk::component_callbacks = (SbPList *) NULL;

/*!
  \typedef void SoGtk::SoGtkComponentActionCallback( SoGtkComponent *, SoGtk::SoGtkComponentAction, void * )
  FIXME: write doc
*/

/*!
  \enum SoGtk::SoGtkComponentAction
  FIXME: write doc
*/

/*!
  \var SoGtk::SoGtkComponentAction SoGtk::CREATION
  FIXME: write doc
*/

/*!
  \var SoGtk::SoGtkComponentAction SoGtk::DESTRUCTION
  FIXME: write doc
*/

/*!
  \var SoGtk::SoGtkComponentAction SoGtk::CHANGE
  FIXME: write doc
*/

// *************************************************************************

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes.  It just adds dummy \a argc and
  \a argv arguments and calls the SoGtk::init() method below.
*/

GtkWidget *
SoGtk::init(
  const char * const appName,
  const char * const className )
{
  if ( appName != NULL ) {
    char buf[1025];
    strncpy( buf, appName, 1024 );
    char * array[2] = { buf, (char *) NULL };
    return SoGtk::init( 1, array, appName, className );
  } else {
    return SoGtk::init( 0, (char **) NULL, appName, className );
  }
} // init()

// *************************************************************************

/*!
  Calls \a SoDB::init(), \a SoNodeKit::init() and \a SoInteraction::init().
  Assumes you are creating your own QApplication and main widget.
  \a topLevelWidget should be your application's main widget.
*/

void
SoGtk::init(
  GtkWidget * const topLevelWidget )
{
#if SOGTK_DEBUG
  if ( SoGtk::mainWidget != NULL ) {
    SoDebugError::postWarning( "SoGtk::init",
                               "This method should be called only once." );
    return;
  }
#endif // SOGTK_DEBUG

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();
  SoGtkObject::init();

  SoDB::getSensorManager()->setChangedCallback(
    SoGtk::sensorQueueChanged, NULL );
  SoGtk::mainWidget = topLevelWidget;
#if defined(ENABLE_NLS)
  char *txt = bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
#endif
} // init()

// *************************************************************************

/*!
  Initializes the SoGtk component toolkit library, as well as the Open Inventor
  library.

  Calls \a SoDB::init(), \a SoNodeKit::init() and \a SoInteraction::init(), and
  creates a QApplication and constructs and returns a  main widget for
  you

  \sa getApplication()
*/

GtkWidget *
SoGtk::init(
  int argc,
  char ** argv,
  const char * const appName,
  const char * const className )
{
#if SOGTK_DEBUG
  if ( SoGtk::mainWidget ) {
    SoDebugError::postWarning( "SoGtk::init",
                               "This method should be called only once." );
    return SoGtk::mainWidget;
  }
#endif // SOGTK_DEBUG

  gtk_init( &argc, &argv );

  GtkWidget * mainwin = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  SoGtk::init( mainwin );

#if 0 // debug
  SoDebugError::postInfo("SoGtk::init", "setCaption('%s')", appName);
#endif // debug

  if ( appName )
    gtk_window_set_title( GTK_WINDOW(mainwin), appName );

//  SoGtk::appobject->setMainWidget(SoGtk::mainWidget);
  return SoGtk::mainWidget;
} // init()

// *************************************************************************

gint
SoGtk::timerSensorCB( // static, private
  gpointer data )
{
  SoDB::getSensorManager()->processTimerQueue();
  SoGtk::sensorQueueChanged( NULL );
  return 0; // FIXME: wild guess.. 20000319 mortene.
} // timerSensorCB()

gint
SoGtk::idleSensorCB( // static, private
  gpointer data )
{
  SoDB::getSensorManager()->processDelayQueue(TRUE);
  SoGtk::sensorQueueChanged( NULL );
  return 0; // FIXME: wild guess.. 20000319 mortene.
} // idleSensorCB()

gint
SoGtk::delaySensorCB( // static, private
  gpointer data)
{
  SoDB::getSensorManager()->processDelayQueue(FALSE);
  SoGtk::sensorQueueChanged( NULL );
  return 0; // FIXME: wild guess.. 20000319 mortene.
}

// *************************************************************************

/*!
  \internal

  This function gets called whenever something has happened to any of
  the sensor queues. It starts or reschedules a timer which will trigger
  when a sensor is ripe for plucking.
*/

void
SoGtk::sensorQueueChanged(
  void * )
{
//  SoDebugError::postInfo( "SoGtk::sensorQueueChanged", "[invoked]" );
  SoSensorManager * sm = SoDB::getSensorManager();

  // FIXME: the timer stuff below (and the timer callback functions
  // above) is just a quick hack. I didn't even read any GTK
  // docs. Needs to be done properly. 20000319 mortene.

  static guint timerid = 0;
  static guint idleid = 0;
  static guint delayid = 0;

  if (timerid) gtk_timeout_remove(timerid); timerid = 0;
  if (idleid) gtk_idle_remove(idleid); idleid = 0;
  if (delayid) gtk_timeout_remove(delayid); delayid = 0;

  // Set up timer queue timeout if necessary.

  SbTime t;
  if (sm->isTimerSensorPending(t)) {
    SbTime interval = t - SbTime::getTimeOfDay();
    timerid = gtk_timeout_add(int(interval.getValue() * 1000.0),
                              SoGtk::timerSensorCB, NULL);
  }

  // Set up idle notification for delay queue processing if necessary.

  if (sm->isDelaySensorPending()) {
    idleid = gtk_idle_add(SoGtk::idleSensorCB, NULL);
    delayid = gtk_timeout_add(SoDB::getDelaySensorTimeout().getMsecValue(), SoGtk::delaySensorCB, NULL);
  }

} // sensorQueueChanged()

// *************************************************************************

#if 0 // FIXME: re-code to be run automatically upon exit. 19991107 mortene.
/*!
  \internal

  This is provided for convenience when debugging the library.
  Should make it easier to find memory leaks.
*/

void
SoGtk::clean(
  void )
{
  delete SoGtk::mainWidget; SoGtk::mainWidget = NULL;
//   delete SoGtk::appobject; SoGtk::appobject = NULL;

  delete SoGtk::timerqueuetimer; SoGtk::timerqueuetimer = NULL;
  delete SoGtk::idletimer; SoGtk::idletimer = NULL;
  delete SoGtk::delaytimeouttimer; SoGtk::delaytimeouttimer = NULL;

  delete SoGtk::slotobj; SoGtk::slotobj = NULL;
} // clean()
#endif // re-code

// *************************************************************************

/*!
  This is the event dispatch loop. It doesn't return until
  \a QApplication::quit() or \a QApplication::exit() is called (which
  is also done automatically by Gtk whenever the user closes an application's
  main widget).
*/

void
SoGtk::mainLoop(
  void )
{
  // We need to process immediate sensors _before_ any events are
  // processed. This is done by installing a global eventFilter here...
  gtk_main();
}

/*!
  This method should tell the main loop to quit.
*/

void
SoGtk::exitMainLoop(
  void )
{
  SOGTK_STUB();
}

// *************************************************************************

/*!
  Returns a pointer to the GtkWidget which is the main widget for the
  application. When this widget gets closed, SoGtk::mainLoop() will
  return (unless the close event is caught by the user).

  \sa getShellWidget()
*/

GtkWidget *
SoGtk::getTopLevelWidget(
  void )
{
  return SoGtk::mainWidget;
} // getTopLevelWidget()

// *************************************************************************

/*!
  Returns a pointer to the GtkWidget which is the top level widget for the
  given GtkWidget \a w. This is just a convenience function provided for
  easier porting of Open Inventor applications based on SoXt components,
  as you can do the same thing by calling the QWidget::topLevelWidget()
  method directly on \a w.

  \sa getTopLevelWidget()
*/

GtkWidget *
SoGtk::getShellWidget(
  const GtkWidget * const widget )
{
#if SOGTK_DEBUG
  if ( widget == NULL ) {
    SoDebugError::postWarning( "SoGtk::getShellWidget",
                               "Called with NULL pointer." );
    return (GtkWidget *) NULL;
  }
#endif // SOGTK_DEBUG

//  return gtk_widget_get_toplevel( widget );
  return (GtkWidget *) NULL;
} // getShellWidget()

// *************************************************************************

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call QWidget::show() and
  QWidget::raise() on the provided \a widget pointer.

  \sa hide()
*/

void
SoGtk::show(
  GtkWidget * const widget )
{
  assert( widget != NULL );
  gtk_widget_show( widget );
} // show()

// *************************************************************************

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call QWidget::hide() on the
  provided \a widget pointer.

  \sa show()
*/

void
SoGtk::hide(
  GtkWidget * const widget )
{
  assert( widget != NULL );
  gtk_widget_hide( widget );
} // hide()

// *************************************************************************

/*!
  This method is provided for easier porting of applications based on the
  Open Inventor SoXt component classes. It will call QWidget::resize() on the
  provided \a w widget pointer.

  \sa getWidgetSize()
*/

void
SoGtk::setWidgetSize(
  GtkWidget * const widget,
  const SbVec2s size )
{
  if ( widget == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning( "SoGtk::setWidgetSize",
                               "Called with NULL pointer." );
#endif // SOGTK_DEBUG
    return;
  }
  if( (size[0] <= 0) || (size[1] <= 0) ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning( "SoGtk::setWidgetSize",
                               "Called with invalid dimension(s): (%d, %d).",
                               size[0], size[1]);
#endif // SOGTK_DEBUG
    return;
  }
  GtkRequisition req = { size[0], size[1] };
  gtk_widget_size_request( GTK_WIDGET(widget), &req );
} // setWidgetSize()

// *************************************************************************

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will do the same as calling
  QWidget::size() (except that we're returning an SbVec2s).

  \sa setWidgetSize()
*/

SbVec2s
SoGtk::getWidgetSize(
  const GtkWidget * const widget )
{
#if SOGTK_DEBUG
  if ( widget == NULL ) {
    SoDebugError::postWarning( "SoGtk::getWidgetSize",
                               "Called with NULL pointer." );
    return SbVec2s(0, 0);
  }
#endif // SOGTK_DEBUG
  SOGTK_STUB();
  return SbVec2s( 0, 0 ); // gtk_widget_width( widget ), gtk_widget_height( widget ) );
} // getWidgetSize()

// *************************************************************************

/*!
  This will pop up an error dialog. It's just a simple wrap-around for
  the Gtk MessageBox::warning() call, provided for easier porting from
  applications using the Open Inventor SoXt component classes.

  If \a widget is \a NULL, the dialog will be modal for the whole
  application (all windows will be blocked for input). If not,
  only the window for the given \a widget will be blocked.

  \a dialogTitle is the title of the dialog box. \a errorStr1 and
  \a errorStr2 contains the text which will be shown in the dialog box.

  There will only be a single "Ok" button for the user to press.
*/

/*
   FIXME: Gtk-1.2.x doesn't have a MessageBox implementation.
   Gnome and yet unreleased versions of Gtk however do. 
   So it might be worth considering either to switch to Gnome or 
   to change this code once Gtk has MessageBoxes (Gtk-2.0?).
*/

void
SoGtk::createSimpleErrorDialog(
  GtkWidget * const widget,
  const char * const dialogTitle,
  const char * const errorStr1,
  const char * const errorStr2 )
{
#if SOGTK_DEBUG
  if (dialogTitle == NULL) {
    SoDebugError::postWarning("SoGtk::createSimpleErrorDialog",
                              "Called with NULL dialogTitle pointer.");
  }
  if (errorStr1 == NULL) {
    SoDebugError::postWarning("SoGtk::createSimpleErrorDialog",
                              "Called with NULL error string pointer.");
  }
#endif // SOGTK_DEBUG
  SbString title(dialogTitle ? dialogTitle : "");
  SbString errstr(errorStr1 ? errorStr1 : "");   

  GtkWidget *dialog = gtk_dialog_new ();
  if ( widget ) // This behavior is strange
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
  gtk_window_set_title (GTK_WINDOW (dialog), title.getString() );
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

  GtkWidget *action_area = GTK_DIALOG (dialog)->action_area;

  GtkWidget *vbox = GTK_DIALOG (dialog)->vbox ;

  GtkWidget *label = gtk_label_new (errstr.getString());
  gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
  gtk_box_pack_start (GTK_BOX(vbox), label, TRUE, TRUE, 10);

  if ( errorStr2 )
  {
    GtkWidget *label2 = gtk_label_new( errorStr2 );
    gtk_label_set_line_wrap (GTK_LABEL (label2), TRUE);
    gtk_box_pack_start (GTK_BOX(vbox), label2, TRUE, TRUE, 10);
  }

  GtkWidget *okbutton = gtk_button_new_with_label ("OK");
  gtk_widget_show (okbutton);
  gtk_box_pack_start (GTK_BOX (action_area), okbutton, FALSE, FALSE, 2);

  gtk_signal_connect_object (GTK_OBJECT (okbutton), "clicked",
    GTK_SIGNAL_FUNC (gtk_widget_destroy),
    GTK_OBJECT(dialog) );

  gtk_widget_show_all (dialog);
} // createSimpleErrorDialog()

// *************************************************************************

struct ActionCallbackInfo {
  SoGtk::SoGtkComponentActionCallback * callback;
  void * closure;
};

/*!
  FIXME: write doc
*/

void
SoGtk::addComponentActionCallback( // static
  SoGtkComponentActionCallback * callback,
  void * closure )
{
  if ( ! SoGtk::component_callbacks )
    SoGtk::component_callbacks = new SbPList;
  ActionCallbackInfo * info = new ActionCallbackInfo;
  info->callback = callback;
  info->closure = closure;
  SoGtk::component_callbacks->append(info);
} // addComponentActionCallback()

/*!
  FIXME: write doc
*/

void
SoGtk::removeComponentActionCallback( // static
  SoGtkComponentActionCallback * callback,
  void * closure )
{
  if ( ! SoGtk::component_callbacks ) {
    SoDebugError::post( "SoGtk::removeComponentActionCallback",
      "no such callback!" );
    return;
  }
  const int numCallbacks = SoGtk::component_callbacks->getLength();
  for ( int i = 0; i < numCallbacks; i++ ) {
    ActionCallbackInfo * info = (ActionCallbackInfo *) SoGtk::component_callbacks->get(i);
    if ( info->callback == callback && info->closure == closure ) {
      delete info;
      SoGtk::component_callbacks->remove(i);
      return;
    }
  }
  SoDebugError::post( "SoGtk::removeComponentActionCallback",
    "no such callback!" );
} // removeComponentActionCallback()

/*!
  FIXME: write doc
*/

void
SoGtk::invokeComponentActionCallbacks( // static, protected
  SoGtkComponent * component,
  SoGtkComponentAction action )
{
  if ( ! SoGtk::component_callbacks ) return;
  const int numCallbacks = SoGtk::component_callbacks->getLength();
  for ( int i = 0; i < numCallbacks; i++ ) {
    ActionCallbackInfo * info = (ActionCallbackInfo *) SoGtk::component_callbacks->get(i);
    info->callback( component, action, info->closure );
  }
} // SoGtk::invokeComponentActionCallbacks()

// *************************************************************************

/*!
  FIXME: write doc
*/

gint
SoGtk::componentCreation( // static, protected
  SoGtkComponent * component )
{
  if ( ! SoGtk::components )
    SoGtk::components = new SbPList;
  SoGtk::components->append( (void *) component );
  SoGtk::invokeComponentActionCallbacks( component, CREATION );
  return FALSE;
} // componentCreation()

/*!
  FIXME: write doc
*/

gint
SoGtk::componentDestruction( // static, protected
  SoGtkComponent * component )
{
  assert( SoGtk::components );
  int idx = SoGtk::components->find( component );
  assert( idx != -1 );
  SoGtk::invokeComponentActionCallbacks( component, DESTRUCTION );
  SoGtk::components->remove( idx );
  return FALSE;
} // componentDestruction()

/*!
  FIXME: write doc
*/

gint
SoGtk::componentChange( // static, protected
  SoGtkComponent * component )
{
  SoGtk::invokeComponentActionCallbacks( component, CHANGE );
  return FALSE;
} // componentChange()

// *************************************************************************

/*!
  Returns the number of components, and appends all the component pointers
  to \a componentlist.
*/

int
SoGtk::getComponents(
  SbPList & componentlist )
{
  if ( ! SoGtk::components ) return 0;
  const int numComponents = SoGtk::components->getLength();
  for ( int i = 0; i < numComponents; i++ )
    componentlist.append( SoGtk::components->get(i) );
  return numComponents;
} // getComponents()

// *************************************************************************
