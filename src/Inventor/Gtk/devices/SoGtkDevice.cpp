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
#endif // HAVE_CONFIG_H

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkDevice.h>
#include <Inventor/Gtk/devices/SoGtkDeviceP.h>

// *************************************************************************

/*!
  \typedef void SoGtkDevice::SoGtkEventHandler(GtkWidget *, GdkEvent *, gpointer)

  FIXME: write doc
*/

// *************************************************************************

SoGtkDevice::SoGtkDevice(void)
{
  PRIVATE(this) = new SoGtkDeviceP(this);
}

SoGtkDevice::~SoGtkDevice()
{
  delete PRIVATE(this);
}

void
SoGtkDevice::invokeHandlers(GdkEvent * event)
{
  PRIVATE(this)->invokeHandlers(SoGtkDeviceP::invokeHandlerCB, event);
}

SbBool
SoGtkDeviceP::invokeHandlerCB(SoGtkEventHandler * handler,
                              GtkWidget * widget,
                              GdkEvent * event,
                              void * handlerclosure)
{
  (void)handler(widget, event, handlerclosure);
  return TRUE;
}
