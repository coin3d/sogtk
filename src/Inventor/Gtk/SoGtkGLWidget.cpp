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
 *  LGPL, please contact SIM to aquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include <gtkgl/gtkglarea.h>

#include <Inventor/errors/SoDebugError.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkGLWidget.h>

// *************************************************************************

/*!
  \class SoGtkGLWidget SoGtkGLWidget.h Inventor/Gtk/SoGtkGLWidget.h
  \brief The SoGtkGLWidget class contains an OpenGL canvas.
  \ingroup components
*/

// *************************************************************************

/*!
  \fn virtual void SoGtkGLWidget::redraw(void)
  This method will be called when we need a redraw. It must be overloaded in
  non-abstract subclasses.
*/

/*!
  \var SbBool SoGtkGLWidget::waitForExpose
  FIXME: write doc
*/

/*!
  \fn GtkWidget * SoGtkGLWidget::getGLWidget(void)
  FIXME: write doc
*/

static const int SO_BORDER_THICKNESS = 2;

// *************************************************************************

class SoGtkGLWidgetP {
public:
  SoGtkGLWidgetP( SoGtkGLWidget * publ );
  ~SoGtkGLWidgetP(void);

  int glLockLevel;
  SbBool currentIsNormal;

  GtkWidget * glParent;
  GtkWidget * glWidget;
  GtkWidget * container;
  int borderThickness;
  int glModeBits;

  SbBool drawFrontBuff;

  gint glInit( GtkWidget * widget );
  static gint sGLInit( GtkWidget * widget, void * userData );
  gint glDraw( GtkWidget * widget, GdkEventExpose * event );
  static gint sGLDraw( GtkWidget * widget, GdkEventExpose * event,
    void * userData );
  gint glReshape( GtkWidget * widget, GdkEventConfigure * event );
  static gint sGLReshape( GtkWidget * widget, GdkEventConfigure * event,
    void * userData );

private:
  SoGtkGLWidget * pub;

}; // class SoGtkGLWidgetP

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

#define THIS (this->pimpl)

// *************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkGLWidget);

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoGtkGLWidget::SoGtkGLWidget(
  GtkWidget * const parent,
  const char * const name,
  const SbBool embed,
  const int glModes,
  const SbBool build )
: inherited( parent, name, embed )
{
  this->pimpl = new SoGtkGLWidgetP( this );

  this->waitForExpose = TRUE;

  PRIVATE(this)->glModeBits = glModes;

  PRIVATE(this)->glParent = (GtkWidget *) NULL;
  PRIVATE(this)->glWidget = (GtkWidget *) NULL;
  PRIVATE(this)->container = (GtkWidget *) NULL;

  PRIVATE(this)->drawFrontBuff = FALSE;

  PRIVATE(this)->borderThickness = SO_BORDER_THICKNESS;

  if ( ! gdk_gl_query() ) {
    SoDebugError::post( "SoGtkGLWidget::SoGtkGLWidget", 
      _( "OpenGL is not available on your display!" ) );
    return;
  }

  if ( ! build ) return;
  this->setClassName( "SoGtkGLWidget" );
  GtkWidget * const glwidget = this->buildWidget( this->getParentWidget() );
  this->setBaseWidget( glwidget );
} // SoGtkGLWidget()

/*!
*/

SoGtkGLWidget::~SoGtkGLWidget(
  void )
{
  delete this->pimpl;
} // ~SoGtkGLWidget()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

GtkWidget *
SoGtkGLWidget::buildWidget(
  GtkWidget * parent )
{
  PRIVATE(this)->glParent = parent;

  int glAttributes[16], i = 0;

  if ( PRIVATE(this)->glModeBits & SO_GL_RGB ) {
    glAttributes[i] = GDK_GL_RGBA; i++;
  }
  if ( PRIVATE(this)->glModeBits & SO_GL_DOUBLE ) {
    glAttributes[i] = GDK_GL_DOUBLEBUFFER; i++;
  }
  if ( PRIVATE(this)->glModeBits & SO_GL_ZBUFFER ) {
    glAttributes[i] = GDK_GL_DEPTH_SIZE; i++;
    glAttributes[i] = 1; i++;
  }
  if ( PRIVATE(this)->glModeBits & SO_GL_STEREO ) {
    glAttributes[i] = GDK_GL_STEREO; i++;
  }

  glAttributes[i] = GDK_GL_STENCIL_SIZE; i++;
  glAttributes[i] = 1; i++;

  glAttributes[i] = GDK_GL_NONE; i++;

  PRIVATE(this)->glWidget = gtk_gl_area_new( glAttributes );
  assert( PRIVATE(this)->glWidget != NULL );
  GtkRequisition req = { 100, 100 };
  gtk_widget_size_request( PRIVATE(this)->glWidget, &req );
  
  gtk_signal_connect( GTK_OBJECT(PRIVATE(this)->glWidget), "realize",
    GTK_SIGNAL_FUNC(SoGtkGLWidgetP::sGLInit), (void *) this );
  gtk_signal_connect( GTK_OBJECT(PRIVATE(this)->glWidget), "configure_event",
    GTK_SIGNAL_FUNC(SoGtkGLWidgetP::sGLReshape), (void *) this );
  gtk_signal_connect( GTK_OBJECT(PRIVATE(this)->glWidget), "expose_event",
    GTK_SIGNAL_FUNC(SoGtkGLWidgetP::sGLDraw), (void *) this );

  PRIVATE(this)->container = gtk_frame_new(0);
  gtk_frame_set_shadow_type(GTK_FRAME(PRIVATE(this)->container), GTK_SHADOW_IN);
  gtk_container_set_border_width( GTK_CONTAINER(PRIVATE(this)->container),
    PRIVATE(this)->borderThickness );
  gtk_container_add( GTK_CONTAINER(PRIVATE(this)->container), PRIVATE(this)->glWidget );

  gtk_widget_show( GTK_WIDGET(PRIVATE(this)->glWidget) );

  gtk_widget_add_events( GTK_WIDGET(PRIVATE(this)->glWidget),
    GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
    GDK_BUTTON_MOTION_MASK | GDK_BUTTON1_MOTION_MASK |
    GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK |
    GDK_POINTER_MOTION_MASK );

  return PRIVATE(this)->container;
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkGLWidget::eventFilter(
  GtkObject * obj,
  GdkEvent * ev )
{
  if ( inherited::eventFilter( obj, ev ) ) return TRUE;
  if ( ! GTK_IS_WIDGET(obj) )
    return FALSE;
  // SoDebugError::postInfo( "SoGtkGLWidget::eventFilter", "[invoked]" );
  // if ( ! GTK_IS_WIDGET(obj) || GTK_WIDGET(obj) != this->glWidget )
  //    return FALSE;
  this->processEvent( ev );
  return TRUE;
} // eventFilter()

/*!
  static wrapper for eventFilter
*/

gint
SoGtkGLWidget::eventHandler( // static, protected
  GtkObject * object,
  GdkEvent * event,
  gpointer closure )
{
  assert( closure != NULL );
  // SoDebugError::postInfo( "SoGtkGLWidget::eventHandler", "[invoked]" );
  SoGtkGLWidget * component = (SoGtkGLWidget *) closure;
  component->eventFilter( object, event );
  return FALSE;
} // eventHandler()

// *************************************************************************

/*!
  Specify that there should be a border around the OpenGL canvas (or not).

  \sa isBorder()
*/

void
SoGtkGLWidget::setBorder(
  const SbBool enable )
{
  PRIVATE(this)->borderThickness = enable ? SO_BORDER_THICKNESS : 0;
  // update canvas if it is created
} // setBorder()

/*!
  Returns whether or not there's a border around the OpenGL canvas.

  \sa setBorder()
*/

SbBool
SoGtkGLWidget::isBorder(
  void ) const
{
  return (PRIVATE(this)->borderThickness != 0) ? TRUE : FALSE;
} // isBorder()

// *************************************************************************

/*!
  Switch between single and double buffer mode for the OpenGL canvas.
  The default is to use a single buffer canvas.

  \sa isDoubleBuffer()
*/

void
SoGtkGLWidget::setDoubleBuffer(
  const SbBool enable )
{
  if ( PRIVATE(this)->glWidget ) {
/*
    if ( enable != this->getQGLWidget()->doubleBuffer() ) {
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

      if (this->glwidget->doubleBuffer()) this->glmodebits |= SO_GLX_DOUBLE;
      else this->glmodebits &= ~SO_GLX_DOUBLE;
    }
*/
  } else {
    if ( enable )
      PRIVATE(this)->glModeBits |= SO_GLX_DOUBLE;
    else
      PRIVATE(this)->glModeBits &= ~SO_GLX_DOUBLE;
  }
} // setDoubleBuffer()

/*!
  Returns the status of the buffer mode.

  \sa setDoubleBuffer()
*/

SbBool
SoGtkGLWidget::isDoubleBuffer(
  void ) const
{
/*
  if ( PRIVATE(this)->glWidget )
    return PRIVATE(this)->glWidget->doubleBuffer();
  else
*/
  return (PRIVATE(this)->glModeBits & SO_GL_DOUBLE) ? TRUE : FALSE;
} // isDoubleBuffer()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkGLWidget::setDrawToFrontBufferEnable(
  const SbBool enable )
{
  PRIVATE(this)->drawFrontBuff = enable;
} // setDrawToFrontBufferEnable()

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkGLWidget::isDrawToFrontBufferEnable(
  void ) const
{
  return PRIVATE(this)->drawFrontBuff;
} // isDrawToFrontBufferEnable()

// *************************************************************************

/*!
  Enables or disables quad buffer stereo.
*/

void 
SoGtkGLWidget::setQuadBufferStereo(const SbBool enable)
{
  // FIXME: do proper implementation. 20001123 mortene.
}

/*!
  Returns \c TRUE if quad buffer stereo is enabled for this widget.
*/

SbBool 
SoGtkGLWidget::isQuadBufferStereo(void) const
{
  // FIXME: do proper implementation. 20001123 mortene.
  return FALSE;
}

// *************************************************************************

/*!
  This method is currently just stubbed.
*/

GtkWidget *
SoGtkGLWidget::getNormalWidget(
  void ) const
{
  SOGTK_STUB();
  return (GtkWidget *) NULL;
} // getNormalWidget()

/*!
  This method is currently just stubbed.
*/

GtkWidget *
SoGtkGLWidget::getOverlayWidget(
  void ) const
{
  SOGTK_STUB();
  return (GtkWidget *) NULL;
} // getOverlayWidget()

// *************************************************************************

/*!
  Sets the size of the GL canvas.
*/

void
SoGtkGLWidget::setGLSize(
  const SbVec2s size )
{
  assert( PRIVATE(this)->container );
  GtkRequisition req = {
    size[0] + PRIVATE(this)->borderThickness * 2,
    size[1] + PRIVATE(this)->borderThickness * 2
  };

  gtk_widget_size_request( GTK_WIDGET(PRIVATE(this)->container), &req );
} // setGLSize()

/*!
  Return the dimensions of the OpenGL canvas.
*/

const SbVec2s
SoGtkGLWidget::getGLSize(
  void ) const
{
  if ( ! PRIVATE(this)->glWidget )
    return SbVec2s( -1, -1 );
  return SbVec2s( PRIVATE(this)->glWidget->allocation.width,
                  PRIVATE(this)->glWidget->allocation.height );
} // getGLSize()

/*!
  Return the aspect ratio of the OpenGL canvas.
*/

float
SoGtkGLWidget::getGLAspectRatio(
  void ) const
{
  if ( ! PRIVATE(this)->glWidget )
    return 1.0f;
  return (float) PRIVATE(this)->glWidget->allocation.width /
         (float) PRIVATE(this)->glWidget->allocation.height;
} // getGLAspectRatio()

/*!
  \fn void SoGtkGLWidget::setGlxSize( const SbVec2s size )
  This method has been renamed to the more appropriate setGLSize
  \sa setGLSize
*/

/*!
  \fn SbVec2s SoGtkGLWidget::getGlxSize(void) const
  This method has been renamed to the more appropriate getGLSize
  \sa getGLSize
*/

/*!
  \fn float SoGtkGLWidget::getGlxAspectRatio(void) const
  This method has been renamed to the more appropriate getGLAspectRatio
  \sa getGLAspectRatio
*/

// *************************************************************************

/*!
  Returns a pointer to the GtkGLArea.
*/

GtkWidget *
SoGtkGLWidget::getGtkGLArea(
  void )
{
  return PRIVATE(this)->glWidget;
} // getGtkGLArea()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkGLWidget::sizeChanged(
  const SbVec2s size )
{
} // sizeChanged()

// *************************************************************************

/*!
  This is the method which gets called whenever the OpenGL widget
  changes in any way. Should be overloaded in subclasses.

  \sa sizeChanged()
*/

void
SoGtkGLWidget::widgetChanged( // virtual
  GtkWidget * widget )
{
} // widgetChanged()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkGLWidget::processEvent( // virtual
  GdkEvent * event )
{
  // FIXME: anything to do here?
} // processEvent()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::glInit(
  void )
{
  this->setOverlayRender( FALSE );
  this->glLock();
  glEnable( GL_DEPTH_TEST );
  this->glUnlock();
} // glInit()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidgetP::sGLInit( // static
  GtkWidget * widget,
  void * closure )
{
  SoGtkGLWidget * glwidget = (SoGtkGLWidget *) closure;
  glwidget->glInit();
  return TRUE;
} // sGLInit()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::glReshape( // virtual
  int width,
  int height )
{
} // glReshape()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidgetP::glReshape(
  GtkWidget * widget,
  GdkEventConfigure * event )
{
  assert( this->glWidget != NULL );
  PUBLIC(this)->glReshape( this->glWidget->allocation.width,
                   this->glWidget->allocation.height );
  return TRUE;
} // glReshape()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidgetP::sGLReshape( // static
  GtkWidget * widget,
  GdkEventConfigure * event, 
  void * closure )
{
  SoGtkGLWidget * glwidget = (SoGtkGLWidget *) closure;
  return PRIVATE(glwidget)->glReshape( widget, event );
} // sGLReshape()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::glRender( // virtual
  void )
{
} // glRender()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidgetP::glDraw(
  GtkWidget * widget,
  GdkEventExpose * event )
{
  PUBLIC(this)->glRender();
//  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) ) )
//    return TRUE;

//  this->redraw();
//  gtk_gl_area_swapbuffers( GTK_GL_AREA(this->glWidget) );

  return TRUE;
} // glDraw()


// Callback function for expose events.

gint
SoGtkGLWidgetP::sGLDraw( // static
  GtkWidget * widget,
  GdkEventExpose * event, 
  void * closure )
{
  SoGtkGLWidget * glwidget = (SoGtkGLWidget *) closure;
  gint result = PRIVATE(glwidget)->glDraw( widget, event );
  glwidget->waitForExpose = FALSE; // Gets flipped from TRUE on first expose.
  return result;
} // sGLDraw()

// *************************************************************************

/*!
  This method returns the number of times glLock() has been called.
*/

int
SoGtkGLWidget::getLockLevel(
  void ) const
{
  return PRIVATE(this)->glLockLevel;
} // getLockLevel()

/*!
  This method invokes makecurrent on the GL appropriate context.
*/

void
SoGtkGLWidget::glLock(
  void )
{
  if ( GTK_IS_GL_AREA(PRIVATE(this)->glWidget) )
    gtk_gl_area_make_current( GTK_GL_AREA(PRIVATE(this)->glWidget) );
  PRIVATE(this)->glLockLevel++;
} // glLock()

/*!
  This method unlocks the locked GL context.
*/

void
SoGtkGLWidget::glUnlock(
  void )
{
  PRIVATE(this)->glLockLevel--;
} // glUnlock()

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::glSwapBuffers(
  void )
{
  if ( GTK_IS_GL_AREA(PRIVATE(this)->glWidget) )
    gtk_gl_area_swapbuffers( GTK_GL_AREA(PRIVATE(this)->glWidget) );
} // glSwapBuffers()

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::glFlushBuffer(
  void )
{
  glFlush();
} // glFlushBuffer()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::afterRealizeHook( // virtual, protected
  void )
{
  inherited::afterRealizeHook();
/*
  if ( ! GTK_IS_WIDGET(this->glWidget) )
    return;
  gtk_widget_add_events( GTK_WIDGET(this->glWidget),
    GDK_EXPOSURE_MASK |
    GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
    GDK_POINTER_MOTION_MASK );
*/
} // afterRealizeHook()

// *************************************************************************

/*!
  FIXME: write doc
*/

SbBool
SoGtkGLWidget::isOverlayRender(
  void ) const
{
  return THIS->currentIsNormal ? FALSE : TRUE;
} // isOverlayRender()

/*!
  FIXME: write doc
*/

void
SoGtkGLWidget::setOverlayRender(
  const SbBool enable )
{
#if SOGTK_DEBUG
  if ( THIS->glLockLevel > 0 ) {
  }
#endif

  THIS->currentIsNormal = enable ? FALSE : TRUE;
} // setOverlayRender()

// *************************************************************************

SoGtkGLWidgetP::SoGtkGLWidgetP(
  SoGtkGLWidget * publ )
{
  this->pub = publ;
  this->glLockLevel = 0;
  this->currentIsNormal = TRUE;
} // SoGtkGLWidgetP()

SoGtkGLWidgetP::~SoGtkGLWidgetP(
  void )
{
} // ~SoGtkGLWidgetP()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkGLWidgetRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

