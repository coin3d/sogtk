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

#ifndef __SOGTK_RENDERAREA_H__
#define __SOGTK_RENDERAREA_H__

#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/Gtk/SoGtkGLWidget.h>

class SbColor;
class SbViewportRegion;
class SoNode;
class SoSelection;
class SoSceneManager;

class SoGtkDevice;
class SoGtkMouse;
class SoGtkKeyboard;

typedef SbBool SoGtkRenderAreaEventCB( void * user, GdkEvent * event );

// *************************************************************************

class SoGtkRenderArea : public SoGtkGLWidget
{
  typedef SoGtkGLWidget inherited;

public:
  SoGtkRenderArea( GtkWidget * parent = NULL,
                   const char * const name = NULL,
                   SbBool buildInsideParent = TRUE,
                   SbBool getMouseInput = TRUE,
                   SbBool getKeyboardInput = TRUE );


  virtual ~SoGtkRenderArea(void);

  virtual void setSceneGraph( SoNode * const newScene );
  virtual SoNode * getSceneGraph(void) const;
  void setOverlaySceneGraph( SoNode * const newScene );
  SoNode * getOverlaySceneGraph(void) const;
  void registerDevice( SoGtkDevice * const device );
  void unregisterDevice( SoGtkDevice * const device );
  void setBackgroundColor( const SbColor & color );
  const SbColor & getBackgroundColor(void) const;
  void setBackgroundIndex( const int idx );
  int getBackgroundIndex(void) const;
  void setOverlayBackgroundIndex( const int idx );
  int getOverlayBackgroundIndex(void) const;
  void setColorMap( const int startIndex, const int num,
                    const SbColor * const colors );
  void setOverlayColorMap( const int startIndex, const int num,
                           const SbColor * const colors );
  void setViewportRegion( const SbViewportRegion & newRegion );
  const SbViewportRegion & getViewportRegion(void) const;

  void setTransparencyType( const SoGLRenderAction::TransparencyType type );
  SoGLRenderAction::TransparencyType getTransparencyType(void) const;
  void setAntialiasing( const SbBool smoothing, const int numPasses );
  void getAntialiasing( SbBool & smoothing, int & numPasses ) const;
  void setClearBeforeRender( const SbBool enable );
  SbBool isClearBeforeRender(void) const;
  void setClearBeforeOverlayRender( const SbBool enable );
  SbBool isClearBeforeOverlayRender(void) const;
  void setAutoRedraw( const SbBool enable );
  SbBool isAutoRedraw(void) const;
  void setRedrawPriority( const uint32_t priority );
  uint32_t getRedrawPriority(void) const;
  static uint32_t getDefaultRedrawPriority(void);
  void render(void);
  void renderOverlay(void);
  void scheduleRedraw(void);
  void scheduleOverlayRedraw(void);
  void redrawOnSelectionChange( SoSelection * const selection );
  void redrawOverlayOnSelectionChange( SoSelection * const selection );
  void setEventCallback( SoGtkRenderAreaEventCB * const fcn,
                         void * const userData = NULL );
  void setGLRenderAction( SoGLRenderAction * const ra );
  SoGLRenderAction * getGLRenderAction(void) const;
  void setOverlayGLRenderAction( SoGLRenderAction * const ra );
  SoGLRenderAction * getOverlayGLRenderAction(void) const;
  void setSceneManager( SoSceneManager * const sm );
  SoSceneManager * getSceneManager(void) const;
  void setOverlaySceneManager( SoSceneManager * const sm );
  SoSceneManager * getOverlaySceneManager(void) const;

protected:
  SoGtkRenderArea( GtkWidget * parent,
                   const char * const name,
                   SbBool buildInsideParent,
                   SbBool getMouseInput,
                   SbBool getKeyboardInput,
                   SbBool buildNow );

  virtual void processEvent( GdkEvent * anyevent );

  GtkWidget * buildWidget( GtkWidget * parent );

  virtual const char * componentClassName(void) const;
  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void redraw(void);
  virtual void actualRedraw(void);
  virtual void redrawOverlay(void);
  virtual void actualOverlayRedraw(void);

  virtual void sizeChanged( const SbVec2s size );

  SoGtkRenderAreaEventCB * appEventHandler;
  void * appEventHandlerData;
  SbBool invokeAppCB( GdkEvent * anyevent );

  SbPList * deviceList;

private:
  void constructor( SbBool getmouseinput, SbBool getkeyboardinput );

  static void renderCB( void * data, SoSceneManager * scenemanager );

  SbColor * colormap;
  int cmNum;
  int cmStartIndex;
  SbColor * overlayColormap;
  int ocmNum;
  int ocmStartIndex;
  SbBool clear;
  SbBool clearOverlay;
  SbBool autoRedraw;
  SoSelection * selection;
  SoSelection * overlaySelection;
  SoSceneManager * sceneManager;
  SoSceneManager * overlaySceneManager;

  SoGtkMouse * mousedevice;
  SoGtkKeyboard * kbddevice;

  gint glReshape( GtkWidget * widget, GdkEventConfigure * event );
  static gint sGLReshape( GtkWidget * widget, GdkEventConfigure * event,
    void * userData );

}; // class SoGtkRenderArea

// *************************************************************************

#endif // ! __SOGTK_RENDERAREA_H__
