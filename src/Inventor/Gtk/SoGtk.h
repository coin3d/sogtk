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

// $Id$

#ifndef SOGTK_H
#define SOGTK_H

#include <math.h>

#include <gtk/gtk.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

#include <Inventor/Gtk/SoGtkBasic.h>

class SoSensor;

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtk
{

public:
  static GtkWidget * init( const char * const appName,
                           const char * const className = "SoGtk" );

  static GtkWidget * init( int argc, char ** argv,
                           const char * const appName,
                           const char * const className = "SoGtk" );

  static void init( GtkWidget * const topLevelWidget );

  static void mainLoop(void);
  static void exitMainLoop(void);

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
