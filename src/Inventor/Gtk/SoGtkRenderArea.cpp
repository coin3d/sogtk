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

#include <gtkgl/gtkglarea.h>

#include <Inventor/SoSceneManager.h>
#include <Inventor/sensors/SoNodeSensor.h>
#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG

// #include <Inventor/Gtk/devices/SoGtkMouse.h>
// #include <Inventor/Gtk/devices/SoGtkKeyboard.h>

#include <Inventor/Gtk/SoGtkRenderArea.h>

// *************************************************************************

/*!
  \class SoGtkRenderArea SoGtkRenderArea.h Inventor/Gtk/SoGtkRenderArea.h
  \brief The SoGtkRenderArea class blablabla TODO

*/

// *************************************************************************

/*!
  \var SoGtkRenderAreaEventCB * SoGtkRenderArea::appEventHandler
  FIXME: write doc
*/

/*!
  \var void * SoGtkRenderArea::appEventHandlerData
  FIXME: write doc
*/

/*!
  \var SbPList * SoGtkRenderArea::deviceList
  FIXME: write doc
*/

// debug
#define SOGTK_RA_RESIZE_DEBUG 0

// *************************************************************************

/*!
  Constructor.
*/

SoGtkRenderArea::SoGtkRenderArea(
  GtkWidget * parent,
  const char * const name,
  SbBool buildInsideParent,
  SbBool getMouseInput,
  SbBool getKeyboardInput )
: inherited( parent, name, buildInsideParent,
    SO_GLX_RGB | SO_GLX_DOUBLE | SO_GLX_ZBUFFER,
    FALSE )
{
  this->constructor( getMouseInput, getKeyboardInput );
  this->buildWidget( buildInsideParent ? parent : NULL );
} // SoGtkRenderArea()

// *************************************************************************

/*!
  This constructor can only be used by subclasses.
*/

SoGtkRenderArea::SoGtkRenderArea(
  GtkWidget * parent,
  const char * const name,
  SbBool buildInsideParent,
  SbBool getMouseInput,
  SbBool getKeyboardInput,
  SbBool buildNow )
: inherited( parent, name, buildInsideParent,
    SO_GLX_RGB | SO_GLX_DOUBLE | SO_GLX_ZBUFFER,
    buildNow )
{
  this->constructor( getMouseInput, getKeyboardInput );
  if ( buildNow )
     this->buildWidget( buildInsideParent ? parent : NULL );
} // SoGtkRenderArea()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

GtkWidget *
SoGtkRenderArea::buildWidget(
  GtkWidget * parent )
{
  GtkWidget * widget = inherited::buildWidget( parent );
  this->setBorder( TRUE );

  if (this->getSize()[0] == -1)
    this->setSize(SbVec2s(400, 400));

  GtkWidget * glWidget = this->getGtkGLArea();
  g_return_val_if_fail( glWidget != NULL, NULL );

  gtk_signal_connect( GTK_OBJECT(glWidget), "configure_event",
    GTK_SIGNAL_FUNC(SoGtkRenderArea::sGLReshape), (void *) this );

  return widget;
} // buildWidget()

// *************************************************************************

/*!
*/

void
SoGtkRenderArea::constructor(
  SbBool getmouseinput,
  SbBool getkeyboardinput )
{
  this->sceneManager = new SoSceneManager;
  this->sceneManager->setRenderCallback( SoGtkRenderArea::renderCB, this );
  // FIXME: this should probably be done in the visibility
  // callback. 990203 mortene.
  this->sceneManager->activate();

  this->overlaySceneManager = new SoSceneManager;
  // FIXME: this should probably be done in the visibility
  // callback. 990203 mortene.
  this->overlaySceneManager->activate();

  this->colormap = NULL;
  this->cmNum = 0;
  this->overlayColormap= NULL;
  this->ocmNum = 0;

  this->clear = TRUE;
  this->clearOverlay = TRUE;
  this->autoRedraw = TRUE;

  this->selection = NULL;
  this->overlaySelection = NULL;

  this->mousedevice = NULL; // getmouseinput ? new SoGtkMouse : NULL;
  this->kbddevice = NULL; // getkeyboardinput ? new SoGtkKeyboard : NULL;
  this->deviceList = NULL;

  this->appEventHandler = NULL;
} // constructor()

// *************************************************************************

/*!
  Destructor.
*/

SoGtkRenderArea::~SoGtkRenderArea(
  void )
{
  delete this->sceneManager;
  delete this->overlaySceneManager;
  delete [] this->colormap;
  delete [] this->overlayColormap;
  delete this->deviceList;
//  delete this->mousedevice;
//  delete this->kbddevice;
} // ~SoGtkRenderArea()

// *************************************************************************

/*!
*/

void
SoGtkRenderArea::renderCB(
  void * data,
  SoSceneManager * scenemanager )
{
  ((SoGtkRenderArea *) data)->render();
} // renderCB()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setSceneGraph(
  SoNode * const newScene )
{
  this->sceneManager->setSceneGraph(newScene);
} // setSceneGraph()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoNode *
SoGtkRenderArea::getSceneGraph(
  void ) const
{
  return this->sceneManager->getSceneGraph();
} // getSceneGraph()
 
// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setOverlaySceneGraph(
  SoNode * const newScene )
{
  this->overlaySceneManager->setSceneGraph(newScene);
} // setOverlaySceneGraph()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoNode *
SoGtkRenderArea::getOverlaySceneGraph(
  void ) const
{
  return this->overlaySceneManager->getSceneGraph();
} // getOverlaySceneGraph()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::registerDevice(
  SoGtkDevice * const device )
{
  if ( ! this->deviceList )
    this->deviceList = new SbPList;
  this->deviceList->append( device );
//  device->setWindowSize( this->getGlxSize() );
} // registerDevice()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::unregisterDevice(
  SoGtkDevice * const device )
{
  int idx = this->deviceList->find( device );
  if ( idx != -1 )
    this->deviceList->remove( idx );
#if SOGTK_DEBUG
  if ( idx == -1 ) {
    SoDebugError::postWarning("SoGtkRenderArea::unregisterDevice",
                              "tried to remove device not in list");
  }
#endif // SOGTK_DEBUG
} // unregisterDevice()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setBackgroundColor(
  const SbColor & color )
{
  this->sceneManager->setBackgroundColor( color );
} // setBackgroundColor()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SbColor &
SoGtkRenderArea::getBackgroundColor(
  void ) const
{
  return this->sceneManager->getBackgroundColor();
} // getBackgroundColor()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setBackgroundIndex(
  const int idx )
{
  this->sceneManager->setBackgroundIndex( idx );
} // setBackgroundIndex()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

int
SoGtkRenderArea::getBackgroundIndex(
  void ) const
{
  return this->sceneManager->getBackgroundIndex();
} // getBackgroundIndex()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setOverlayBackgroundIndex(
  const int idx )
{
  this->overlaySceneManager->setBackgroundIndex( idx );
} // setBackgroundIndex()

// *************************************************************************

/*!
  FIXME: write function documentation
*/
int
SoGtkRenderArea::getOverlayBackgroundIndex(
  void ) const
{
  return this->overlaySceneManager->getBackgroundIndex();
} // getOverlayBackgroundIndex()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setColorMap(
  const int startIndex,
  const int num,
  const SbColor * const colors )
{
  delete [] this->colormap;
  this->cmStartIndex = startIndex;
  this->cmNum = num;
  this->colormap = new SbColor[num];
  for (int i = 0; i < num; i++) {
    this->colormap[i] = colors[i];
  }
} // setColorMap()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setOverlayColorMap(
  const int startIndex,
  const int num,
  const SbColor * const colors )
{
  delete [] this->overlayColormap;
  this->ocmStartIndex = startIndex;
  this->ocmNum = num;
  this->overlayColormap = new SbColor[num];
  for (int i = 0; i < num; i++) {
    this->overlayColormap[i] = colors[i];
  }
} // setOverlayColorMap()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setViewportRegion(
  const SbViewportRegion & region )
{
  this->sceneManager->getGLRenderAction()->setViewportRegion( region );
} // setViewportRegion()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SbViewportRegion &
SoGtkRenderArea::getViewportRegion(
  void ) const
{
  return this->sceneManager->getGLRenderAction()->getViewportRegion();
} // getViewportRegion()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setTransparencyType(
  const SoGLRenderAction::TransparencyType type )
{
  this->sceneManager->getGLRenderAction()->setTransparencyType(type);
} // setTransparencyType()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoGLRenderAction::TransparencyType
SoGtkRenderArea::getTransparencyType(
  void ) const
{
  return this->sceneManager->getGLRenderAction()->getTransparencyType();
} // getTransparencyType()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setAntialiasing(
  const SbBool smoothing,
  const int passes )
{
  this->sceneManager->setAntialiasing( smoothing, passes );
} // setAntialiasing()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::getAntialiasing(
  SbBool & smoothing,
  int & passes ) const
{
  this->sceneManager->getAntialiasing( smoothing, passes );
} // getAntialiasing()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setClearBeforeRender(
  const SbBool enable )
{
  this->clear = enable;
} // setClearBeforeRender()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkRenderArea::isClearBeforeRender(
  void ) const
{
  return this->clear;
} // isClearBeforeRender()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setClearBeforeOverlayRender(
  const SbBool enable )
{
  this->clearOverlay = enable;
} // setClearBeforeOverlayRender()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkRenderArea::isClearBeforeOverlayRender(
  void ) const
{
  return this->clearOverlay;
} // isClearBeforeOverlayRender()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setAutoRedraw(
  const SbBool enable )
{
  this->autoRedraw = enable;
} // setAutoRedraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkRenderArea::isAutoRedraw(
  void ) const
{
  return this->autoRedraw;
} // isAutoRedraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setRedrawPriority(
  const uint32_t priority )
{
  this->sceneManager->setRedrawPriority(priority);
  this->overlaySceneManager->setRedrawPriority(priority);
} // setRedrawPriority()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

uint32_t
SoGtkRenderArea::getRedrawPriority(
  void ) const
{
  return this->sceneManager->getRedrawPriority();
} // getRedrawPriority()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

uint32_t
SoGtkRenderArea::getDefaultRedrawPriority(
  void )
{
  return SoSceneManager::getDefaultRedrawPriority();
} // getDefaultRedrawPriority()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::render(
  void )
{
  assert( this->getGtkGLArea() != NULL );

  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(this->getGtkGLArea()) ) )
    return;

  this->redraw();

  if ( this->isDoubleBuffer() )
    gtk_gl_area_swapbuffers( GTK_GL_AREA(this->getGtkGLArea()) );
} // render()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::renderOverlay(
  void )
{
  assert( this->overlaySceneManager );
  this->overlaySceneManager->render( this->clearOverlay,
                                     this->clearOverlay );
} // renderOverlay()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::scheduleRedraw(
  void )
{
  this->sceneManager->scheduleRedraw();
} // scheduleRedraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::scheduleOverlayRedraw(
  void )
{
  this->sceneManager->scheduleRedraw();
} // scheduleOverlayRedraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::redrawOnSelectionChange(
  SoSelection * const selection )
{
  this->selection = selection;
} // redrawOnSelectionChange()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::redrawOverlayOnSelectionChange(
  SoSelection * const selection )
{
  this->overlaySelection = selection;
} // redrawOverlayOnSelectionChange()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setEventCallback(
  SoGtkRenderAreaEventCB * const fcn,
  void * const userData)
{
  this->appEventHandler = fcn;
  this->appEventHandlerData = userData;
} // setEventCallback()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setGLRenderAction(
  SoGLRenderAction * const action )
{
  this->sceneManager->setGLRenderAction( action );
} // setGLRenderAction()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoGLRenderAction *
SoGtkRenderArea::getGLRenderAction(
  void ) const
{
  return this->sceneManager->getGLRenderAction();
} // getGLRenderAction()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setOverlayGLRenderAction(
  SoGLRenderAction * const action )
{
  this->overlaySceneManager->setGLRenderAction( action );
} // setOverlayGLRenderAction()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoGLRenderAction *
SoGtkRenderArea::getOverlayGLRenderAction(
  void ) const
{
  return this->overlaySceneManager->getGLRenderAction();
} // getOverlayGLRenderAction()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setSceneManager(
  SoSceneManager * const manager )
{
  this->sceneManager = manager;
} // setSceneManager()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoSceneManager *
SoGtkRenderArea::getSceneManager(
  void ) const
{
  return this->sceneManager;
} // getSceneManager()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::setOverlaySceneManager(
  SoSceneManager * const manager )
{
  this->overlaySceneManager = manager;
} // setOverlaySceneManager()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SoSceneManager *
SoGtkRenderArea::getOverlaySceneManager(
  void ) const
{
  return this->overlaySceneManager;
} // getOverlaySceneManager()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::sizeChanged( // virtual
  const SbVec2s size )
{
  assert(this->sceneManager);

  this->sceneManager->setWindowSize(size);
  this->sceneManager->setSize(size);

  this->overlaySceneManager->setWindowSize(size);
  this->overlaySceneManager->setSize(size);

  // All devices belonging to the render area need to know the window
  // size to be able to convert from window system coordinates to
  // OpenGL/OIV system (i.e. y=0 at the bottom of the canvas).
//  if (this->mousedevice) this->mousedevice->setWindowSize(newSize);
//  if (this->kbddevice) this->kbddevice->setWindowSize(newSize);
  if (this->deviceList) {
//    for (int i=0; i < this->deviceList->getLength(); i++)
//      ((SoGtkDevice *)((*(this->deviceList))[i]))->setWindowSize(newSize);
  }
} // sizeChanged()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::redraw(
  void )
{
#if 0 // debug
  SoDebugError::postInfo("SoGtkRenderArea::redraw", "start");
#endif // debug

  // actualRedraw() can be overloaded by subclasses if there's need
  // for any magic when rendering (like multipass rendering for hidden
  // line drawstyle, for instance).
  this->actualRedraw();
} // redraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::actualRedraw(
  void )
{
  // NB! This is only called upon expose-type paint events. QGLWidget
  // takes care of making a current context and swapping buffers in
  // those cases.
  this->sceneManager->render(this->clear /* ?? */, this->clear /* ?? */);
} // actualRedraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::redrawOverlay(
  void )
{
  // See comments on SoGtkRenderArea::redraw().
  this->actualOverlayRedraw();
} // redrawOverlay()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::actualOverlayRedraw(
  void )
{
  // FIXME: defunct -- overlay planes not supported in Gtk. 990224 mortene
  this->overlaySceneManager->render(this->clear /* ?? */,
                                    this->clear /* ?? */);
} // actualOverlayRedraw()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const char *
SoGtkRenderArea::componentClassName(
  void ) const
{
  return "SoGtkRenderArea";
} // componentClassName()

// *************************************************************************

/*!
  Returns the default name of an SoGtkRenderArea widget. Should be overloaded
  by subclasses.
*/

const char *
SoGtkRenderArea::getDefaultWidgetName(
  void ) const
{
  return "SoGtkRenderArea";
} // getDefaultWidgetName()

// *************************************************************************

/*!
  Returns the default window caption string of this component. Should be
  overloaded by subclasses.
*/

const char *
SoGtkRenderArea::getDefaultTitle(
  void ) const
{
  return "Gtk RenderArea";
} // getDefaultTitle()

// *************************************************************************

/*!
  Returns the default icon title of this component. Should be overloaded
  by subclasses.
*/

const char *
SoGtkRenderArea::getDefaultIconTitle(
  void ) const
{
  return "Gtk RenderArea";
} // getDefaultIconTitle()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

SbBool
SoGtkRenderArea::invokeAppCB(
  GdkEvent * event )
{
  return (this->appEventHandler &&
          this->appEventHandler(this->appEventHandlerData, event));
} // invokeAppCB()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkRenderArea::processEvent(
  GdkEvent * event )
{
  // If there's an event handling callback registered, it should be
  // used before the OIV translation and handling.

  const SoEvent * ev = NULL;

  // Workaround for Gtk design problem: mouse position is not recorded
  // for anything but MouseMove events. We want to know the mouse
  // position for all event types.
  GtkWidget * w = this->getGtkGLArea();
//  SoGtkDevice::lasteventposition = w->mapFromGlobal(QCursor::pos());

  // Try list of user devices (if any) before mouse and keyboard.
  if (this->deviceList) {
    for (int i=0; i < this->deviceList->getLength(); i++) {
//      SoGtkDevice * dev = (SoGtkDevice *)(*(this->deviceList))[i];
//      ev = dev->translateEvent(event);
//      if (ev) break;
    }
  }


  // Try mouse device.
//  if (!ev && this->mousedevice)
//    ev = this->mousedevice->translateEvent(anyevent);

  // Try keyboard device.
//  if (!ev && this->kbddevice)
//    ev = this->kbddevice->translateEvent(anyevent);

  if (ev) {
    if (!this->overlaySceneManager->processEvent(ev)) {
      this->sceneManager->processEvent(ev);
    }
  }
} // processDevice()

// *************************************************************************

/*!
  FIXME: write doc
*/

gint
SoGtkRenderArea::glReshape(
  GtkWidget * widget,
  GdkEventConfigure * event )
{
  GtkWidget * glWidget = this->getGtkGLArea();
  if ( ! gtk_gl_area_make_current( GTK_GL_AREA(glWidget) ) )
    return TRUE;

  this->setViewportRegion( SbViewportRegion(
    glWidget->allocation.width, glWidget->allocation.height ) );

  return TRUE;
} // glReshape()

/*!
  FIXME: write doc
*/

gint
SoGtkRenderArea::sGLReshape( // static
  GtkWidget * widget,
  GdkEventConfigure * event,
  void * user)
{
  SoGtkRenderArea * that = (SoGtkRenderArea *) user;
  return that->glReshape( widget, event );
} // sGLReshape()

// *************************************************************************
