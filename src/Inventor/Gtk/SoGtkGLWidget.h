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
  void setBorder( const SbBool enable ); // FIXME: set virtual
  SbBool isBorder(void) const;

  virtual void setDoubleBuffer( const SbBool enable );
  SbBool isDoubleBuffer(void) const;

  void setDrawToFrontBufferEnable( const SbBool enable ); // FIXME: set virtual
  SbBool isDrawToFrontBufferEnable(void) const; // FIXME: change name

  void setQuadBufferStereo(const SbBool enable);
  SbBool isQuadBufferStereo(void) const;

protected:
  SoGtkGLWidget( GtkWidget * const parent = NULL,
                 const char * const name = NULL,
                 const SbBool buildInsideParent = TRUE,
                 const int glModes = SO_GLX_RGB,
                 const SbBool buildNow = TRUE);

  virtual void processEvent( GdkEvent * anyevent );

  GtkWidget * buildWidget( GtkWidget * parent );

  GtkWidget * getGtkGLArea(void);
  GtkWidget * getGLWidget(void) { return getGtkGLArea(); }

  virtual void redraw(void) = 0;

  virtual void sizeChanged( const SbVec2s size );
  virtual void widgetChanged(void);

  void setGLSize( const SbVec2s newSize );
  const SbVec2s getGLSize(void) const;
  float getGLAspectRatio(void) const;

  void glLock(void);
  void glUnlock(void);
  void glSwapBuffers(void);
  void glFlushBuffer(void);

  virtual void glInit(void);
  virtual void glReshape(int width, int height);
  virtual void glRender(void);

  SbBool waitForExpose;

  virtual void afterRealizeHook(void);

  virtual SbBool eventFilter( GtkObject * object, GdkEvent * event );
  static gint eventHandler( GtkObject * object, GdkEvent * event, gpointer closure );

private:
  GtkWidget * glParent;
  GtkWidget * glWidget;
  GtkWidget * container;
  int borderThickness;
  int glModeBits;

  struct {
    unsigned int mouseInput    : 1;
    unsigned int keyboardInput : 1;
    unsigned int drawFrontBuff : 1;
  } properties;

  gint glInit( GtkWidget * widget );
  static gint sGLInit( GtkWidget * widget, void * userData );
  gint glDraw( GtkWidget * widget, GdkEventExpose * event );
  static gint sGLDraw( GtkWidget * widget, GdkEventExpose * event,
    void * userData );
  gint glReshape( GtkWidget * widget, GdkEventConfigure * event );
  static gint sGLReshape( GtkWidget * widget, GdkEventConfigure * event,
    void * userData );

}; // class SoGtkGLWidget

// *************************************************************************

#endif // ! SOGTK_GLWIDGET_H
