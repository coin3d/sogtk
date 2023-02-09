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

// Class documentation in common/SoGuiGLWidgetCommon.cpp.in.

// *************************************************************************

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if HAVE_GLX
#include <GL/glx.h> // For glXIsDirect().
#endif // HAVE_GLX

#include <Inventor/Gtk/common/gl.h>

#include <gtkgl/gtkglarea.h>
#include <gdk/gdk.h>

#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkGLWidget.h>
#include <Inventor/Gtk/SoGtkGLWidgetP.h>
#include <Inventor/Gtk/SoAny.h>
#include <GL/glx.h>

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

static const int SO_BORDER_THICKNESS = 2;

// *************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkGLWidget);

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SoGtkGLWidget::SoGtkGLWidget(GtkWidget * const parent,
                             const char * const name,
                             const SbBool embed,
                             const int glModes,
                             const SbBool build)
  : inherited(parent, name, embed)
{
  this->pimpl = new SoGtkGLWidgetP(this);

  this->waitForExpose = TRUE;
  this->drawToFrontBuffer = FALSE; // FIXME: not in use in SoGtk. 20011012 mortene.
  PRIVATE(this)->wasresized = FALSE;
  PRIVATE(this)->glSize = SbVec2s(0,0);

  PRIVATE(this)->glModeBits = glModes;

  PRIVATE(this)->glParent = (GtkWidget *) NULL;
  PRIVATE(this)->glWidget = (GtkWidget *) NULL;
  PRIVATE(this)->container = (GtkWidget *) NULL;

  PRIVATE(this)->drawFrontBuff = FALSE;

  PRIVATE(this)->borderThickness = SO_BORDER_THICKNESS;

  if (! gdk_gl_query()) {
    SoDebugError::post("SoGtkGLWidget::SoGtkGLWidget", 
      _("OpenGL is not available on your display!"));
    return;
  }

  if (! build) return;
  this->setClassName("SoGtkGLWidget");
  GtkWidget * const glwidget = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(glwidget);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SoGtkGLWidget::~SoGtkGLWidget()
{
  if (PRIVATE(this)->glWidget) SoAny::si()->unregisterGLContext((void*) this);
  delete this->pimpl;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
GtkWidget *
SoGtkGLWidget::buildWidget(GtkWidget * parent)
{
  PRIVATE(this)->glParent = parent;

  // FIXME: Not tested, but I think it just might work. Use SbName to
  // map the display name into a constant and global pointer. Screen
  // is probably coded into the display string. pederb, 2001-06-29
  SbName displayname(gdk_get_display());

  void * display = (void*) displayname.getString();
  void * screen = NULL; // I think this is ok
  
  SoGtkGLWidget * sharewidget = (SoGtkGLWidget*) SoAny::si()->getSharedGLContext(display, screen);

  int glAttributes[16], i = 0;

  if (PRIVATE(this)->glModeBits & SO_GL_RGB) {
    glAttributes[i] = GDK_GL_RGBA; i++;
  }
  if (PRIVATE(this)->glModeBits & SO_GL_DOUBLE) {
    glAttributes[i] = GDK_GL_DOUBLEBUFFER; i++;
  }
  if (PRIVATE(this)->glModeBits & SO_GL_ZBUFFER) {
    glAttributes[i] = GDK_GL_DEPTH_SIZE; i++;
    glAttributes[i] = 1; i++;
  }
  if (PRIVATE(this)->glModeBits & SO_GL_STEREO) {
    glAttributes[i] = GDK_GL_STEREO; i++;
  }

  glAttributes[i] = GDK_GL_STENCIL_SIZE; i++;
  glAttributes[i] = 1; i++;

  glAttributes[i] = GDK_GL_NONE; i++;

  if (sharewidget) {
    PRIVATE(this)->glWidget = gtk_gl_area_share_new(glAttributes, (GtkGLArea*) sharewidget->getGLWidget());
  }
  else {
    PRIVATE(this)->glWidget = gtk_gl_area_new(glAttributes);    
  }
  assert(PRIVATE(this)->glWidget != NULL);

  SoAny::si()->registerGLContext((void*) this, display, screen);

  GtkRequisition req = { 100, 100 };
  gtk_widget_size_request(PRIVATE(this)->glWidget, &req);
  
  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->glWidget), "realize",
                      GTK_SIGNAL_FUNC(SoGtkGLWidgetP::sGLInit), (void *) this);
  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->glWidget), "configure_event",
                      GTK_SIGNAL_FUNC(SoGtkGLWidgetP::sGLReshape), (void *) this);
  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->glWidget), "expose_event",
                      GTK_SIGNAL_FUNC(SoGtkGLWidgetP::sGLDraw), (void *) this);
  
  PRIVATE(this)->container = gtk_frame_new(0);
  gtk_frame_set_shadow_type(GTK_FRAME(PRIVATE(this)->container), GTK_SHADOW_IN);
  gtk_container_set_border_width(GTK_CONTAINER(PRIVATE(this)->container),
    PRIVATE(this)->borderThickness);
  gtk_container_add(GTK_CONTAINER(PRIVATE(this)->container), PRIVATE(this)->glWidget);

  gtk_widget_show(GTK_WIDGET(PRIVATE(this)->glWidget));

  return PRIVATE(this)->container;
}

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkGLWidget::eventFilter(GtkWidget * obj,
                           GdkEvent * ev)
{
  if (inherited::eventFilter(obj, ev)) return TRUE;
  if (! GTK_IS_WIDGET(obj))
    return FALSE;
  // SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "[invoked]");
  // if (! GTK_IS_WIDGET(obj) || GTK_WIDGET(obj) != this->glWidget)
  //    return FALSE;
  this->processEvent(ev);
  return TRUE;
}

/*!
  static wrapper for eventFilter
*/

gint
SoGtkGLWidgetP::eventHandler(GtkWidget * object,
                             GdkEvent * event,
                             gpointer closure)
{
  assert(closure != NULL);
  // SoDebugError::postInfo("SoGtkGLWidgetP::eventHandler", "[invoked]");
  SoGtkGLWidget * component = (SoGtkGLWidget *) closure;
  component->eventFilter(object, event);
  return FALSE;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::setBorder(const SbBool enable)
{
  PRIVATE(this)->borderThickness = enable ? SO_BORDER_THICKNESS : 0;
  // update canvas if it is created
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoGtkGLWidget::isBorder(void) const
{
  return (PRIVATE(this)->borderThickness != 0) ? TRUE : FALSE;
}

// *************************************************************************

// FIXME: make doc in SoQtGLWidget valid for this also. 20011116 mortene.
void
SoGtkGLWidget::setOverlayRender(const SbBool onoff)
{
  SOGTK_STUB();
}

// FIXME: make doc in SoQtGLWidget valid for this also. 20011116 mortene.
SbBool
SoGtkGLWidget::isOverlayRender(void) const
{
  SOGTK_STUB();
  return FALSE;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::setDoubleBuffer(const SbBool enable)
{
  if (PRIVATE(this)->glWidget) {
/*
    if (enable != this->getQGLWidget()->doubleBuffer()) {
      QGLFormat format = this->getQGLWidget()->format();
      format.setDoubleBuffer(enable);
      this->getQGLWidget()->setFormat(format);
      if(!this->getQGLWidget()->isValid()) {
        SoDebugError::post("SoGtkGLWidget::setDoubleBuffer",
                           "Couldn't switch to %s buffer mode. "
                           "Falling back on %s buffer.",
                           enable ? "double" : "single",
                           enable ? "single" : "double");
        format.setDoubleBuffer(!enable);
        this->getQGLWidget()->setFormat(format);
      }

      if (this->glwidget->doubleBuffer()) this->glmodebits |= SO_GL_DOUBLE;
      else this->glmodebits &= ~SO_GL_DOUBLE;
    }
*/
  } else {
    if (enable)
      PRIVATE(this)->glModeBits |= SO_GL_DOUBLE;
    else
      PRIVATE(this)->glModeBits &= ~SO_GL_DOUBLE;
  }
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoGtkGLWidget::isDoubleBuffer(void) const
{
  /*
    if (PRIVATE(this)->glWidget)
    return PRIVATE(this)->glWidget->doubleBuffer();
    else
  */
  return (PRIVATE(this)->glModeBits & SO_GL_DOUBLE) ? TRUE : FALSE;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::setDrawToFrontBufferEnable(const SbBool enable)
{
  PRIVATE(this)->drawFrontBuff = enable;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoGtkGLWidget::isDrawToFrontBufferEnable(void) const
{
  return PRIVATE(this)->drawFrontBuff;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void 
SoGtkGLWidget::setAccumulationBuffer(const SbBool enable)
{
  // FIXME: not implemented yet. 20020503 mortene.
  SOGTK_STUB();
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::getAccumulationBuffer(void) const
{
  return FALSE;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void 
SoGtkGLWidget::setStencilBuffer(const SbBool enable)
{
  // FIXME: not implemented yet. 20020503 mortene.
  SOGTK_STUB();
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::getStencilBuffer(void) const
{
  return FALSE;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void 
SoGtkGLWidget::setQuadBufferStereo(const SbBool enable)
{
  // FIXME: do proper implementation. 20001123 mortene.
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::isQuadBufferStereo(void) const
{
  // FIXME: do proper implementation. 20001123 mortene.
  return FALSE;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
GtkWidget *
SoGtkGLWidget::getNormalWidget(void) const
{
  return this->getGLWidget();
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
GtkWidget *
SoGtkGLWidget::getOverlayWidget(void) const
{
  SOGTK_STUB();
  return (GtkWidget *) NULL;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::setGLSize(const SbVec2s size)
{
  assert(PRIVATE(this)->container);
  GtkRequisition req = {
    size[0] + PRIVATE(this)->borderThickness * 2,
    size[1] + PRIVATE(this)->borderThickness * 2
  };

  gtk_widget_size_request(GTK_WIDGET(PRIVATE(this)->container), &req);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbVec2s
SoGtkGLWidget::getGLSize(void) const
{
  if (! PRIVATE(this)->glWidget)
    return SbVec2s(-1, -1);
  return SbVec2s(PRIVATE(this)->glWidget->allocation.width,
                  PRIVATE(this)->glWidget->allocation.height);
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
float
SoGtkGLWidget::getGLAspectRatio(void) const
{
  if (! PRIVATE(this)->glWidget)
    return 1.0f;
  return (float) PRIVATE(this)->glWidget->allocation.width /
    (float) PRIVATE(this)->glWidget->allocation.height;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
GtkWidget *
SoGtkGLWidget::getGLWidget(void) const
{
  return PRIVATE(this)->glWidget;
}

// *************************************************************************

// doc in super
void
SoGtkGLWidget::sizeChanged(const SbVec2s & size)
{
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::widgetChanged(GtkWidget * widget)
{
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::processEvent(GdkEvent * event)
{
  // Nothing is done here for the SoGtkGLWidget, as realize, resize
  // and expose events are caught by explicitly attaching signal
  // callbacks to the widget.
}


/*!
  FIXME: write doc
*/

gint
SoGtkGLWidgetP::sGLInit(GtkWidget * widget,
                        void * closure)
{
  SoGtkGLWidget * glwidget = (SoGtkGLWidget *) closure;
  glwidget->initGraphic();
  return TRUE;
}

// *************************************************************************

/*!
  FIXME: write doc
*/
gint
SoGtkGLWidgetP::sGLReshape(GtkWidget * widget,
                           GdkEventConfigure * event, 
                           void * closure)
{
  SoGtkGLWidget * glwidget = (SoGtkGLWidget *) closure;
  PRIVATE(glwidget)->wasresized = TRUE;
  PRIVATE(glwidget)->glSize = SbVec2s(PRIVATE(glwidget)->glWidget->allocation.width,
                                      PRIVATE(glwidget)->glWidget->allocation.height);
  return TRUE;
}

// *************************************************************************

// Callback function for expose events.

gint
SoGtkGLWidgetP::sGLDraw(GtkWidget * widget,
                        GdkEventExpose * event, 
                        void * closure)
{
  SoGtkGLWidget * glwidget = (SoGtkGLWidget *) closure;
  glwidget->waitForExpose = FALSE; // Gets flipped from TRUE on first expose.
  if (PRIVATE(glwidget)->wasresized) {
    PRIVATE(glwidget)->wasresized = FALSE;
    glwidget->sizeChanged(PRIVATE(glwidget)->glSize);
  }

  if (!glwidget->glScheduleRedraw()) {
    glwidget->redraw();
  }
  return TRUE;
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::glLockNormal(void)
{
  if (GTK_IS_GL_AREA(PRIVATE(this)->glWidget))
    gtk_gl_area_make_current(GTK_GL_AREA(PRIVATE(this)->glWidget));
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::glUnlockNormal(void)
{
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::glLockOverlay(void)
{
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::glUnlockOverlay(void)
{
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::glSwapBuffers(void)
{
  if (GTK_IS_GL_AREA(PRIVATE(this)->glWidget))
    gtk_gl_area_swapbuffers(GTK_GL_AREA(PRIVATE(this)->glWidget));
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoGtkGLWidget::glFlushBuffer(void)
{
  glFlush();
}

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void 
SoGtkGLWidget::redrawOverlay(void)
{
  // should be empty
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::hasOverlayGLArea(void) const 
{
  return this->getOverlayWidget() != NULL;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::hasNormalGLArea(void) const 
{
  return this->getNormalWidget() != NULL;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void 
SoGtkGLWidget::initGraphic(void)
{
  this->glLockNormal();
  // Need to set this explicitly when running on top of Open Inventor,
  // as it seems to have been forgotten there.
  // This code should be invoked from SoQtRenderArea::initGraphics()
  glEnable(GL_DEPTH_TEST);
  this->glUnlockNormal();
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void 
SoGtkGLWidget::initOverlayGraphic(void)
{
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::glScheduleRedraw(void)
{
  return FALSE;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool 
SoGtkGLWidget::isRGBMode(void)
{
  return TRUE; // FIXME
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
unsigned long
SoGtkGLWidget::getOverlayTransparentPixel(void)
{
  SOGTK_STUB();
  // FIXME: investigate if this function is really used for the other
  // toolkits. 20011012 mortene.
  return 0;
}

// *************************************************************************

SoGtkGLWidgetP::SoGtkGLWidgetP(SoGtkGLWidget * publ)
  : SoGuiGLWidgetP(publ)
{
}

SoGtkGLWidgetP::~SoGtkGLWidgetP()
{
}

// Return a flag indicating whether or not OpenGL rendering is
// happening directly from the CPU(s) to the GPU(s), ie on a local
// display. With GLX on X11, it is possible to do remote rendering.
SbBool
SoGtkGLWidgetP::isDirectRendering(void)
{
#if defined(GDK_WINDOWING_X11)
    /*
  PUBLIC(this)->glLockNormal();
  GLXContext ctx = glXGetCurrentContext();
  if (!ctx) {
    SoDebugError::postWarning("SoGtkGLWidgetP::isDirectRendering",
                              "Could not get hold of current context.");
    return TRUE;
  }
  Bool isdirect = glXIsDirect(GDK_DISPLAY(), ctx);
  PUBLIC(this)->glUnlockNormal();
  return isdirect ? TRUE : FALSE;
     */
    // TODO: fix usage 
    return (TRUE);
#else // ! X11
  return TRUE; // Neither MSWindows nor Mac OS X is capable of remote display.
#endif // ! X11
}

// *************************************************************************
