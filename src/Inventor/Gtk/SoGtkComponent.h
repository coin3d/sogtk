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

#ifndef SOGTK_COMPONENT_H
#define SOGTK_COMPONENT_H

#include <gtk/gtk.h>

#include <Inventor/SbLinear.h>
#include <Inventor/SoLists.h>

#include <Inventor/Gtk/SoGtkObject.h>

class SoGtkComponent;

typedef void SoGtkComponentCB( void * userData, SoGtkComponent * comp );
typedef void SoGtkComponentVisibilityCB( void * userData, SbBool visibleFlag );

// *************************************************************************

class SOGTK_DLL_API SoGtkComponent : public SoGtkObject {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkComponent, SoGtkObject);

public:
  virtual ~SoGtkComponent(void);

  virtual void show(void);
  virtual void hide(void);
  SbBool isVisible(void);

  GtkWidget * getWidget(void) const;
  GtkWidget * baseWidget(void) const;
  SbBool isTopLevelShell(void) const;
  GtkWidget * getShellWidget(void) const;
  GtkWidget * getParentWidget(void) const;

  void setSize( const SbVec2s size );
  SbVec2s getSize(void) const;

  void setTitle( const char * const newTitle );
  const char * getTitle(void) const;
  void setIconTitle( const char * const newIconTitle );
  const char * getIconTitle(void) const;

  void setWindowCloseCallback( SoGtkComponentCB * func,
                               void * const data = NULL );
  static SoGtkComponent * getComponent( GtkWidget * const widget );
  const char * getWidgetName(void) const;
  const char * getClassName(void) const;

  static void initClasses(void);

protected:
  SoGtkComponent( GtkWidget * const parent = (GtkWidget *) NULL,
                  const char * const name = (char *) NULL,
                  const SbBool embed = TRUE );

  void setClassName( const char * const name );
  void setBaseWidget( GtkWidget * widget );

  virtual void afterRealizeHook(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void sizeChanged( const SbVec2s size );

  void addVisibilityChangeCallback( SoGtkComponentVisibilityCB * func,
                                    void * userData = NULL);
  void removeVisibilityChangeCallback( SoGtkComponentVisibilityCB * func,
                                       void * userData = NULL);

  void openHelpCard( const char * name );

  virtual SbBool eventFilter( GtkObject * object, GdkEvent * event );
  static gint eventHandler( GtkObject * object, GdkEvent * event, gpointer closure );

private:
  gint realizeHandler( GtkObject * object );
  static gint realizeHandlerCB( GtkObject * object, gpointer closure );

private:
  GtkWidget * widget;
  GtkWidget * parent;
  SbBool embedded;
  SbBool shelled;
  char * className;
  char * widgetName;
  char * captionText;
  char * iconText;
  SoGtkComponentCB * closeCB;
  void * closeCBdata;
  SbPList * visibilityChangeCBs;
  SbVec2s storeSize;

}; // class SoGtkComponent

// *************************************************************************

#endif // ! SOGTK_COMPONENT_H
