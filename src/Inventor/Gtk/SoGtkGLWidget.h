/**************************************************************************\
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

//  $Id$

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

class SOGTK_DLL_EXPORT SoGtkGLWidget : public SoGtkComponent
{
  typedef SoGtkComponent inherited;

public:
  void setBorder( const SbBool enable );
  SbBool isBorder(void) const;

  virtual void setDoubleBuffer( const SbBool enable );
  SbBool isDoubleBuffer(void) const;

  void setDrawToFrontBufferEnable( const SbBool enable );
  SbBool isDrawToFrontBufferEnable(void) const;

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

  void setGlxSize( const SbVec2s newSize );
  void setGLSize( const SbVec2s newSize );
  const SbVec2s getGlxSize(void) const;
  const SbVec2s getGLSize(void) const;
  float getGlxAspectRatio(void) const;
  float getGLAspectRatio(void) const;

  virtual SbBool eventFilter( GtkObject * object, GdkEvent * event );

  void glLock(void);
  void glUnlock(void);
  void glSwapBuffers(void);
  void glFlushBuffer(void);

  virtual void glInit(void);
  virtual void glReshape(int width, int height);
  virtual void glRender(void);

  SbBool waitForExpose;

  static void eventHandler( GtkWidget *, void *, GdkEvent *, bool * );

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
