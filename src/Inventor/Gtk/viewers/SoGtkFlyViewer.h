/**************************************************************************
 *
 *  This file is part of the Coin SoGtk GUI binding library.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoGtk for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// $Id$

#ifndef SOGTK_FLYVIEWER_H
#define SOGTK_FLYVIEWER_H

#include <Inventor/Gtk/viewers/SoGtkConstrainedViewer.h>

// ************************************************************************

class SOGTK_DLL_API SoGtkFlyViewer : public SoGtkConstrainedViewer {
  SOGTK_OBJECT_HEADER(SoGtkFlyViewer, SoGtkConstrainedViewer);

public:
  SoGtkFlyViewer(
    GtkWidget * parent = (GtkWidget *) 0,
    const char * name = (const char *) 0, 
    SbBool embed = TRUE, 
    SoGtkFullViewer::BuildFlag flag = SoGtkFullViewer::BUILD_ALL,
    SoGtkViewer::Type type = SoGtkViewer::BROWSER);
  ~SoGtkFlyViewer(void);

  virtual void setViewing(SbBool enable);
  virtual void resetToHomePosition(void);
  virtual void setCamera(SoCamera * camera);
  virtual void setCursorEnabled(SbBool enable);
  virtual void setCameraType(SoType type);

protected:
  SoGtkFlyViewer(
    GtkWidget * parent,
    const char * const name, 
    SbBool embed, 
    SoGtkFullViewer::BuildFlag flag, 
    SoGtkViewer::Type type, 
    SbBool build);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual SbBool processSoEvent(const SoEvent * const event);
  virtual void processEvent(GdkEvent * event);
  virtual void setSeekMode(SbBool enable);
  virtual void actualRedraw(void);

  virtual void rightWheelMotion(float value);

  virtual void createPrefSheet(void);
  virtual void openViewerHelpCard(void);

  virtual GtkWidget * makeSubPreferences(GtkWidget * parent);

private:
  void constructor(const SbBool build);

private:
  // friends and family
  class SoAnyFlyViewer * common;
  friend class SoAnyFlyViewer;
  class SoGtkFlyViewerP * pimpl;
  friend class SoGtkFlyViewerP;

}; // class SoGtkFlyViewer

// ************************************************************************

#endif // ! SOGTK_FLYVIEWER_H
