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

//  $Id$

#ifndef SOGTK_EXAMINERVIEWER_H
#define SOGTK_EXAMINERVIEWER_H

#include <Inventor/SbLinear.h>

#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>

class SbSphereSheetProjector;
class SoTimerSensor;
class SoSeparator;
class SoSwitch;
class SoTranslation;
class SoScale;

class SoAnyExaminerViewer;

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkExaminerViewer : public SoGtkFullViewer {
  typedef SoGtkFullViewer inherited;
  friend class SoAnyExaminerViewer;

public:
  SoGtkExaminerViewer(
    GtkWidget * parent = NULL,
    const char * name = NULL,
    SbBool embed = TRUE,
    SoGtkFullViewer::BuildFlag flags = BUILD_ALL,
    SoGtkViewer::Type type = BROWSER );
  ~SoGtkExaminerViewer(void);

  virtual void setViewing( SbBool enable );

  void setAnimationEnabled( const SbBool enable );
  SbBool isAnimationEnabled(void) const;
      
  void stopAnimating(void);
  SbBool isAnimating(void) const;
	  
  void setFeedbackVisibility( const SbBool enable );
  SbBool isFeedbackVisible(void) const;

  void setFeedbackSize( const int size );
  int getFeedbackSize(void) const;

  virtual void setCamera( SoCamera * camera );

protected:
  SoGtkExaminerViewer(
    GtkWidget * parent,
    const char * name,
    SbBool embed,
    SoGtkFullViewer::BuildFlag flags,
    SoGtkViewer::Type type,
    SbBool build );

  virtual void actualRedraw(void);

  virtual void setSeekMode( SbBool enable );

  virtual void processEvent( GdkEvent * event );
  virtual void afterRealizeHook(void);

  virtual SbBool processSoEvent( const SoEvent * const event );

  virtual void leftWheelStart(void);
  virtual void leftWheelMotion( float value );
  virtual void bottomWheelStart(void);
  virtual void bottomWheelMotion( float value );
  virtual void rightWheelMotion( float value );

  virtual void openViewerHelpCard(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void createViewerButtons( GtkWidget * parent, SbPList * buttonlist );
  void camerabuttonClicked(void);
  static void camerabuttonClickedCB( GtkWidget *, gpointer );

  virtual void createPrefSheet(void);

  virtual GtkWidget * makeSubPreferences( GtkWidget * parent );

private:
  void constructor( const SbBool build );

  enum ViewerMode {
    INTERACT,
    EXAMINE, DRAGGING,
    WAITING_FOR_SEEK,
    ZOOMING,
    WAITING_FOR_PAN, PANNING
  };

  ViewerMode currentMode;

  void setMode( const ViewerMode mode );
  void setModeFromState( const unsigned int state );

  struct {
    GtkWidget * orthogonal;
    GtkWidget * perspective;
  } pixmaps;


  void setCursorRepresentation( const ViewerMode mode );

  GtkWidget * cameraToggleButton;
  GtkWidget * feedbackLabel1;
  GtkWidget * feedbackLabel2;
  GtkWidget * feedbackWheel;
  GtkWidget * feedbackEdit;
  void setEnableFeedbackControls( const SbBool flag );

private: // slots:
  // Pref sheet.
  void spinAnimationToggled( SbBool flag );
  void feedbackVisibilityToggle( SbBool flag );
  void feedbackEditPressed(void);
  void feedbackWheelPressed(void);
  void feedbackSizeChanged( float val );
  void feedbackWheelReleased(void);
  // Button row.
  void cameratoggleClicked(void);

private:
  SoAnyExaminerViewer * const common;

}; // class SoGtkExaminerViewer

// *************************************************************************

#endif // ! SOGTK_EXAMINERVIEWER_H
