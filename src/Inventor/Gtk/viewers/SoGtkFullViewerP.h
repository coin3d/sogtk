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

#ifndef SOGTKFULLVIEWERP_H
#define SOGTKFULLVIEWERP_H

#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>
#include <Inventor/SbBasic.h>
#include <SoGuiFullViewerP.h>
#include <gtk/gtk.h>

class SoGtkFullViewer;

// ************************************************************************

// This class contains private data and methods used within the
// SoGtkFullViewer class.

class SoGtkFullViewerP : public SoGuiFullViewerP
{
public:
  SoGtkFullViewerP(SoGtkFullViewer * publ);
  ~SoGtkFullViewerP();

  static void setThumbWheelValue(void * wheel, float val);

  GtkWidget * viewerWidget, * canvasParent, * canvas;
  GtkWidget * interactbutton, * viewbutton, * seekbutton;

  SbBool decorations;

  SbBool menuEnabled;

  GtkWidget * mainLayout;
  void showDecorationWidgets(SbBool enable);

  GtkWidget * appButtonLayout;
  GtkWidget * appButtonForm;
  SbPList * appButtonList;
  void layoutAppButtons(GtkWidget * form);

  SbPList * viewerButtons;

  void setCameraZoom(const float zoom);
  float getCameraZoom(void);

  // Thumbwheels.
  void leftWheelPressed(void);
  void leftWheelChanged(float value);
  void leftWheelReleased(void);
  void rightWheelPressed(void);
  void rightWheelChanged(float value);
  void rightWheelReleased(void);
  void bottomWheelPressed(void);
  void bottomWheelChanged(float value);
  void bottomWheelReleased(void);

  // Generic Signal Handlers.
  static void increaseInteractiveCount(GtkWidget *, gpointer);
  static void decreaseInteractiveCount(GtkWidget *, gpointer);

  static SoGtkViewerButton SoGtkFullViewerButtons[];

  static void interactbuttonClickedCB(GtkWidget *, gpointer);
  static void viewbuttonClickedCB(GtkWidget *, gpointer);
  static void helpbuttonClickedCB(GtkWidget *, gpointer);
  static void homebuttonClickedCB(GtkWidget *, gpointer);
  static void sethomebuttonClickedCB(GtkWidget *, gpointer);
  static void viewallbuttonClickedCB(GtkWidget *, gpointer);
  static void seekbuttonClickedCB(GtkWidget *, gpointer);

  static void leftwheelPressedCB(GtkWidget *, gpointer);
  static void leftwheelMovedCB(GtkWidget *, gpointer);
  static void leftwheelReleasedCB(GtkWidget *, gpointer);
  static void bottomwheelPressedCB(GtkWidget *, gpointer);
  static void bottomwheelMovedCB(GtkWidget *, gpointer);
  static void bottomwheelReleasedCB(GtkWidget *, gpointer);
  static void rightwheelPressedCB(GtkWidget *, gpointer);
  static void rightwheelMovedCB(GtkWidget *, gpointer);
  static void rightwheelReleasedCB(GtkWidget *, gpointer);

  // menu selections
  void selectedViewing(void);
  void selectedDecoration(void);
  void selectedHeadlight(void);
  void copyviewSelected(void);
  void pasteviewSelected(void);

  // button selections
  void interactbuttonClicked(void);
  void viewbuttonClicked(void);
  void helpbuttonClicked(void);
  void homebuttonClicked(void);
  void sethomebuttonClicked(void);
  void viewallbuttonClicked(void);
  void seekbuttonClicked(void);
};

// ************************************************************************

#endif // ! SOGTKFULLVIEWERP_H
