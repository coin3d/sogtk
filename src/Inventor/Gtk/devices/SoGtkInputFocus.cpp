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
#include <Inventor/Gtk/devices/SoGtkInputFocus.h>

// *************************************************************************

/*!
  \class SoGtkInputFocus SoGtkInputFocus.h Inventor/Gtk/devices/SoGtkInputFocus.h
  \brief The SoGtkInputFocus class is a device for handling focus events.
  \ingroup devices

  The SoGtkInputFocus class does not translate events like the other
  devices, it is only added for snooping events for the components.

  This class has no functionality yet...
*/

// *************************************************************************

/*!
  \enum SoGtkInputFocus::Events

  Bit-wise enumeration over events handled by the SoGtkInputFocus device class.
  Used in the SoGtkInputFocus constructor.
*/

/*!
  \var SoGtkInputFocus::Events SoGtkInputFocus::ENTER_WINDOW

  The event emitted when focus comes to a widget.
*/

/*!
  \var SoGtkInputFocus::Events SoGtkInputFocus::LEAVE_WINDOW

  The event emitted when focus leaves a widget.
*/

/*!
  \var SoGtkInputFocus::Events SoGtkInputFocus::ALL_EVENTS

  All the events handled by the SoGtkInputFocus device.
*/

// *************************************************************************
// The type system.

SOGTK_OBJECT_SOURCE(SoGtkInputFocus);

// *************************************************************************

/*!
  Constructor.

  The \a eventbits argument decides which events are handled.
  It defaults to SoGtkInputFocus::ALL_EVENTS.
*/

SoGtkInputFocus::SoGtkInputFocus(
  const int eventbits)
{
  this->eventmask = eventbits;
} // SoGtkInputFocus()

/*!
  Destructor.
*/

SoGtkInputFocus::~SoGtkInputFocus()
{
} // ~SoGtkInputFocus()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkInputFocus::enable(
  GtkWidget * widget,
  SoGtkEventHandler * func,
  void * data)
{
  SOGTK_STUB();
} // enable()

/*!
  FIXME: write function documentation
*/

void
SoGtkInputFocus::disable(
  GtkWidget * widget,
  SoGtkEventHandler * func,
  void * data)
{
  SOGTK_STUB();
} // disable()

// *************************************************************************

/*!
  This method snoops on the events, and deals with the input focus related
  events.

  No functionality has been implemented yet.
*/

const SoEvent *
SoGtkInputFocus::translateEvent(
  GdkEvent * event)
{
  SOGTK_STUB();
  return NULL;
} // translateEvent()

// *************************************************************************
