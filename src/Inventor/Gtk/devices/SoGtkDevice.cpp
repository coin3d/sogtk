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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkDevice.h>
#include <Inventor/Gtk/devices/SoGtkInputFocus.h>
#include <Inventor/Gtk/devices/SoGtkKeyboard.h>
#include <Inventor/Gtk/devices/SoGtkMouse.h>
#include <Inventor/Gtk/devices/SoGtkSpaceball.h>

// *************************************************************************

/*!
  \class SoGtkDevice SoGtkDevice.h Inventor/Gtk/devices/SoGtkDevice.h
  \brief The SoGtkDevice class ...
  \ingroup devices

  FIXME: write class doc
*/

// *************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkDevice);

/*!
  This function initializes the type system for all the device classes.
*/

void
SoGtkDevice::initClasses( // static
  void )
{
  SoGtkDevice::initClass();
  SoGtkInputFocus::initClass();
  SoGtkMouse::initClass();
  SoGtkKeyboard::initClass();
  SoGtkSpaceball::initClass();
} // initClasses()

// *************************************************************************

/*!
  \var SbVec2s SoGtkDevice::lastEventPosition
  FIXME: write doc
*/

SbVec2s SoGtkDevice::lastEventPosition; // static

// *************************************************************************

/*!
  \typedef void SoGtkDevice::SoGtkEventHandler( GtkWidget *, GdkEvent *, gpointer )

  FIXME: write doc
*/

/*!
  \fn void SoGtkDevice::enable( GtkWidget * widget, SoGtkEventHandler * func, void * closure ) = 0

  FIXME: write doc
*/

/*!
  \fn void SoGtkDevice::disable( GtkWidget * widget, SoGtkEventHandler * func, void * closure ) = 0

  FIXME: write doc
*/

/*!
  \fn const SoEvent * SoGtkDevice::translateEvent( GdkEvent * event)

  FIXME: write doc
*/

// *************************************************************************

/*!
  FIXME: write function documentation
*/

// FIXME: perhaps make virtual?
void
SoGtkDevice::setWindowSize(
  const SbVec2s size )
{
  this->widgetSize = size;
} // setWindowSize()

/*!
  FIXME: write function documentation
*/

const SbVec2s
SoGtkDevice::getWindowSize(
  void ) const
{
  return this->widgetSize;
} // getWindowSize()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkDevice::setEventPosition(
  SoEvent * event,
  int x,
  int y ) const
{
  this->lastEventPosition = SbVec2s( x, y );
  event->setPosition( SbVec2s( x, this->widgetSize[1] - y - 1 ) );
} // setEventPosition()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbVec2s
SoGtkDevice::getLastEventPosition( // static
  void )
{
  return SoGtkDevice::lastEventPosition;
} // getLastEventPosition()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkDeviceRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

