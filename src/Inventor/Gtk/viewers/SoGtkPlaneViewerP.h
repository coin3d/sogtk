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

#ifndef SOGTKPLANEVIEWERP_H
#define SOGTKPLANEVIEWERP_H

#include <SoGuiPlaneViewerP.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoGtkPlaneViewer class.

class SoGtkPlaneViewerP : public SoGuiPlaneViewerP
{
public:
  SoGtkPlaneViewerP(SoGtkPlaneViewer * publ);
  ~SoGtkPlaneViewerP();

  void constructor(const SbBool build);

  static struct SoGtkViewerButton SoGtkPlaneViewerButtons[];
  GdkPixmap * orthopixmap, * perspectivepixmap;
  GdkBitmap * orthomask, * perspectivemask;

  GtkWidget * cameratogglebutton;
  static void xbuttonCB(GtkWidget *, gpointer);
  static void ybuttonCB(GtkWidget *, gpointer);
  static void zbuttonCB(GtkWidget *, gpointer);
  static void camerabuttonCB(GtkWidget *, gpointer);
};

// ************************************************************************

#endif // ! SOGTKPLANEVIEWERP_H
