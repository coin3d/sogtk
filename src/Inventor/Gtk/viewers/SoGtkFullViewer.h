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

#ifndef __SOGTK_FULLVIEWER_H__
#define __SOGTK_FULLVIEWER_H__

#include <Inventor/Gtk/viewers/SoGtkViewer.h>

class QPushButton;
class QLayout;
class QLabel;
class QPopupMenu;
class QLineEdit;
class QSlider;
class QtThumbwheel;

// *************************************************************************

class SoGtkFullViewer : public SoGtkViewer {
  typedef SoGtkViewer inherited;

public:
  enum BuildFlag {
    BUILD_NONE = 0x00,
    BUILD_DECORATION = 0x01,
    BUILD_POPUP = 0x02,
    BUILD_ALL = 0xff,
  };

  void setDecoration(const SbBool on);
  SbBool isDecoration(void) const;

  void setPopupMenuEnabled(const SbBool on);
  SbBool isPopupMenuEnabled(void) const;

  GtkWidget * getAppPushButtonParent(void) const;
  void addAppPushButton(GtkWidget * newButton);
  void insertAppPushButton(GtkWidget * newButton, int index);
  void removeAppPushButton(GtkWidget * oldButton);
  int findAppPushButton(GtkWidget * oldButton) const;
  int lengthAppPushButton(void) const;

  GtkWidget * getRenderAreaWidget(void);

  // internal
  // These are overloaded to update user interface components.
  virtual void setViewing(SbBool on);
  virtual void setHeadlight(SbBool on);
  virtual void setDrawStyle(SoGtkViewer::DrawType type,
                            SoGtkViewer::DrawStyle style);
  virtual void setBufferingType(SoGtkViewer::BufferType type);
  virtual void setCamera(SoCamera * cam);
  virtual void hide(void);

protected:
  SoGtkFullViewer(GtkWidget * parent, const char * name, SbBool buildInsideParent,
                 BuildFlag flag, Type type,
                 SbBool buildNow);
  ~SoGtkFullViewer(void);

  virtual SbBool eventFilter(GtkObject * obj, GdkEvent * e);

  GtkWidget * buildWidget(GtkWidget * parent);

  virtual void buildDecoration(GtkWidget * parent);
  virtual GtkWidget * buildLeftTrim(GtkWidget * parent);
  virtual GtkWidget * buildBottomTrim(GtkWidget * parent);
  virtual GtkWidget * buildRightTrim(GtkWidget * parent);
  GtkWidget * buildAppButtons(GtkWidget * parent);
  GtkWidget * buildViewerButtons(GtkWidget * parent);
  virtual void createViewerButtons(GtkWidget * parent, SbPList * buttonlist);

  virtual void buildPopupMenu(void);
  void setPopupMenuString(const char * title);
  GtkWidget * buildFunctionsSubmenu(GtkWidget * popup);
  GtkWidget * buildDrawStyleSubmenu(GtkWidget * popup);

  virtual GtkWidget * makeSubPreferences(GtkWidget * parent);
  void setPrefSheetString(const char * title);

  virtual void leftWheelStart(void);
  virtual void leftWheelMotion(float);
  virtual void leftWheelFinish(void);
  float getLeftWheelValue(void) const;
  virtual void bottomWheelStart(void);
  virtual void bottomWheelMotion(float);
  virtual void bottomWheelFinish(void);
  float getBottomWheelValue(void) const;
  virtual void rightWheelStart(void);
  virtual void rightWheelMotion(float);
  virtual void rightWheelFinish(void);
  float getRightWheelValue(void) const;

  void setLeftWheelString(const char * name);
  void setBottomWheelString(const char * name);
  void setRightWheelString(const char * name);

  virtual void openViewerHelpCard(void);

private:
  enum {
    LEFTDECORATION,
    BOTTOMDECORATION,
    RIGHTDECORATION,

    FIRSTDECORATION = LEFTDECORATION,
    LASTDECORATION = RIGHTDECORATION,
    NUMDECORATIONS = LASTDECORATION - FIRSTDECORATION + 1,
  };

  GtkWidget * makePreferencesWindow(void);
  GtkWidget * makeSeekPreferences(GtkWidget * parent);
  GtkWidget * makeSeekDistancePreferences(GtkWidget * parent);
  GtkWidget * makeZoomPreferences(GtkWidget * parent);
  GtkWidget * makeAutoclipPreferences(GtkWidget * parent);


  GtkWidget * viewerwidget, * canvasparent, * canvas;
  SbBool decorations;
  float wheelvalues[NUMDECORATIONS];
  QString wheelstrings[NUMDECORATIONS];
  GtkWidget * wheels[NUMDECORATIONS];
  QLabel * wheellabels[NUMDECORATIONS];
  GtkWidget * decorform[NUMDECORATIONS];

  SbString menutitle;
  QPopupMenu * prefmenu;
  SbBool menuenabled;

  QLayout * mainlayout;
  void showDecorationWidgets(SbBool onOff);

  QLayout * appbuttonlayout;
  GtkWidget * appbuttonform;
  SbPList * appbuttonlist;
  void layoutAppButtons(GtkWidget * form);

  SbPList * viewerbuttons;

  GtkWidget * prefwindow;
  SbString prefwindowtitle;

  QSlider * zoomslider;
  QLineEdit * zoomfield, * zoomrangefrom, * zoomrangeto;
  SbVec2f zoomrange;

  void setCameraZoom(const float zoom);
  float getCameraZoom(void);
  void setZoomSliderPosition(float zoom);
  void setZoomFieldString(float zoom);

  QtThumbwheel * seekdistancewheel;
  QLineEdit * seekdistancefield;

  QLabel * nearclippinglabel, * farclippinglabel;
  QtThumbwheel * nearclippingwheel, * farclippingwheel;
  QLineEdit * nearclippingedit, * farclippingedit;

  void setDrawStyleMenuActivation(SoQtViewer::DrawType type,
                                  SoQtViewer::DrawStyle val);

  void setEnabledClippingWidgets(SbBool flag);

private slots:
  // Thumbwheels.
  void leftWheelPressed(void);
  void leftWheelChanged(float value);
  void leftWheelReleased(void);
  void rightWheelPressed(void);
  void rightWheelChanged(float value);
  void rightWheelReleased(void);
  void bottomWheelPressed(void);
  void bottomWheelChanged(float value);
  void bottomWheelReleased(void);

  // Button row.
  void interactbuttonToggled(SbBool);
  void viewbuttonToggled(SbBool);
  void helpbuttonClicked();
  void homebuttonClicked();
  void sethomebuttonClicked();
  void viewallbuttonClicked();
  void seekbuttonClicked();

  // Menu items.
  void selectedViewing();
  void selectedDecoration();
  void selectedHeadlight();
  void copyviewSelected();
  void pasteviewSelected();
  void drawstyleActivated(int);
  void selectedPrefs();

  // Pref sheet.
  //  seek settings
  void seekAnimationTimeChanged(const char *);
  void seekAnimationTimeChanged(const QString &);
  void seekDetailToggled(int id);
  void seekDistanceWheelChanged(float);
  void seekDistanceEdit();
  void seekDistanceTypeToggle(int id);
  //  zoom settings
  void zoomSliderMoved(int val);
  void zoomFieldChanged();
  void zoomRangeChanged1();
  void zoomRangeChanged2();
  //  clipping settings
  void clippingToggled(SbBool flag);
  void nearclippingwheelMoved(float val);
  void farclippingwheelMoved(float val);
  void nearclipEditPressed();
  void farclipEditPressed();

  // Generic slots.
  void increaseInteractiveCount();
  void decreaseInteractiveCount();

}; // class SoGtkFullViewer

// *************************************************************************

#endif // ! __SOGTK_FULLVIEWER_H__
