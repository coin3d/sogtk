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
  \class SoGtkSpaceball SoGtkSpaceball.h Inventor/Gtk/devices/SoGtkSpaceball.h
  \brief The SoGtkSpaceball class ...
  \ingroup gtkdevices

  FIXME: write class doc
*/


/*!
  TODO:
  - not implemented yet! no spaceball available
  - idea: maybe we could make device classes for other available cheap
  PC equipment? wheel mice, joysticks, ....
*/

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkSpaceball.h>


/*!
  \enum SoGtkSpaceball::Mask
  FIXME: write documentation for enum
*/
/*!
  \var SoGtkSpaceball::Mask SoGtkSpaceball::MOTION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkSpaceball::Mask SoGtkSpaceball::PRESS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkSpaceball::Mask SoGtkSpaceball::RELEASE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkSpaceball::Mask SoGtkSpaceball::ALL
  FIXME: write documentation for enum definition
*/


/*!
  Constructor.
*/
SoGtkSpaceball::SoGtkSpaceball(SoGtkSpaceball::Mask mask)
{
  this->mask = mask;
  this->rotationscale = .006f;
  this->translationscale = .006f;
  this->focustowindow = FALSE;
}

/*!
  Destructor.
*/
SoGtkSpaceball::~SoGtkSpaceball()
{
}

/*!
  FIXME: write function documentation
*/
void
SoGtkSpaceball::enable(GtkWidget * /*w*/, SoGtkEventHandler /*f*/, void * /*data*/)
{
  // TODO: implement
}

/*!
  FIXME: write function documentation
*/
void
SoGtkSpaceball::disable(GtkWidget * /*w*/, SoGtkEventHandler /*f*/, void * /*data*/)
{
  // TODO: implement
}

/*!
  FIXME: write function documentation
*/
const SoEvent *
SoGtkSpaceball::translateEvent(GdkEvent * /*event*/)
{
  // TODO: implement
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoGtkSpaceball::setRotationScaleFactor(float f)
{
  this->rotationscale = f;
}

/*!
  FIXME: write function documentation
*/
float
SoGtkSpaceball::getRotationScaleFactor(void) const
{
  return this->rotationscale;
}

/*!
  FIXME: write function documentation
*/
void
SoGtkSpaceball::setTranslationScaleFactor(float f)
{
  this->translationscale = f;
}

/*!
  FIXME: write function documentation
*/
float
SoGtkSpaceball::getTranslationScaleFactor(void) const
{
  return this->translationscale;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoGtkSpaceball::exists(void)
{
  // TODO: implement
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoGtkSpaceball::setFocusToWindow(SbBool flag)
{
  this->focustowindow = flag;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoGtkSpaceball::isFocusToWindow(void) const
{
  return this->focustowindow;
}
