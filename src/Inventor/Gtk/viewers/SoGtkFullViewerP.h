/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef SOGTKFULLVIEWERP_H
#define SOGTKFULLVIEWERP_H

#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>
#include <Inventor/SbBasic.h>
#include <Inventor/Gtk/viewers/SoGuiFullViewerP.h>

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

  // button selections
  void interactbuttonClicked(void);
  void viewbuttonClicked(void);
  void homebuttonClicked(void);
  void sethomebuttonClicked(void);
  void viewallbuttonClicked(void);
  void seekbuttonClicked(void);
};

// ************************************************************************

#endif // ! SOGTKFULLVIEWERP_H
