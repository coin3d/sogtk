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
