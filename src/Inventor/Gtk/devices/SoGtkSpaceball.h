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

//  $Id$

#ifndef SOGTK_SPACEBALL_H
#define SOGTK_SPACEBALL_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkSpaceball : public SoGtkDevice
{
  typedef SoGtkDevice inherited;

public:
  enum Mask {
    MOTION = 0x01,
    PRESS = 0x02,
    RELEASE = 0x04,
    ALL = 0x07
  };

  SoGtkSpaceball( Mask mask = SoGtkSpaceball::ALL );
  virtual ~SoGtkSpaceball(void);

  virtual void enable( GtkWidget * widget, SoGtkEventHandler * func, void * data );
  virtual void disable( GtkWidget * widget, SoGtkEventHandler * func, void * data );

  virtual const SoEvent * translateEvent( GdkEvent * event );

  void setRotationScaleFactor( float f );
  float getRotationScaleFactor(void) const;
  void setTranslationScaleFactor( float f );
  float getTranslationScaleFactor(void) const;

  static SbBool exists(void);

  void setFocusToWindow( SbBool flag );
  SbBool isFocusToWindow(void) const;

private:
  float rotationscale, translationscale;
  SbBool focustowindow;
  Mask mask;

}; // class SoGtkSpaceball

// *************************************************************************

#endif // ! SOGTK_SPACEBALL_H
