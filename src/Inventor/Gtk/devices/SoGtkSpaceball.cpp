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

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkSpaceball.h>

// *************************************************************************

/*!
  \class SoGtkSpaceball SoGtkSpaceball.h Inventor/Gtk/devices/SoGtkSpaceball.h
  \brief The SoGtkSpaceball class ...
  \ingroup devices

  FIXME: write class doc
*/

// *************************************************************************

/*¡
  TODO:
  - not implemented yet! no spaceball available
  - idea: maybe we could make device classes for other available cheap
  PC equipment? wheel mice, joysticks, ....
  - implement XInput support
*/

/*!
  \enum SoGtkSpaceball::Events
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkSpaceball::Events SoGtkSpaceball::MOTION
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkSpaceball::Events SoGtkSpaceball::BUTTON_PRESS
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkSpaceball::Events SoGtkSpaceball::BUTTON_RELEASE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkSpaceball::Events SoGtkSpaceball::ALL_EVENTS
  FIXME: write documentation for enum definition
*/

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkSpaceball);

// *************************************************************************

/*!
  Constructor.
*/

SoGtkSpaceball::SoGtkSpaceball(
  const int eventbits )
{
  this->events = eventbits;
  this->rotationscale = 0.006f;
  this->translationscale = 0.006f;
  this->focustowindow = FALSE;
} // SoGtkSpaceball()

/*!
  Destructor.
*/

SoGtkSpaceball::~SoGtkSpaceball(
  void )
{
} // ~SoGtkSpaceball()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkSpaceball::enable(
  GtkWidget *, // w,
  SoGtkEventHandler *, // f,
  void * ) // data )
{
  // TODO: implement
} // enable()

/*!
  FIXME: write function documentation
*/

void
SoGtkSpaceball::disable(
  GtkWidget * /*w*/,
  SoGtkEventHandler * /*f*/,
  void * /*data*/ )
{
  // TODO: implement
} // disable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SoEvent *
SoGtkSpaceball::translateEvent(
  GdkEvent * /*event*/ )
{
  // TODO: implement
  return NULL;
} // translateEvent()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkSpaceball::setRotationScaleFactor(
  float f )
{
  this->rotationscale = f;
} // setRotationScaleFactor()

/*!
  FIXME: write function documentation
*/

float
SoGtkSpaceball::getRotationScaleFactor(
  void ) const
{
  return this->rotationscale;
} // getRotationScaleFactor()

/*!
  FIXME: write function documentation
*/

void
SoGtkSpaceball::setTranslationScaleFactor(
  float f )
{
  this->translationscale = f;
} // setTranslationScaleFactor()

/*!
  FIXME: write function documentation
*/

float
SoGtkSpaceball::getTranslationScaleFactor(
  void ) const
{
  return this->translationscale;
} // getTranslationScaleFactor()

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkSpaceball::exists( // static
  void )
{
  // TODO: implement
  return FALSE;
} // exists()

/*!
  FIXME: write function documentation
*/

void
SoGtkSpaceball::setFocusToWindow(
  SbBool flag )
{
  this->focustowindow = flag;
} // setFocusToWindow()

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkSpaceball::isFocusToWindow(
  void ) const
{
  return this->focustowindow;
} // isFocusToWindow()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkSpaceballRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

