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

#ifndef SOGTKGLWIDGETP_H
#define SOGTKGLWIDGETP_H

#include <Inventor/Gtk/SoGuiGLWidgetP.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoGtkGLWidget class.

class SoGtkGLWidgetP : public SoGuiGLWidgetP
{
public:
  SoGtkGLWidgetP(SoGtkGLWidget * publ);
  ~SoGtkGLWidgetP();

  SbBool wasresized;
  SbVec2s glSize;

  GtkWidget * glParent;
  GtkWidget * glWidget;
  GtkWidget * container;
  int borderThickness;
  int glModeBits;

  SbBool drawFrontBuff;

  gint glInit(GtkWidget * widget);
  static gint sGLInit(GtkWidget * widget, void * userData);
  gint glDraw(GtkWidget * widget, GdkEventExpose * event);
  static gint sGLDraw(GtkWidget * widget, GdkEventExpose * event, void * userData);
  gint glReshape(GtkWidget * widget, GdkEventConfigure * event);
  static gint sGLReshape(GtkWidget * widget, GdkEventConfigure * event, void * userData);
};

// ************************************************************************

#endif // !SOGTKGLWIDGETP_H
