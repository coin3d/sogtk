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

#include <Inventor/Gtk/SoGtkTypedObject.h>
#include <Inventor/Gtk/devices/SoGtkDevice.h>
#include <Inventor/Gtk/SoGtkComponent.h>

/*!
  \class SoGtkTypedObject Inventor/Gtk/SoGtkTypedObject.h
  \brief The SoGtkTypedObject class provides run-time type checking
  facilities for SoGtk objects.

  This class is an extension to the original Inventor SoXt API.
*/

// *************************************************************************

SoType SoGtkTypedObject::classTypeId; // static

void
SoGtkTypedObject::init( // static
  void )
{
  SoGtkTypedObject::initClass();
  SoGtkDevice::initClasses();
  SoGtkComponent::initClasses();
} // init()

void
SoGtkTypedObject::initClass( // static
  void )
{
  assert( SoGtkTypedObject::classTypeId == SoType::badType() );
  SoGtkTypedObject::classTypeId =
    SoType::createType( SoType::badType(), "GtkTypedObject");
} // initClass()

SbBool
SoGtkTypedObject::isOfType(
  SoType type ) const
{
  return this->getTypeId().isDerivedFrom(type);
} // isOfType()

SoType
SoGtkTypedObject::getClassTypeId( // static
  void ) 
{
  return SoGtkTypedObject::classTypeId;
} // getClassTypeId()

// *************************************************************************
