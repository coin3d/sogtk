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

// $Id$

#ifndef SOGTK_PLANEVIEWER_H
#define SOGTK_PLANEVIEWER_H

#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>

// class SbPlaneProjector;

// *************************************************************************

class SOGTK_DLL_API SoGtkPlaneViewer : public SoGtkFullViewer {
  SOGTK_OBJECT_HEADER(SoGtkPlaneViewer, SoGtkFullViewer);

public:
  SoGtkPlaneViewer(
    GtkWidget * parent = (GtkWidget *) 0,
    const char * const name = (char *) 0,
    const SbBool embed = TRUE,
    const SoGtkFullViewer::BuildFlag flag = SoGtkFullViewer::BUILD_ALL,
    const SoGtkViewer::Type type = SoGtkViewer::BROWSER);
  ~SoGtkPlaneViewer(void);

  // overloaded
  virtual void setViewing(SbBool enable);
  virtual void setCamera(SoCamera * camera);
  virtual void setCursorEnabled(SbBool enable);

protected:
  SoGtkPlaneViewer(
    GtkWidget * parent,
    const char * const name,
    const SbBool embed,
    const SoGtkFullViewer::BuildFlag flag,
    const SoGtkViewer::Type type,
    const SbBool build);

  GtkWidget * buildWidget(GtkWidget * parent);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual SbBool processSoEvent(const SoEvent * const event);
  virtual void processEvent(GdkEvent * event);
  virtual void setSeekMode(SbBool enable);
  virtual void actualRedraw(void);

  virtual void bottomWheelMotion(float value);
  virtual void leftWheelMotion(float value);
  virtual void rightWheelMotion(float value);

  virtual void createViewerButtons(GtkWidget * parent, SbPList * buttons);
  virtual void openViewerHelpCard(void);

private:
  void constructor(const SbBool build);

  void setCursorRepresentation(int mode);
private:
  // friends and family
  class SoAnyPlaneViewer * common;
  friend class SoAnyPlaneViewer;
  class SoGtkPlaneViewerP * pimpl;
  friend class SoGtkPlaneViewerP;

}; // class SoGtkPlaneViewer

// *************************************************************************

#endif // ! SOGTK_PLANEVIEWER_H
