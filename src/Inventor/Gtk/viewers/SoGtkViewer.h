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

#ifndef __SOGTK_VIEWER_H__
#define __SOGTK_VIEWER_H__

#include <math.h>

#include <Inventor/SbTime.h>
#include <Inventor/SoType.h>

#include <Inventor/Gtk/SoGtkRenderArea.h>

/*!
  \defgroup gtkviewers GTK+ Viewer Components
*/

class SoCallbackList;
class SoBaseColor;
class SoCamera;
class SoComplexity;
class SoDirectionalLight;
class SoDrawStyle;
class SoGetBoundingBoxAction;
class SoGroup;
class SoLightModel;
class SoMaterialBinding;
class SoNode;
class SoPolygonOffset;
class SoSensor;
class SoSeparator;
class SoSwitch;
class SoTimerSensor;

class SoGtkViewer;

typedef void SoGtkViewerCB( void * data, SoGtkViewer * viewer );
typedef void SoGtkViewerFPSCB( float fps, void * data, SoGtkViewer * viewer );
typedef void SoGtkViewerDecimationPercentageCB( float percentage, void * data,
                                                SoGtkViewer * viewer );

// *************************************************************************

class SoGtkViewer : public SoGtkRenderArea {
  typedef SoGtkRenderArea inherited;

public:
  enum Type {
    BROWSER,
    EDITOR
  };

  enum DecimationStrategy {
    NORMAL,
    FIXED_NUM_TRIANGLES,
    FRAMES_PER_SECOND,
    FIXED_PERCENTAGE
  };

  enum DrawStyle {
    VIEW_AS_IS          = 0x0001,
    VIEW_HIDDEN_LINE    = 0x0002,
    VIEW_NO_TEXTURE     = 0x0004,
    VIEW_LOW_COMPLEXITY = 0x0008,
    VIEW_LINE           = 0x0010,
    VIEW_POINT          = 0x0020,
    VIEW_BBOX           = 0x0040,
    VIEW_LOW_RES_LINE   = 0x0080,
    VIEW_LOW_RES_POINT  = 0x0100,
    VIEW_SAME_AS_STILL  = 0x0200,

    ALL_STYLES_COMBINED = 0x03ff
  };

  enum DrawType {
    STILL = 0,
    INTERACTIVE
  };

  enum BufferType {
    BUFFER_SINGLE,
    BUFFER_DOUBLE,
    BUFFER_INTERACTIVE
  };

  virtual void setCamera( SoCamera * cam );
  SoCamera * getCamera(void) const;

  virtual void setCameraType( SoType type );
  SoType getCameraType(void) const;

  virtual void viewAll(void);

  virtual void saveHomePosition(void);
  virtual void resetToHomePosition(void);

  virtual void setHeadlight( SbBool on );
  SbBool isHeadlight(void) const;
  SoDirectionalLight * getHeadlight(void) const;

  virtual void setDrawStyle( SoGtkViewer::DrawType type,
                             SoGtkViewer::DrawStyle style );
  SoGtkViewer::DrawStyle getDrawStyle( const SoGtkViewer::DrawType type ) const;

  virtual void setBufferingType( SoGtkViewer::BufferType type );
  SoGtkViewer::BufferType getBufferingType(void) const;

  virtual void setViewing( SbBool enable );
  SbBool isViewing(void) const;

  virtual void setCursorEnabled( SbBool enable );
  SbBool isCursorEnabled(void) const;

  void setAutoClipping( SbBool enable );
  SbBool isAutoClipping(void) const;

  virtual void setStereoViewing( SbBool enable );
  virtual SbBool isStereoViewing(void) const;

  void setStereoOffset( const float dist );
  float getStereoOffset(void) const;

  void setDetailSeek( const SbBool enable );
  SbBool isDetailSeek(void) const;

  void setSeekTime( const float seconds );
  float getSeekTime(void) const;

  void addStartCallback( SoGtkViewerCB * func, void * data = NULL );
  void addFinishCallback( SoGtkViewerCB * func, void * data = NULL );
  void removeStartCallback( SoGtkViewerCB * func, void * data = NULL );
  void removeFinishCallback( SoGtkViewerCB * func, void * data = NULL );

  void copyView( const SbTime eventTime );
  void pasteView( const SbTime eventTime );

  virtual void recomputeSceneSize(void);

  void setDecimationStrategy( const DecimationStrategy strategy );
  DecimationStrategy getDecimationStrategy(void) const;

  void setGoalNumberOfTriangles( const int32_t goal );
  int32_t getGoalNumberOfTriangles(void) const;

  void setGoalFramesPerSecond( const float goal );
  float getGoalFramesPerSecond(void) const;

  void setFixedPercentage( const float percent );
  float getFixedPercentage(void) const;

  void enableFullRenderingWhenStill( const SbBool enable );
  SbBool isFullRenderingWhenStill(void) const;
  SbBool isStillNow(void) const;

  void setFramesPerSecondCallback( SoGtkViewerFPSCB * callback, void * data );

  void setNumSamples( const int numFrames );
  int getNumSamples(void) const;

  void setDecimationPercentageCallback( SoGtkViewerDecimationPercentageCB * cb,
                                        void * data = NULL );
  float getCurrentDecimationPercentage(void) const;

  virtual void setDoubleBuffer( SbBool enable );

  virtual void setSceneGraph( SoNode * root );
  virtual SoNode * getSceneGraph(void);

protected:
  SoGtkViewer( GtkWidget * parent, const char * name, SbBool buildInsideParent,
               Type type, SbBool buildNow );
  ~SoGtkViewer(void);

  virtual void setSeekMode( SbBool enable );
  SbBool isSeekMode(void) const;
  SbBool seekToPoint( const SbVec2s screenpos );

  virtual void actualRedraw(void);
  SbBool processCommonEvents( GdkEvent * event );

  void interactiveCountInc(void);
  void interactiveCountDec(void);
  int getInteractiveCount(void) const;

  void setSeekDistance( const float distance );
  float getSeekDistance(void) const;
  void setSeekValueAsPercentage( const SbBool enable );
  SbBool isSeekValuePercentage(void) const;

  virtual void toggleCameraType(void);

private:
  // Seek functionality
  SoTimerSensor * seeksensor;
  static void seeksensorCB( void * data, SoSensor * );
  float seekperiod;
  SbBool inseekmode;
  SbBool seektopoint;
  SbVec3f camerastartposition, cameraendposition;
  float seekdistance;
  SbBool seekdistanceabs;

  // Camera handling
  SoCamera * camera;
  SbBool deletecamera;
  SbVec3f storedposition;
  SbRotation storedorientation;
  float storedheightval;

  // Headlight handling
  SbBool lighton;
  SoGroup * lightroot;

  // Drawstyles
  DrawStyle drawstyles[2];
  SbBool drawInteractiveAsStill(void) const;
  SbBool drawAsHiddenLine(void) const;
  SoGtkViewer::DrawStyle currentDrawStyle(void) const;
  void changeDrawStyle( SoGtkViewer::DrawStyle style );
  SoSwitch * drawstyleroot, * hiddenlineroot, * polygonoffsetparent;
  SoBaseColor * sobasecolor;
  SoComplexity * socomplexity;
  SoDrawStyle * sodrawstyle;
  SoLightModel * solightmodel;
  SoMaterialBinding * somaterialbinding;
  SoPolygonOffset * sopolygonoffset;

  // Automatic setting of clipping planes
  SbBool adjustclipplanes;
  void setClippingPlanes(void);
  SoGetBoundingBoxAction * autoclipbox;

  // Misc
  Type viewertype;
  SoType cameratype;
  SbBool viewmode;
  SbBool cursoron;

  SoCallbackList * interactionstartCallbacks, * interactionendCallbacks;

  static void interactivestartCB( void *, SoGtkViewer * thisp );
  static void interactiveendCB( void *, SoGtkViewer * thisp );
  int interactionnesting;

  BufferType buffertype;

  void moveCameraScreen( const SbVec2f & screenpos );

  SoSeparator * viewerroot;
  SoNode * userroot;

}; // class SoGtkViewer

// *************************************************************************

#endif // ! __SOGTK_VIEWER_H__
