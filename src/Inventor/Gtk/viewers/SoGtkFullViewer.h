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

// $Id$

#ifndef SOGTK_FULLVIEWER_H
#define SOGTK_FULLVIEWER_H

#include <Inventor/Gtk/viewers/SoGtkViewer.h>

class SoAnyFullViewer;
class SoAnyPopupMenu;
struct SoGtkViewerButton;

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkFullViewer : public SoGtkViewer {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkFullViewer, SoGtkViewer);
  friend class SoAnyFullViewer;

public:
  enum BuildFlag {
    BUILD_NONE = 0x00,
    BUILD_DECORATION = 0x01,
    BUILD_POPUP = 0x02,
    BUILD_ALL = 0xff
  };

  void setDecoration( const SbBool enable );
  SbBool isDecoration(void) const;

  void setPopupMenuEnabled( const SbBool enable );
  SbBool isPopupMenuEnabled(void) const;

  GtkWidget * getAppPushButtonParent(void) const;
  void addAppPushButton( GtkWidget * button );
  void insertAppPushButton( GtkWidget * button , int index);
  void removeAppPushButton( GtkWidget * button );
  int findAppPushButton( GtkWidget * button ) const;
  int lengthAppPushButton(void) const;

  GtkWidget * getRenderAreaWidget(void);

  // internal
  // These are overloaded to update user interface components.
  virtual void setViewing( SbBool enable );
  virtual void setCamera( SoCamera * camera );
  virtual void hide(void);

protected:
  SoGtkFullViewer(
    GtkWidget * parent,
    const char * name,
    SbBool buildInsideParent,
    BuildFlag flag,
    Type type,
    SbBool buildNow );
  ~SoGtkFullViewer(void);

  virtual SbBool eventFilter( GtkObject * object, GdkEvent * event );

  GtkWidget * buildWidget( GtkWidget * parent );

  virtual void buildDecoration( GtkWidget * parent );
  virtual GtkWidget * buildLeftTrim( GtkWidget * parent );
  virtual GtkWidget * buildBottomTrim( GtkWidget * parent );
  virtual GtkWidget * buildRightTrim( GtkWidget * parent );
  GtkWidget * buildAppButtons( GtkWidget * parent );
  GtkWidget * buildViewerButtons( GtkWidget * parent );
  virtual void createViewerButtons( GtkWidget * parent, SbPList * buttonlist );

  virtual void buildPopupMenu(void);
  void setPopupMenuString( const char * title );
  void openPopupMenu(const SbVec2s position);

  virtual GtkWidget * makeSubPreferences( GtkWidget * parent );
  void setPrefSheetString( const char * title );

  virtual void leftWheelStart(void);
  virtual void leftWheelMotion( float value );
  virtual void leftWheelFinish(void);
  float getLeftWheelValue(void) const;
  void setLeftWheelValue( const float value );

  virtual void bottomWheelStart(void);
  virtual void bottomWheelMotion( float value );
  virtual void bottomWheelFinish(void);
  float getBottomWheelValue(void) const;
  void setBottomWheelValue( const float value );

  virtual void rightWheelStart(void);
  virtual void rightWheelMotion( float value );
  virtual void rightWheelFinish(void);
  float getRightWheelValue(void) const;
  void setRightWheelValue( const float value );

  void setLeftWheelString( const char * name );
  void setBottomWheelString( const char * name );
  void setRightWheelString( const char * name );

  virtual SbBool processSoEvent(const SoEvent * const event);
  virtual void openViewerHelpCard(void);

  SoAnyPopupMenu * prefmenu;

  SbBool addPointer( const char * name, void * data );
  SbBool setCurrentPointer( const char * name );
  const char * getCurrentPointer(void) const;

protected:
  GtkWidget * leftDecoration;
  GtkWidget * leftWheel;
  GtkWidget * leftWheelLabel;
  char * leftWheelStr;
  float leftWheelVal;

  GtkWidget * bottomDecoration;
  GtkWidget * bottomWheel;
  GtkWidget * bottomWheelLabel;
  char * bottomWheelStr;
  float bottomWheelVal;

  GtkWidget * rightDecoration;
  GtkWidget * rightWheel;
  GtkWidget * rightWheelLabel;
  char * rightWheelStr;
  float rightWheelVal;

private:
  GtkWidget * makePreferencesWindow(void);
  GtkWidget * makeSeekPreferences( GtkWidget * parent );
  GtkWidget * makeSeekDistancePreferences( GtkWidget * parent );
  GtkWidget * makeZoomPreferences( GtkWidget * parent );
  GtkWidget * makeAutoclipPreferences( GtkWidget * parent );
  GtkWidget * makeStereoPreferences( GtkWidget * parent );
  
  GtkWidget * viewerWidget, * canvasParent, * canvas;
  SbBool decorations;

  SbString menuTitle;
  SbBool menuEnabled;

  GtkWidget * mainLayout;
  void showDecorationWidgets( SbBool enable );

  GtkWidget * appButtonLayout;
  GtkWidget * appButtonForm;
  SbPList * appButtonList;
  void layoutAppButtons( GtkWidget * form );

  SbPList * viewerButtons;

  GtkWidget * prefwindow;
  SbString prefwindowtitle;

  GtkWidget * zoomslider;
  GtkWidget * zoomfield, * zoomrangefrom, * zoomrangeto;
  SbVec2f zoomrange;

  void setCameraZoom( const float zoom );
  float getCameraZoom(void);
  void setZoomSliderPosition( float zoom );
  void setZoomFieldString( float zoom );

  GtkWidget * seekdistancewheel;
  GtkWidget * seekdistancefield;

  GtkWidget * clippingtable ;
  GtkWidget * nearclippinglabel, * farclippinglabel;
  GtkWidget * nearclippingwheel, * farclippingwheel;
  GtkWidget * nearclippingedit, * farclippingedit;

  void setEnabledClippingWidgets( SbBool flag );

private: //  slots:
  // Thumbwheels.
  void leftWheelPressed(void);
  void leftWheelChanged( float value );
  void leftWheelReleased(void);
  void rightWheelPressed(void);
  void rightWheelChanged( float value );
  void rightWheelReleased(void);
  void bottomWheelPressed(void);
  void bottomWheelChanged( float value );
  void bottomWheelReleased(void);

#if 0
  // Button row.
  void interactbuttonToggled(SbBool);
  void viewbuttonToggled(SbBool);
#endif
  // Menu items.
  void selectedViewing();
  void selectedDecoration();
  void selectedHeadlight();
  void copyviewSelected();
  void pasteviewSelected();
  void selectedPrefs();

  // Pref sheet.
  static void preferencesDestroyed(
    GtkObject           *object,
    gpointer            closure);

  //  seek settings
  static void seekAnimationTimeChanged(
    GtkEditable     	*editable,
    gpointer         	closure);
  static void seekDetailToggled(
    GtkToggleButton	*button,
    gpointer		closure );
  static void seekDistanceWheelChanged(
    GtkWidget		*wheel,
    gpointer		closure );
  static void seekDistanceEdit(
    GtkEditable     	*editable,
    gpointer         	closure);
  static void seekDistanceTypeToggle(
    GtkToggleButton* button,
    gpointer		closure );

  //  zoom settings
  static void zoomSliderMoved(
    GtkAdjustment *adjustment,
    gpointer closure);
  static void zoomFieldChanged(
    GtkEditable     	*editable,
    gpointer         	closure);
  static void zoomRangeChanged1(
    GtkEditable     	*editable,
    gpointer         	closure);
  static void zoomRangeChanged2(
    GtkEditable     	*editable,
    gpointer         	closure);

  //  clipping settings
  static void clippingToggled(
    GtkToggleButton 	*toggle_button,
    gpointer         	closure);
  static void nearclippingwheelMoved(
    GtkWidget		*w,
    gpointer		closure );
  static void farclippingwheelMoved(
    GtkWidget		*w,
    gpointer		closure );
  static void nearclipEditPressed(
    GtkWidget		*w,
    gpointer		closure );
  static void farclipEditPressed(
    GtkWidget		*w,
    gpointer		closure );

  // Stereo settings
  static void stereoToggled(
    GtkToggleButton	*toggle_button,
    gpointer		closure );

  // Generic Signal Handlers.
  static void increaseInteractiveCount(
    GtkWidget		*w,
    gpointer		closure );
  static void decreaseInteractiveCount(
    GtkWidget		*w,
    gpointer		closure );

private:
  static SoGtkViewerButton SoGtkFullViewerButtons[];
  SoGtkViewerButton * buttons;

  GtkWidget * findButton( const char * const keyword );

  void interactbuttonClicked(void);
  static void interactbuttonClickedCB( GtkWidget *, gpointer );
  void viewbuttonClicked(void);
  static void viewbuttonClickedCB( GtkWidget *, gpointer );

  void helpbuttonClicked(void);
  static void helpbuttonClickedCB( GtkWidget *, gpointer );
  void homebuttonClicked(void);
  static void homebuttonClickedCB( GtkWidget *, gpointer );
  void sethomebuttonClicked(void);
  static void sethomebuttonClickedCB( GtkWidget *, gpointer );
  void viewallbuttonClicked(void);
  static void viewallbuttonClickedCB( GtkWidget *, gpointer );
  void seekbuttonClicked(void);
  static void seekbuttonClickedCB( GtkWidget *, gpointer );

  static void leftwheelPressedCB( GtkWidget *, gpointer );
  static void leftwheelMovedCB( GtkWidget *, gpointer );
  static void leftwheelReleasedCB( GtkWidget *, gpointer );
  static void bottomwheelPressedCB( GtkWidget *, gpointer );
  static void bottomwheelMovedCB( GtkWidget *, gpointer );
  static void bottomwheelReleasedCB( GtkWidget *, gpointer );
  static void rightwheelPressedCB( GtkWidget *, gpointer );
  static void rightwheelMovedCB( GtkWidget *, gpointer );
  static void rightwheelReleasedCB( GtkWidget *, gpointer );

private:
  SoAnyFullViewer * const common;

}; // class SoGtkFullViewer

// *************************************************************************

#endif // ! SOGTK_FULLVIEWER_H
