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

//  $Id$

#ifndef SOGTK_MOUSE_H
#define SOGTK_MOUSE_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>

class SoMouseButtonEvent;
class SoLocation2Event;

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkMouse : public SoGtkDevice
{
  typedef SoGtkDevice inherited;

public:
  enum SoGtkMouseEventMask {
    ButtonPressMask         = 0x01,
    ButtonReleaseMask       = 0x02,
    PointerMotionMask       = 0x04,
    ButtonMotionMask        = 0x08,

    SO_GTK_ALL_MOUSE_EVENTS = 0x0f
  };

  SoGtkMouse( SoGtkMouseEventMask mask = SO_GTK_ALL_MOUSE_EVENTS );
  virtual ~SoGtkMouse(void);

  virtual void enable( GtkWidget * widget, SoGtkEventHandler * func, void * data );
  virtual void disable( GtkWidget * widget, SoGtkEventHandler * func, void * data );

  virtual const SoEvent * translateEvent( GdkEvent * event );

private:
  SoGtkMouseEventMask events;
  SoMouseButtonEvent * buttonevent;
  SoLocation2Event * locationevent;

}; // class SoGtkMouse

// *************************************************************************

#endif // ! SOGTK_MOUSE_H
