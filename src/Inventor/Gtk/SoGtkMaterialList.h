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

#ifndef SOGTK_MATERIALLIST_H
#define SOGTK_MATERIALLIST_H

#include <Inventor/Gtk/SoGtkComponent.h>

class SoMaterial;
class SoAnyMaterialList;

typedef void SoGtkMaterialListCB( void * closure, const SoMaterial * material );

// *************************************************************************

class SOGTK_DLL_API SoGtkMaterialList : public SoGtkComponent {
  SOGTK_OBJECT_HEADER(SoGtkMaterialList, SoGtkComponent);

public:
  SoGtkMaterialList( GtkWidget * parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE,
    const char * const dir = NULL );
  ~SoGtkMaterialList(void);

  void addCallback(
    SoGtkMaterialListCB * const callback,
    void * const closure = NULL );
  void removeCallback(
    SoGtkMaterialListCB * const callback,
    void * const closure = NULL );

protected:
  SoGtkMaterialList( GtkWidget * parent, const char * const name,
    const SbBool embed, const char * const dir, const SbBool build  );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  GtkWidget * buildWidget( GtkWidget * parent );
  GtkWidget * buildPulldownMenu( GtkWidget * parent );

private:
  void constructor( const SbBool build );

  void menuactivation( GtkWidget * menuitem );
  static void menuactivationCB( GtkObject * obj, gpointer closure );

  void itemactivation( int item );
  static void itemactivationCB( GtkObject * obj, gint row, gint column,
    GdkEvent * event, gpointer closure );

  GtkWidget * materiallist;
  SoAnyMaterialList * common;

}; // class SoGtkMaterialList

// *************************************************************************

#endif // ! SOGTK_MATERIALLIST_H
