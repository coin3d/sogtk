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

#ifndef SOGTKEXAMINERVIEWERP_H
#define SOGTKEXAMINERVIEWERP_H

#include <SoGuiExaminerViewerP.h>

class SoGtkExaminerViewer;

// ************************************************************************

// This class contains private data and methods used within the
// SoGtkExaminerViewer class.

class SoGtkExaminerViewerP : public SoGuiExaminerViewerP
{
public:
  SoGtkExaminerViewerP(SoGtkExaminerViewer * publ);
  ~SoGtkExaminerViewerP();

  void constructor(const SbBool build);

  static class SoGtkViewerButton SoGtkExaminerViewerButtons[];

  GdkPixmap * orthopixmap, * perspectivepixmap;
  GdkBitmap * orthomask, * perspectivemask;

  GtkWidget * cameratogglebutton;
  GtkWidget * feedbacklabel1;
  GtkWidget * feedbacklabel2;
  GtkWidget * feedbackwheel;
  GtkWidget * feedbackedit;
  void setEnableFeedbackControls(const SbBool flag);

  // Pref sheet.
  static void spinAnimationToggled(GtkToggleButton *, gpointer);
  static void feedbackVisibilityToggled(GtkToggleButton *, gpointer);
  static void feedbackEditPressed(GtkEntry *, gpointer);
  static void feedbackWheelPressed(GtkWidget *, gpointer);
  static void feedbackSizeChanged(GtkWidget *, gpointer);
  static void feedbackWheelReleased(GtkWidget *, gpointer);

  // Button row.
  static void camerabuttonCB(GtkWidget *, gpointer);
};

// ************************************************************************

#endif // ! SOGTKEXAMINERVIEWERP_H
