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

#include <gtk/gtk.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SbTime.h>
#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG

#include <Inventor/Gtk/SoGtk.h>

// *************************************************************************

/*!
  \class SoGtk SoGtk.h Inventor/Gtk/SoGtk.h
  \brief The SoGtk class takes care of Gtk initialization and event dispatching.

  \sa SoGtkComponent
*/

// *************************************************************************

GtkWidget * SoGtk::mainWidget = NULL;
// GtkApplication * SoGtk::appobject = NULL;
// Timer * SoGtk::idletimer = NULL;
// Timer * SoGtk::timerqueuetimer = NULL;
// Timer * SoGtk::delaytimeouttimer = NULL;
SoGtk * SoGtk::slotobj = NULL;

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
    char * array[2] = { buf, NULL };
    return SoGtk::init( 1, array, appName, className );
  } else {
    return SoGtk::init( 0, NULL, appName, className );
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

  SoDB::getSensorManager()->setChangedCallback(
    SoGtk::sensorQueueChanged, NULL );
  SoGtk::mainWidget = topLevelWidget;
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
    return NULL;
  }
#endif // SOGTK_DEBUG

//  return gtk_widget_get_toplevel( widget );
  return NULL;
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
#if SOGTK_DEBUG
  if ( widget == NULL ) {
    SoDebugError::postWarning( "SoGtk::setWidgetSize",
                               "Called with NULL pointer." );
    return;
  }
  if( (size[0] <= 0) || (size[1] <= 0) ) {
    SoDebugError::postWarning( "SoGtk::setWidgetSize",
                               "Called with invalid dimension(s): (%d, %d).",
                               size[0], size[1]);
    return;
  }
#endif // SOGTK_DEBUG

  GtkRequisition req = {
    size[0], size[1]
  };

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

void
SoGtk::createSimpleErrorDialog(
  GtkWidget * const widget,
  const char * const dialogTitle,
  const char * const errorStr1,
  const char * const errorStr2 )
{
} // createSimpleErrorDialog()

// *************************************************************************

/*!
  \internal

  We're using the singleton pattern to create a single SoGtk object instance
*/

SoGtk *
SoGtk::sogtk_instance(
  void )
{
  if ( ! SoGtk::slotobj )
    SoGtk::slotobj = new SoGtk;
  return SoGtk::slotobj;
} // sogtk_instance()

// *************************************************************************

/*!
  \internal

  Uses an event filter on qApp to be able to process immediate delay
  sensors before any system events.
*/

/*
bool
SoGtk::eventFilter(
  GtkObject *,
  GdkEvent * )
{
  if ( SoDB::getSensorManager()->isDelaySensorPending() )
    SoDB::getSensorManager()->processImmediateQueue();

  return FALSE;
} // eventFilter()
*/

// *************************************************************************
