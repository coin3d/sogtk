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

//  $Id$

#ifndef SOGTK_KEYBOARD_H
#define SOGTK_KEYBOARD_H

#include <Inventor/SbDict.h>

#include <Inventor/Gtk/devices/SoGtkDevice.h>

class SoKeyboardEvent;

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkKeyboard : public SoGtkDevice
{
  typedef SoGtkDevice inherited;

public:
  enum SoGtkKeyboardEventMask {
    KeyPressMask               = 0x01,
    KeyReleaseMask             = 0x02,
    SO_GTK_ALL_KEYBOARD_EVENTS = 0x03
  };

  SoGtkKeyboard( SoGtkKeyboardEventMask mask = SO_GTK_ALL_KEYBOARD_EVENTS );
  virtual ~SoGtkKeyboard(void);

  virtual void enable( GtkWidget * widget, SoGtkEventHandler * func, void * data );
  virtual void disable( GtkWidget * widget, SoGtkEventHandler * func, void * data );

  virtual const SoEvent * translateEvent( GdkEvent * event );

private:
  sogtkEventMask events;
  SoKeyboardEvent * kbdevent;

}; // class SoGtkKeyboard

// *************************************************************************

#endif // ! SOGTK_KEYBOARD_H
