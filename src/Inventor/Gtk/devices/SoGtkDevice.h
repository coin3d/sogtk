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

#ifndef SOGTK_DEVICE_H
#define SOGTK_DEVICE_H

#include <gtk/gtk.h>

#include <Inventor/SbLinear.h>

#include <Inventor/Gtk/SoGtkObject.h>

class SoEvent;

// *************************************************************************

class SOGTK_DLL_API SoGtkDevice : public SoGtkObject {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkDevice, SoGtkObject);

public:
  typedef void SoGtkEventHandler( GtkWidget *, GdkEvent *, gpointer );

  virtual void enable( GtkWidget * widget, SoGtkEventHandler * func,
                       void * closure ) = 0;
  virtual void disable( GtkWidget * widget, SoGtkEventHandler * func,
                        void * closure ) = 0;

  virtual const SoEvent * translateEvent( GdkEvent * event ) = 0;

  void setWindowSize( const SbVec2s size );
  const SbVec2s getWindowSize(void) const;

  static void initClasses(void);

protected:
  void setEventPosition( SoEvent * event, int x, int y ) const;
  static SbVec2s getLastEventPosition(void);

private:
  static SbVec2s lastEventPosition;
  SbVec2s widgetSize;

}; // class SoGtkDevice

typedef void (*SoGtkEventHandlerPtr)( GtkWidget *, GdkEvent *, gpointer );

// *************************************************************************

#endif // ! SOGTK_DEVICE_H
