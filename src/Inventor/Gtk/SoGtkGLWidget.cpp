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

static const char rcsid[] =
  "$Id$";

#include <assert.h>

#include <GL/gl.h>

#include <gtkgl/gtkglarea.h>

#include <Inventor/errors/SoDebugError.h>

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
  : inherited( parent )
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
  this->properties.drawFrontBuff = TRUE;

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
  int glAttributes[10];
  int i = 0;

  if ( this->glModeBits & SO_GLX_DOUBLE )
    glAttributes[i++] = GDK_GL_DOUBLEBUFFER;
//  if ( this->glModeBits & SO_GLX_ZBUFFER )
//    glAttributes[i++] = GDK_GL_DEPTH;
  if ( this->glModeBits & SO_GLX_RGB )
    glAttributes[i++] = GDK_GL_RGBA;
  if ( this->glModeBits & SO_GLX_STEREO )
    glAttributes[i++] = GDK_GL_STEREO;

  glAttributes[i++] = GDK_GL_NONE;

  this->glWidget = gtk_gl_area_new( glAttributes );
  
  gtk_widget_set_events( GTK_WIDGET(this->glWidget),
    GDK_EXPOSURE_MASK | GDK_KEY_PRESS_MASK );

  /* GTK_WIDGET_SET_FLAGS( GTK_WIDGET(this->glWidget), GTK_CAN_FOCUS ); */
  /* gtk_widget_grab_focus( GTK_WIDGET(this->glWidget) ); */

  gtk_signal_connect( GTK_OBJECT(this->glWidget), "expose_event",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::sGLDraw), (void *) this );
  gtk_signal_connect( GTK_OBJECT(this->glWidget), "configure_event",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::sGLReshape), (void *) this );
  gtk_signal_connect( GTK_OBJECT(this->glWidget), "realize",
    GTK_SIGNAL_FUNC(SoGtkGLWidget::sGLInit), (void *) this );

  /* gtk_widget_set_usize( GTK_WIDGET(this->glWidget), 32, 32 ); /* min. size */

  this->container = gtk_vbox_new( FALSE, 0 );
  gtk_container_set_border_width( GTK_CONTAINER(this->container),
    this->borderThickness );

  gtk_box_pack_start( GTK_BOX(this->container), GTK_WIDGET(this->glWidget),
      TRUE, TRUE, FALSE );

  gtk_container_add( GTK_CONTAINER(parent), GTK_WIDGET(this->container) );

  gtk_widget_show( GTK_WIDGET(this->glWidget) );
  gtk_widget_show( GTK_WIDGET(this->container) );

  this->setBaseWidget( this->container );
  this->subclassInitialized();

  return this->glWidget;
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
/*

#if 0 // debug
  SoDebugError::postInfo("SoGtkGLWidget::eventFilter", "obj: %p", obj);
#endif // debug

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
  }
  else {
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
  return (this->glModeBits & SO_GLX_DOUBLE) ? TRUE : FALSE;
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

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::glInit(
  GtkWidget * widget )
{
  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) ) )
    return TRUE;

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

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::glReshape(
  GtkWidget * widget,
  GdkEventConfigure * event )
{
  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) ) )
    return TRUE;

// FIXME: get viewport set/updated on resizes!

/*
  glViewport( 0, 0,
    this->glWidget->allocation.width, this->glWidget->allocation.height );
*/

  return TRUE;
} // glReshape()

/*!
  FIXME: write doc
*/

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

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::glDraw(
  GtkWidget * widget,
  GdkEventExpose * event )
{
  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->glWidget) ) )
    return TRUE;

  this->redraw();
  gtk_gl_area_swapbuffers( GTK_GL_AREA(this->glWidget) );

  return TRUE;
} // glDraw()

/*!
  FIXME: write doc
*/

gint
SoGtkGLWidget::sGLDraw( // static
  GtkWidget * widget,
  GdkEventExpose * event, 
  void * userData )
{
  SoGtkGLWidget * that = (SoGtkGLWidget *) userData;
  return that->glDraw( widget, event );
} // sGLDraw()

// *************************************************************************
