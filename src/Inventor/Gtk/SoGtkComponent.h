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

#ifndef SOGTK_COMPONENT_H
#define SOGTK_COMPONENT_H

#include <Inventor/SbLinear.h>
#include <Inventor/SbString.h>
#include <Inventor/SoLists.h>

#include <gtk/gtk.h>

class SoGtkComponent;

typedef void SoGtkComponentCB( void * userData, SoGtkComponent * comp );
typedef void SoGtkComponentVisibilityCB( void * userData, SbBool visibleFlag );

// *************************************************************************

class SoGtkComponent
{
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

protected:
  SoGtkComponent( GtkWidget * const parent = NULL,
                  const char * const name = NULL,
                  const SbBool buildInsideParent = TRUE );

  void setClassName( const char * const name );
  void setBaseWidget( GtkWidget * widget );

  void subclassInitialized(void);

//  virtual const char * componentClassName(void) const = 0;
  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void sizeChanged( const SbVec2s size ) = 0;

  void addVisibilityChangeCallback( SoGtkComponentVisibilityCB * func,
                                    void * userData = NULL);
  void removeVisibilityChangeCallback( SoGtkComponentVisibilityCB * func,
                                       void * userData = NULL);

  void openHelpCard( const char * name );

  virtual SbBool eventFilter( GtkObject * object, GdkEvent * event );

private:
  GtkWidget * widget;
  GtkWidget * parent;
  SbBool buildInside;
  SbString className, widgetName, captionText, iconText;
  SoGtkComponentCB * closeCB;
  void * closeCBdata;
  SbPList * visibilityChangeCBs;
  SbVec2s storeSize;

  static SbPList * soGtkCompList;

}; // class SoGtkComponent

// *************************************************************************

#endif // ! SOGTK_COMPONENT_H
