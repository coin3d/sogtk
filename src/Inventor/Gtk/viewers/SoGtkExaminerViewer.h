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

// class SbSphereSheetProjector;
// class SoTimerSensor;
// class SoSeparator;
// class SoSwitch;
// class SoTranslation;
// class SoScale;

// *************************************************************************

class SOGTK_DLL_API SoGtkExaminerViewer : public SoGtkFullViewer {
  SOGTK_OBJECT_HEADER(SoGtkExaminerViewer, SoGtkFullViewer);

public:
  SoGtkExaminerViewer(
    GtkWidget * parent = (GtkWidget *) NULL,
    const char * name = (char *) NULL,
    SbBool embed = TRUE,
    SoGtkFullViewer::BuildFlag flags = SoGtkFullViewer::BUILD_ALL,
    SoGtkViewer::Type type = SoGtkViewer::BROWSER );
  ~SoGtkExaminerViewer(void);

  void setAnimationEnabled( const SbBool enable );
  SbBool isAnimationEnabled(void) const;

  void stopAnimating(void);
  SbBool isAnimating(void) const;

  void setFeedbackVisibility( const SbBool enable );
  SbBool isFeedbackVisible(void) const;

  void setFeedbackSize( const int size );
  int getFeedbackSize(void) const;

  // overloaded
  virtual void setViewing( SbBool enable );
  virtual void setCamera( SoCamera * newCamera );
  virtual void setCursorEnabled( SbBool enable );

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

  virtual GtkWidget * makeSubPreferences( GtkWidget * parent );
  virtual void createViewerButtons( GtkWidget * parent, SbPList * buttonlist );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void openViewerHelpCard(void);

private:
  void constructor( const SbBool build );

  void setCursorRepresentation( int mode );

private:
  // friends and family
  class SoAnyExaminerViewer * common;
  friend class SoAnyExaminerViewer;
  class SoGtkExaminerViewerP * pimpl;
  friend class SoGtkExaminerViewerP;

}; // class SoGtkExaminerViewer

// *************************************************************************

#endif // ! SOGTK_EXAMINERVIEWER_H
