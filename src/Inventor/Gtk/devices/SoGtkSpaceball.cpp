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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkSpaceball.h>
#include <Inventor/Gtk/devices/SoGtkSpaceballP.h>

// *************************************************************************

// FIXME:
//  - not implemented yet! no spaceball available
//  - idea: maybe we could make device classes for other available cheap
//    PC equipment? wheel mice, joysticks, ....
//  - implement XInput support

// *************************************************************************

SoGtkSpaceball::SoGtkSpaceball(int eventbits)
{
  PRIVATE(this) = new SoGtkSpaceballP(this);
  PRIVATE(this)->events = eventbits;
  PRIVATE(this)->rotationscale = 0.006f;
  PRIVATE(this)->translationscale = 0.006f;
  PRIVATE(this)->focustowindow = FALSE;
}

SoGtkSpaceball::~SoGtkSpaceball()
{
  delete PRIVATE(this);
}

// *************************************************************************

void
SoGtkSpaceball::enable(GtkWidget * widget,
                       SoGtkEventHandler * func,
                       gpointer closure)
{
  if (func)
    gtk_signal_connect(GTK_OBJECT(widget), "event",
                       GTK_SIGNAL_FUNC(func), closure);
}

void
SoGtkSpaceball::disable(GtkWidget * widget,
                        SoGtkEventHandler * func,
                        gpointer closure)
{
  if (func)
    gtk_signal_disconnect_by_func(GTK_OBJECT(widget),
                                  GTK_SIGNAL_FUNC(func), closure);
}

// *************************************************************************

const SoEvent *
SoGtkSpaceball::translateEvent(GdkEvent * /*event*/)
{
  // FIXME: implement
  SOGTK_STUB();
  return NULL;
}

// *************************************************************************

SbBool
SoGtkSpaceball::exists(void)
{
  // FIXME: implement
  SOGTK_STUB();
  return FALSE;
}

void
SoGtkSpaceball::setFocusToWindow(SbBool flag)
{
  PRIVATE(this)->focustowindow = flag;
}

SbBool
SoGtkSpaceball::isFocusToWindow(void) const
{
  return PRIVATE(this)->focustowindow;
}

// *************************************************************************
