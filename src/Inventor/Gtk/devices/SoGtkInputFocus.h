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

#ifndef SOGTK_INPUTFOCUS_H
#define SOGTK_INPUTFOCUS_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>

#define sogtkEnterWindowMask 0x01
#define sogtkLeaveWindowMask 0x02

#define SO_GTK_ALL_FOCUS_EVENTS (sogtkEnterWindowMask | sogtkLeaveWindowMask)

// *************************************************************************

class SoGtkInputFocus : public SoGtkDevice
{
  typedef SoGtkDevice inherited;

public:
  SoGtkInputFocus( sogtkEventMask mask = SO_GTK_ALL_FOCUS_EVENTS );
  virtual ~SoGtkInputFocus(void);

  virtual void enable( GtkWidget * w, SoGtkEventHandler * func, void * data );
  virtual void disable( GtkWidget * w, SoGtkEventHandler * func, void * data );

  virtual const SoEvent * translateEvent( GdkEvent * event );

private:
  sogtkEventMask mask;

}; // class SoGtkInputFocus

// *************************************************************************

#endif // ! SOGTK_INPUTFOCUS_H
