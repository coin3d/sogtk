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

#ifndef __SOGTK_EXAMINERVIEWER_H__
#define __SOGTK_EXAMINERVIEWER_H__

#include <Inventor/SbLinear.h>

#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>

class SbSphereSheetProjector;
class SoTimerSensor;
class SoSeparator;
class SoSwitch;
class SoTranslation;
class SoScale;

// *************************************************************************

class SoGtkExaminerViewer : public SoGtkFullViewer {
  typedef SoGtkFullViewer inherited;

public:
  SoGtkExaminerViewer(
    GtkWidget * parent = NULL,
    const char * name = NULL,
    SbBool buildInsideParent = TRUE,
    SoGtkFullViewer::BuildFlag flag = BUILD_ALL,
    SoGtkViewer::Type type = BROWSER );
  ~SoGtkExaminerViewer(void);

  void setFeedbackVisibility(const SbBool on);
  SbBool isFeedbackVisible(void) const;

  void setFeedbackSize(const int size);
  int getFeedbackSize(void) const;

  void setAnimationEnabled(SbBool on);
  SbBool isAnimationEnabled(void);

  void stopAnimating(void);
  SbBool isAnimating(void);

  virtual void setViewing(SbBool on);
  virtual void setCamera(SoCamera * cam);
  virtual void setCursorEnabled(SbBool on);

protected:
  SoGtkExaminerViewer(GtkWidget * parent,
                     const char * name,
                     SbBool buildInsideParent,
                     SoGtkFullViewer::BuildFlag flag,
                     SoGtkViewer::Type type,
                     SbBool buildNow);

  virtual void leftWheelMotion(float val);
  virtual void bottomWheelMotion(float val);
  virtual void rightWheelMotion(float val);

  virtual GtkWidget * makeSubPreferences(GtkWidget * parent);
  virtual void createViewerButtons(GtkWidget * parent, SbPList * buttonlist);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void openViewerHelpCard(void);

  virtual void processEvent(GdkEvent * anyevent);
  virtual void setSeekMode(SbBool on);
  virtual void actualRedraw(void);

private:
  enum ViewerMode {
    INTERACT,
    EXAMINE, DRAGGING,
    WAITING_FOR_SEEK,
    ZOOMING,
    WAITING_FOR_PAN, PANNING,
  };

  ViewerMode currentMode;

  void reorientCamera(const SbRotation & rot);
  void zoom(const float diffvalue);
  void zoomByCursor(const SbVec2f & mousepos);
  void spin(const SbVec2f & mousepos);
  void pan(const SbVec2f & mousepos);

  void setMode(const ViewerMode mode);
  void setModeFromState(const unsigned int state);

  void drawAxisCross(void);
  void drawArrow(void);

  SbBool axisCrossOn;
  int axisCrossSize;

  struct {
    GtkWidget * orthogonal;
    GtkWidget * perspective;
  } pixmaps;

  void constructor(SbBool buildNow);
  static void visibilityCB(void * data, SbBool visible);

//  QTimer * spinDetectTimer;
  SbBool spinAnimating, animatingAllowed;
  SoTimerSensor * timerTrigger;
  static void timertriggeredCB(void * data, SoSensor *);

  SbSphereSheetProjector * projector;

  void setCursorRepresentation(const ViewerMode mode);
//  QCursor * panCursor, * rotateCursor;
//  QCursor * defaultCursor, * zoomCursor;

  SbVec2f prevMousePosition;
  SbPlane panningPlane;

  SbRotation spinIncrement;
  int spinSampleCounter;

  GtkWidget * cameraToggleButton;
  GtkWidget * feedbackLabel1, * feedbackLabel2;
  GtkWidget * feedbackWheel;
  GtkWidget * feedbackEdit;
  void setEnableFeedbackControls(const SbBool flag);

private: // slots:
  // Pref sheet.
  void spinAnimationToggled(SbBool flag);
  void feedbackVisibilityToggle(SbBool flag);
  void feedbackEditPressed();
  void feedbackWheelPressed();
  void feedbackSizeChanged(float val);
  void feedbackWheelReleased();
  // Button row.
  void cameratoggleClicked();

}; // class SoGtkExaminerViewer

// *************************************************************************

#endif // ! __SOGTK_EXAMINERVIEWER_H__
