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

static const char rcsid[] =
  "$Id$";

/*!
  \class SoGtkInputFocus SoGtkInputFocus.h Inventor/Gtk/devices/SoGtkInputFocus.h
  \brief The SoGtkInputFocus class ...
  \ingroup gtkdevices

  FIXME: write class doc
*/

#include <Inventor/Gtk/devices/SoGtkInputFocus.h>

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
