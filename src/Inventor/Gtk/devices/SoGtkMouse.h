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

#ifndef __SOGTK_MOUSE_H__
#define __SOGTK_MOUSE_H__

#include <Inventor/Gtk/devices/SoGtkDevice.h>

class SoMouseButtonEvent;
class SoLocation2Event;

// *************************************************************************

class SoGtkMouse : public SoGtkDevice
{
  typedef SoGtkDevice inherited;

public:
  enum SoGtkMouseEventMask {
    ButtonPressMask = 0x01,
    ButtonReleaseMask = 0x02,
    PointerMotionMask = 0x04,
    ButtonMotionMask = 0x08,

    SO_GTK_ALL_MOUSE_EVENTS = 0x0f,
  };

  SoGtkMouse( SoGtkMouseEventMask mask = SO_GTK_ALL_MOUSE_EVENTS );
  virtual ~SoGtkMouse(void);

  virtual void enable( GtkWidget * widget, SoGtkEventHandler * func,
                       void * data );
  virtual void disable( GtkWidget * widget, SoGtkEventHandler * func,
                        void * data );

  virtual const SoEvent * translateEvent( GdkEvent * event );

private:
  SoMouseButtonEvent * buttonevent;
  SoLocation2Event * locationevent;
  SoGtkMouseEventMask eventmask;

}; // class SoGtkMouse

// *************************************************************************

#endif // ! __SOGTK_MOUSE_H__
