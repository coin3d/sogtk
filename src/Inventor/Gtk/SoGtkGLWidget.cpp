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

static const char rcsid[] =
  "$Id$";

#include <assert.h>

#include <GL/gl.h>

#include <gtkgl/gtkglarea.h>

#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkGLWidget.h>

// *************************************************************************

/*!
  \class SoGtkGLWidget SoGtkGLWidget.h Inventor/Gtk/SoGtkGLWidget.h
  \brief The SoGtkGLWidget class contains an OpenGL canvas.
*/

// *************************************************************************

/*!
  \fn virtual void SoGtkGLWidget::redraw(void)
  This method will be called when we need a redraw. It must be overloaded in
  non-abstract subclasses.
*/

/*!
  \var SbBool SoGtkGLWidget::drawToFrontBuffer

  FIXME: write doc
*/

static const int SO_BORDER_THICKNESS = 2;

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoGtkGLWidget::SoGtkGLWidget(
  GtkWidget * const parent,
  const char * const name,
  const SbBool buildInsideParent,
  const int glModes,
  const SbBool buildNow )
  : inherited( parent ), waitForExpose( TRUE )
{
  this->glModeBits = glModes;

  this->glParent = NULL;
  this->glWidget = NULL;
  this->container = NULL;

  if ( gdk_gl_query() != FALSE ) {
    if ( buildNow )
      this->buildWidget( buildInsideParent ? parent : NULL );
  } else {
    SoDebugError::post( "SoGtkGLWidget::SoGtkGLWidget",
                        "OpenGL is not available on your display!" );
  }

  this->properties.mouseInput = FALSE;
  this->properties.keyboardInput = FALSE;
  this->properties.drawFrontBuff = FALSE;

  this->borderThickness = 0;
} // SoGtkGLWidget()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

GtkWidget *
SoGtkGLWidget::buildWidget(
  GtkWidget * parent )
{
  int glAttributes[16];
  int i = 0;

  if ( this->glModeBits & SO_GL_RGB ) {
    glAttributes[i] = GDK_GL_RGBA; i++;
  }
  if ( this->glModeBits & SO_GL_DOUBLE ) {
    glAttributes[i] = GDK_GL_DOUBLEBUFFER; i++;
  }
  if ( this->glModeBits & SO_GL_ZBUFFER ) {
    glAttributes[i] = GDK_GL_DEPTH_SIZE; i++;
    glAttributes[i] = 1; i++;
  }
  if ( this->glModeBits & SO_GL_STEREO ) {
    glAttributes[i] = GDK_GL_STEREO; i++;
  }

  glAttributes[i] = GDK_GL_STENCIL_SIZE; i++;
  glAttributes[i] = 1; i++;

  glAttributes[i] = GDK_GL_NONE; i++;

  this->glWidget = gtk_gl_area_new( glAttributes );
  assert( this->glWidget != NULL );
  gtk_widget_set_usize( this->glWidget, 100, 100 );
  
  gtk_widget_set_events( GTK_WIDGET(this->glWidget),
    GDK_EXPOSURE_MASK | GDK_KEY_PRESS_MASK |
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
    GDK_POINTER_MOTION_MASK );

  /* configure_event should probably be moved to SoGtkRenderArea? */
  gtk_signal_connect( GTK_OBJECT(this->glWidget), "realize",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::sGLInit), (void *) this );
  gtk_signal_connect( GTK_OBJECT(this->glWidget), "configure_event",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::sGLReshape), (void *) this );
  gtk_signal_connect( GTK_OBJECT(this->glWidget), "expose_event",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::sGLDraw), (void *) this );

  gtk_signal_connect( GTK_OBJECT(this->glWidget), "event",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::eventHandler), (void *) this );

  this->container = gtk_vbox_new( FALSE, 0 );
  gtk_container_set_border_width( GTK_CONTAINER(this->container),
    this->borderThickness );

  gtk_box_pack_start( GTK_BOX(this->container), GTK_WIDGET(this->glWidget),
      TRUE, TRUE, FALSE );

  gtk_widget_show( GTK_WIDGET(this->glWidget) );
//  gtk_widget_show( GTK_WIDGET(this->container) );

  this->setBaseWidget( this->container );
  this->subclassInitialized();

  return this->container;
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkGLWidget::eventFilter(
  GtkObject * object,
  GdkEvent * event )
{
  SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "[invoked]" );

/*
#if 0 // debug
  switch (e->type()) {
  case Event_MouseButtonPress:
//      SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "button press");
    break;
  case Event_MouseButtonRelease:
//      SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "button release");
    break;
  case Event_MouseButtonDblClick:
//      SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "dbl click");
    break;
  case Event_MouseMove:
//      SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "mousemove");
    break;
  case Event_Paint:
    SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "paint");
    break;
  case Event_Resize:
    SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "resize");
    break;
  case Event_FocusIn:
  case Event_FocusOut:
  case Event_Enter:
  case Event_Leave:
  case Event_Move:
  case Event_LayoutHint:
  case Event_ChildInserted:
  case Event_ChildRemoved:
    // ignored
    break;
  default:
    SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "type %d", e->type());
    break;
  }
#endif // debug

  SbBool stopevent = FALSE;

  // Redirect keyboard events to the GL canvas widget (workaround for
  // problems with Gtk focus policy).
  if ((e->type() == Event_KeyPress) || (e->type() == Event_KeyRelease))
    obj = this->glwidget;

  if (obj == (QObject *)this->glparent) {
    if (e->type() == Event_Resize) {
      QResizeEvent * r = (QResizeEvent *)e;
#if 0  // debug
      SoDebugError::postInfo("SoGtkGLWidget::eventFilter",
                             "resize %p: (%d, %d)",
                             this->glwidget,
                             r->size().width(), r->size().height());
#endif // debug

      this->borderwidget->resize(r->size());
      int newwidth = r->size().width() - 2 * this->borderthickness;
      int newheight = r->size().height() - 2 * this->borderthickness;

      ((PrivateGLWidget *)this->glwidget)->doRender(FALSE);
      this->glwidget->setGeometry(this->borderthickness,
                                  this->borderthickness,
                                  newwidth, newheight);
      ((PrivateGLWidget *)this->glwidget)->doRender(TRUE);

      this->sizeChanged(SbVec2s(newwidth, newheight));
#if 0 // debug
      SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "resize done");
#endif // debug
    }
  }
  else if (obj == (QObject *)this->glwidget) {
#if 0  // debug
    if (e->type() == Event_Resize)
      SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "gl widget resize");
#endif // debug
    // Pass this on further down the inheritance hierarchy of the SoGtk
    // components.
    this->processEvent(e);
  } else {
    // Handle in superclass.
    stopevent = inherited::eventFilter(obj, e);
  }

  return stopevent;
*/
  return FALSE;
} // eventFilter()

// *************************************************************************

/*!
  Specify that there should be a border around the OpenGL canvas (or not).

  \sa isBorder()
*/

void
SoGtkGLWidget::setBorder(
  const SbBool enable )
{
  this->borderThickness = enable ? SO_BORDER_THICKNESS : 0;
  // update canvas if it is created
} // setBorder()

// *************************************************************************

/*!
  Returns whether or not there's a border around the OpenGL canvas.

  \sa setBorder()
*/

SbBool
SoGtkGLWidget::isBorder(
  void ) const
{
  return (this->borderThickness != 0) ? TRUE : FALSE;
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
  if ( this->glWidget ) {
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
      this->glModeBits |= SO_GLX_DOUBLE;
    else
      this->glModeBits &= ~SO_GLX_DOUBLE;
  }
} // setDoubleBuffer()

// *************************************************************************

/*!
  Returns the status of the buffer mode.

  \sa setDoubleBuffer()
*/

SbBool
SoGtkGLWidget::isDoubleBuffer(
  void ) const
{
/*
  if ( this->glWidget )
    return this->glWidget->doubleBuffer();
  else
*/
  return (this->glModeBits & SO_GL_DOUBLE) ? TRUE : FALSE;
} // isDoubleBuffer()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkGLWidget::setDrawToFrontBufferEnable(
  const SbBool enable )
{
  this->properties.drawFrontBuff = enable;
} // setDrawToFrontBufferEnable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkGLWidget::isDrawToFrontBufferEnable(
  void ) const
{
  return this->properties.drawFrontBuff;
} // isDrawToFrontBufferEnable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkGLWidget::setGlxSize(
  const SbVec2s size )
{
  assert( this->container );

  GtkRequisition req = {
    size[0] + this->borderThickness * 2,
    size[1] + this->borderThickness * 2
  };

  gtk_widget_size_request( GTK_WIDGET(this->container), &req );
} // setGlxSize()

void
SoGtkGLWidget::setGLSize(
  const SbVec2s size )
{
  assert( this->container );

  GtkRequisition req = {
    size[0] + this->borderThickness * 2,
    size[1] + this->borderThickness * 2
  };

  gtk_widget_size_request( GTK_WIDGET(this->container), &req );
} // setGLSize()

// *************************************************************************

/*!
  Return the dimensions of the OpenGL canvas.
*/

const SbVec2s
SoGtkGLWidget::getGlxSize(
  void ) const
{
  return SbVec2s( this->glWidget->allocation.width,
                  this->glWidget->allocation.height );
} // getGlxSize()

const SbVec2s
SoGtkGLWidget::getGLSize(
  void ) const
{
  return SbVec2s( this->glWidget->allocation.width,
                  this->glWidget->allocation.height );
} // getGLSize()

// *************************************************************************

/*!
  Return the aspect ratio of the OpenGL canvas.
*/

float
SoGtkGLWidget::getGlxAspectRatio(
  void ) const
{
  assert( this->glWidget );
  return (float) this->glWidget->allocation.width /
         (float) this->glWidget->allocation.height;
} // getGlxAspectRatio()

float
SoGtkGLWidget::getGLAspectRatio(
  void ) const
{
  assert( this->glWidget );
  return (float) this->glWidget->allocation.width /
         (float) this->glWidget->allocation.height;
} // getGLAspectRatio()

// *************************************************************************

/*!
  Returns a pointer to the GtkGLArea.
*/

GtkWidget *
SoGtkGLWidget::getGtkGLArea(
  void )
{
  return this->glWidget;
} // getGltGLArea()

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
SoGtkGLWidget::widgetChanged(
  void )
{
} // widgetChanged()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkGLWidget::processEvent(
  GdkEvent * event )
{
  // FIXME: anything to do here?
} // processEvent()

// *************************************************************************

void
SoGtkGLWidget::glInit(
  void )
{
  glEnable( GL_DEPTH_TEST );
} // glInit()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::glInit(
  GtkWidget * widget )
{
  this->glInit();
//  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) ) )
//    return TRUE;

  /* glEnable( GL_DEPTH_TEST ); */

  return TRUE;
} // glInit()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::sGLInit( // static
  GtkWidget * widget,
  void * userData )
{
  SoGtkGLWidget * that = (SoGtkGLWidget *) userData;
  return that->glInit( widget );
} // sGLInit()

// *************************************************************************

void
SoGtkGLWidget::glReshape( // virtual
  int width,
  int height )
{
} // glReshape()


gint
SoGtkGLWidget::glReshape(
  GtkWidget * widget,
  GdkEventConfigure * event )
{
  assert( this->glWidget != NULL );
  this->glReshape( this->glWidget->allocation.width,
                   this->glWidget->allocation.height );
  return TRUE;
} // glReshape()

gint
SoGtkGLWidget::sGLReshape( // static
  GtkWidget * widget,
  GdkEventConfigure * event, 
  void * userData )
{
  SoGtkGLWidget * that = (SoGtkGLWidget *) userData;
  return that->glReshape( widget, event );
} // sGLReshape()

// *************************************************************************

void
SoGtkGLWidget::glRender( // virtual
  void )
{
} // glRender()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::glDraw(
  GtkWidget * widget,
  GdkEventExpose * event )
{
  this->glRender();
//  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) ) )
//    return TRUE;

//  this->redraw();
//  gtk_gl_area_swapbuffers( GTK_GL_AREA(this->glWidget) );

  return TRUE;
} // glDraw()


// Callback function for expose events.

gint
SoGtkGLWidget::sGLDraw( // static
  GtkWidget * widget,
  GdkEventExpose * event, 
  void * userData )
{
  SoGtkGLWidget * that = (SoGtkGLWidget *) userData;
  gint result = that->glDraw( widget, event );
  that->waitForExpose = FALSE; // Gets flipped from TRUE on first expose.
  return result;
} // sGLDraw()

// *************************************************************************

void
SoGtkGLWidget::glLock(
  void )
{
  if ( GTK_IS_GL_AREA(this->glWidget) )
    gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) );
} // glLock()

void
SoGtkGLWidget::glUnlock(
  void )
{
} // glUnlock()

void
SoGtkGLWidget::glSwapBuffers(
  void )
{
  if ( GTK_IS_GL_AREA(this->glWidget) )
    gtk_gl_area_swapbuffers( GTK_GL_AREA(this->glWidget) );
} // glSwapBuffers()

void
SoGtkGLWidget::glFlushBuffer(
  void )
{
  glFlush();
} // glFlushBuffer()

// *************************************************************************

/*!
*/

gint
SoGtkGLWidget::eventHandler( // static
  GtkWidget * widget,
  GdkEvent * event,
  void * closure )
{
  assert( closure != NULL );
  SoGtkGLWidget * component = (SoGtkGLWidget *) closure;
  component->processEvent( event );
  return FALSE;
} // eventHandler()

// *************************************************************************
