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
