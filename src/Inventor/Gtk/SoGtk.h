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

//  $Id$

#ifndef SOGTK_H
#define SOGTK_H

#include <math.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

#include <gtk/gtk.h>

class SoSensor;

// *************************************************************************

class SoGtk
{

public:
  static GtkWidget * init( const char * const appName,
                           const char * const className = "SoGtk" );

  static GtkWidget * init( int argc, char ** argv,
                           const char * const appName,
                           const char * const className = "SoGtk" );

  static void init( GtkWidget * const topLevelWidget );

  static void mainLoop(void);

//  static GtkApplication * getApplication(void);
  static GtkWidget * getTopLevelWidget(void);
  static GtkWidget * getShellWidget( const GtkWidget * const widget );

  static void show( GtkWidget * const widget );
  static void hide( GtkWidget * const widget );

  static void setWidgetSize( GtkWidget * const widget, const SbVec2s size );
  static SbVec2s getWidgetSize( const GtkWidget * const widget );

  static void createSimpleErrorDialog( GtkWidget * const widget,
                                       const char * const dialogTitle,
                                       const char * const errorStr1,
                                       const char * const errorStr2 = NULL );

protected:
  bool eventFilter( GtkObject * object, GdkEvent * event );

private:
  static void sensorQueueChanged( void * cbdata );
  static gint timerSensorCB( gpointer data );
  static gint idleSensorCB( gpointer data );
  static gint delaySensorCB( gpointer data );

  static GtkWidget * mainWidget;
//  static GtkApplication * appobject;
//  static Timer * timerqueuetimer, * idletimer, * delaytimeouttimer;

  static SoGtk * sogtk_instance(void);
  static SoGtk * slotobj; // necessary?

}; // class SoGtk

// *************************************************************************

#endif // ! SOGTK_H
