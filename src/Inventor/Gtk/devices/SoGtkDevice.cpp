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
 *  LGPL, please contact SIM to aquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

static const char rcsid[] =
  "$Id$";

/*!
  \class SoGtkDevice SoGtkDevice.h Inventor/Gtk/devices/SoGtkDevice.h
  \brief The SoGtkDevice class ...
  \ingroup gtkdevices

  FIXME: write class doc
*/

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/events/SoEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkDevice.h>

// *************************************************************************

/*!
  \typedef void (SoGtkDevice::SoGtkEventHandler)( GtkWidget *, void *, GdkEvent *, bool * )

  FIXME: write doc
*/

/*!
  \fn void SoGtkDevice::enable( GtkWidget * w, SoGtkEventHandler * f, void * data)

  FIXME: write doc
*/

/*!
  \fn void SoGtkDevice::disable( GtkWidget * w, SoQtEventHandler * f, void * data)
  FIXME: write doc
*/

/*!
  \fn const SoEvent * SoGtkDevice::translateEvent( GdkEvent * event)

  FIXME: write doc
*/

// *************************************************************************

/*!
  \var SbVec2s SoGtkDevice::lastEventPos
  FIXME: write doc
*/

SbVec2s SoGtkDevice::lastEventPos;

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkDevice::setWindowSize(
  const SbVec2s size )
{
  this->widgetSize = size;
} // setWindowSize()

// *************************************************************************

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
  event->setPosition( SbVec2s( x, this->widgetSize[1] - y - 1 ) );
} // setEventPosition()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbVec2s
SoGtkDevice::getLastEventPosition(
  void )
{
  return SoGtkDevice::lastEventPos;
} // getLastEventPosition()

// *************************************************************************
