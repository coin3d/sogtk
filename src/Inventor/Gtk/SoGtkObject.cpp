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

#ifdef SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif

#include <assert.h>

#include <Inventor/Gtk/SoGtkObject.h>
#include <Inventor/Gtk/devices/SoGtkDevice.h>
#include <Inventor/Gtk/SoGtkComponent.h>

/*!
  \class SoGtkObject Inventor/Gtk/SoGtkObject.h
  \brief The SoGtkObject class provides run-time type checking facilities
  for SoGtk objects.

  This class is an extension to the original Inventor SoXt API.
*/

// *************************************************************************

SoType SoGtkObject::classTypeId; // static

void
SoGtkObject::init( // static
  void )
{
  SoGtkObject::initClass();
  SoGtkDevice::initClasses();
  SoGtkComponent::initClasses();
} // init()

void
SoGtkObject::initClass( // static
  void )
{
  assert( SoGtkObject::classTypeId == SoType::badType() );
  SoGtkObject::classTypeId = SoType::createType( SoType::badType(), "GtkObject");
} // initClass()

SbBool
SoGtkObject::isOfType(
  SoType type ) const
{
  return this->getTypeId().isDerivedFrom(type);
} // isOfType()

SoType
SoGtkObject::getClassTypeId( // static
  void ) 
{
  return SoGtkObject::classTypeId;
} // getClassTypeId()

// *************************************************************************
