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

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkInputFocus.h>

// *************************************************************************

/*!
  \class SoGtkInputFocus SoGtkInputFocus.h Inventor/Gtk/devices/SoGtkInputFocus.h
  \brief The SoGtkInputFocus class ...
  \ingroup sogtkdevices

  FIXME: write class doc
*/

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkInputFocus);

// *************************************************************************

/*!
  Constructor.
*/

SoGtkInputFocus::SoGtkInputFocus(
  sogtkEventMask mask )
{
  this->mask = mask;
} // SoGtkInputFocus()

// *************************************************************************

/*!
  Destructor.
*/

SoGtkInputFocus::~SoGtkInputFocus(
  void )
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
  void * data )
{
} // enable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkInputFocus::disable(
  GtkWidget * widget,
  SoGtkEventHandler * func,
  void * data )
{
} // disable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SoEvent *
SoGtkInputFocus::translateEvent(
  GdkEvent * event )
{
  return NULL;
} // translateEvent()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkInputFocusRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

