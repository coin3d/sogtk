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

class SoAnyPopupMenu;

// *************************************************************************

class SOGTK_DLL_API SoGtkFullViewer : public SoGtkViewer {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkFullViewer, SoGtkViewer);

public:
  enum BuildFlag {
    BUILD_NONE        = 0x00,
    BUILD_DECORATION  = 0x01,
    BUILD_POPUP       = 0x02,
    BUILD_ALL         = (BUILD_DECORATION | BUILD_POPUP)
  };

  void setDecoration( const SbBool enable );
  SbBool isDecoration(void) const;

  void setPopupMenuEnabled( const SbBool enable );
  SbBool isPopupMenuEnabled(void) const;

  GtkWidget * getAppPushButtonParent(void) const;
  void addAppPushButton( GtkWidget * button );
  void insertAppPushButton( GtkWidget * button, int index );
  void removeAppPushButton( GtkWidget * button );
  int findAppPushButton( GtkWidget * button ) const;
  int lengthAppPushButton(void) const;

  GtkWidget * getRenderAreaWidget(void);

  // overloaded
  virtual void setViewing( SbBool enable );
  virtual void setCamera( SoCamera * camera );
  virtual void hide(void);

protected:
  SoGtkFullViewer(
    GtkWidget * parent,
    const char * name,
    SbBool embed,
    SoGtkFullViewer::BuildFlag flag,
    SoGtkViewer::Type type,
    SbBool build );
  ~SoGtkFullViewer(void);

  virtual SbBool eventFilter( GtkWidget * object, GdkEvent * event );

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
  void openPopupMenu( const SbVec2s position );

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

  virtual SbBool processSoEvent( const SoEvent * const event );
  virtual void openViewerHelpCard(void);

  SoAnyPopupMenu * prefmenu;

protected:
  GtkWidget * leftDecoration;
  GtkWidget * leftWheel;
  GtkWidget * leftWheelLabel;
  char *      leftWheelStr;
  float       leftWheelVal;

  GtkWidget * bottomDecoration;
  GtkWidget * bottomWheel;
  GtkWidget * bottomWheelLabel;
  char *      bottomWheelStr;
  float       bottomWheelVal;

  GtkWidget * rightDecoration;
  GtkWidget * rightWheel;
  GtkWidget * rightWheelLabel;
  char *      rightWheelStr;
  float       rightWheelVal;

private:
  // hooks for SoAnyFullViewer

  // menu selections
  void selectedPrefs(void);
  void selectedViewing(void);
  void selectedDecoration(void);
  void selectedHeadlight(void);
  void copyviewSelected(void);
  void pasteviewSelected(void);

  // button selections
  void interactbuttonClicked(void);
  void viewbuttonClicked(void);
  void helpbuttonClicked(void);
  void homebuttonClicked(void);
  void sethomebuttonClicked(void);
  void viewallbuttonClicked(void);
  void seekbuttonClicked(void);

private:
  // friends and family
  class SoAnyFullViewer * common;
  friend class SoAnyFullViewer;
  class SoGtkFullViewerP * pimpl;
  friend class SoGtkFullViewerP;

}; // class SoGtkFullViewer

// *************************************************************************

#endif // ! SOGTK_FULLVIEWER_H
