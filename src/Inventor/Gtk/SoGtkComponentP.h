/**************************************************************************\
 *
 *  This file is part of the Coin GUI toolkit libraries.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you want to use this library with software that is incompatible
 *  licensewise with the LGPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef SOGTKCOMPONENTP_H
#define SOGTKCOMPONENTP_H

#include <Inventor/Gtk/SoGuiComponentP.h>
#include <Inventor/Gtk/SoGtkCursor.h>

// ************************************************************************

// "Hidden" class for storing private data (to avoid cluttering up the
// header file, and to make it easier to keep ABI compatibility upon
// changes.)

class SoGtkComponentP : public SoGuiComponentP
{
public:
  SoGtkComponentP(SoGtkComponent * publ);
  ~SoGtkComponentP();

  static gint realizeHandlerCB(GtkObject * object, gpointer closure);
  static GdkCursor * getNativeCursor(GtkWidget * w,
                                     const SoGtkCursor::CustomCursor * cc);

  GtkWidget * widget;
  GtkWidget * parent;
  SbBool embedded;
  SbBool fullscreen;
  SbBool shelled;
  char * className;
  char * widgetName;
  char * captionText;
  char * iconText;
  SoGtkComponentCB * closeCB;
  void * closeCBdata;
  SbPList * visibilityChangeCBs;
  SbVec2s storeSize;

  static GdkCursor * arrowcursor;
  static GdkCursor * crosscursor;
  static GdkCursor * uparrowcursor;

  struct NonFull {
    gint x, y, w, h;
  };
  NonFull nonfull;

  // List of all SoGtkComponent instances. Needed for the
  // SoGtkComponent::getComponent() function.
  static SbPList * soGtkCompList;

private:
  static SbDict * cursordict;
};

// ************************************************************************

#endif // !SOGTKCOMPONENTP_H
