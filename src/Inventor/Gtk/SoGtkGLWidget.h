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

#ifndef SOGTK_GLWIDGET_H
#define SOGTK_GLWIDGET_H

#include <gtk/gtk.h>

#include <Inventor/SbBasic.h>

#include <Inventor/Gtk/SoGtkComponent.h>

enum glModes {
  SO_GL_RGB      = 0x01, SO_GLX_RGB      = SO_GL_RGB,
  SO_GL_DOUBLE   = 0x02, SO_GLX_DOUBLE   = SO_GL_DOUBLE,
  SO_GL_ZBUFFER  = 0x04, SO_GLX_ZBUFFER  = SO_GL_ZBUFFER,
  SO_GL_OVERLAY  = 0x08, SO_GLX_OVERLAY  = SO_GL_OVERLAY,
  SO_GL_STEREO   = 0x10, SO_GLX_STEREO   = SO_GL_STEREO
};

// *************************************************************************

class SOGTK_DLL_API SoGtkGLWidget : public SoGtkComponent {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkGLWidget, SoGtkComponent);

public:
  void setBorder(const SbBool enable);
  SbBool isBorder(void) const;

  virtual void setDoubleBuffer(const SbBool enable);
  SbBool isDoubleBuffer(void) const;

  void setDrawToFrontBufferEnable(const SbBool enable);
  SbBool isDrawToFrontBufferEnable(void) const;

  void setQuadBufferStereo(const SbBool enable);
  SbBool isQuadBufferStereo(void) const;

  GtkWidget * getNormalWidget(void) const;
  GtkWidget * getOverlayWidget(void) const;

  SbBool hasOverlayGLArea(void) const;
  SbBool hasNormalGLArea(void) const;

protected:
  SoGtkGLWidget(
    GtkWidget * const parent = (GtkWidget *) NULL,
    const char * const name = (char *) NULL,
    const SbBool embed = TRUE,
    const int glModes = SO_GL_RGB,
    const SbBool build = TRUE);
  virtual ~SoGtkGLWidget(void);

  virtual void processEvent(GdkEvent * anyevent);

  GtkWidget * buildWidget(GtkWidget * parent);

  GtkWidget * getGtkGLArea(void) const;
  GtkWidget * getGLWidget(void) const { return this->getGtkGLArea(); }

  virtual void redraw(void) = 0;
  virtual void redrawOverlay(void);

  virtual void sizeChanged(const SbVec2s size);
  virtual void widgetChanged(GtkWidget * widget);

  virtual void afterRealizeHook(void);
  virtual void initGraphic(void);
  virtual void initOverlayGraphic(void);

  void setGLSize(const SbVec2s size);
  const SbVec2s getGLSize(void) const;
  float getGLAspectRatio(void) const;

  // old aliases
  void setGlxSize(const SbVec2s size) { this->setGLSize(size); }
  const SbVec2s getGlxSize(void) const { return this->getGLSize(); }
  float getGlxAspectRatio(void) const { return this->getGLAspectRatio(); }
  void setStereoBuffer(SbBool flag) {
    this->setQuadBufferStereo(flag);
  }
  SbBool isStereoBuffer(void) const {
    return this->isQuadBufferStereo();
  }
  SbBool isRGBMode(void);

  void glLockNormal(void);
  void glUnlockNormal(void);

  void glLockOverlay(void);
  void glUnlockOverlay(void);

  void glSwapBuffers(void);
  void glFlushBuffer(void);

  virtual SbBool glScheduleRedraw(void);

  SbBool waitForExpose;

  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
  static gint eventHandler(GtkWidget * object, GdkEvent * event, gpointer closure);

private:
  // friends and family
  class SoGtkGLWidgetP * pimpl;
  friend class SoGtkGLWidgetP;

}; // class SoGtkGLWidget

// *************************************************************************

#endif // ! SOGTK_GLWIDGET_H
