/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

static const char rcsid[] =
  "$Id$";

/*!
  \class SoGtkKeyboard SoGtkKeyboard.h Inventor/Gtk/devices/SoGtkKeyboard.h
  \brief The SoGtkKeyboard class ...
  \ingroup gtkdevices

  FIXME: write class doc
*/

#include <assert.h>

#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG
#include <Inventor/events/SoKeyboardEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkKeyboard.h>

// *************************************************************************

/*!
  Constructor.
*/

SoGtkKeyboard::SoGtkKeyboard(
  sogtkEventMask mask )
{
  this->eventmask = mask;
  this->kbdevent = NULL;
} // SoGtkKeyboard()

/*!
  Destructor.
*/

SoGtkKeyboard::~SoGtkKeyboard(
  void )
{
  delete this->kbdevent;
} // ~SoGtkKeyboard()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkKeyboard::enable(
  GtkWidget *, // w,
  SoGtkEventHandler, // f,
  void * ) // data )
{
  // TODO: implement
} // enable()

/*!
  FIXME: write function documentation
*/

void
SoGtkKeyboard::disable(
  GtkWidget *, // w,
  SoGtkEventHandler, // f,
  void * ) // data )
{
  // TODO: implement
} // disable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SoEvent *
SoGtkKeyboard::translateEvent(
  GdkEvent * event )
{
//  SOGTK_STUB();
  return (SoEvent *) NULL;
} // translateEvent()

// *************************************************************************
