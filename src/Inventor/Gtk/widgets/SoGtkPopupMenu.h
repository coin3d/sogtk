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

#ifndef SOGTK_POPUPMENU_H
#define SOGTK_POPUPMENU_H

#include <Inventor/Gtk/SoGtkBasic.h>
#include <Inventor/Gtk/widgets/SoAnyPopupMenu.h>

struct MenuRecord;
struct ItemRecord;

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkPopupMenu : public SoAnyPopupMenu {
public:
  SoGtkPopupMenu(void);
  ~SoGtkPopupMenu(void);

  virtual int NewMenu( char * name, int menuid = -1 );
  virtual int GetMenu( char * name );
  virtual void SetMenuTitle( int id, char * title );
  virtual char * GetMenuTitle( int id );

  virtual int NewMenuItem( char * name, int itemid = -1 );
  virtual int GetMenuItem( char * name );
  virtual void SetMenuItemTitle( int itemid, char * title );
  virtual char * GetMenuItemTitle( int itemid );
  virtual void SetMenuItemEnabled( int itemid, SbBool enabled );
  virtual SbBool GetMenuItemEnabled( int itemid );

  virtual void _setMenuItemMarked( int itemid, SbBool marked );
  virtual SbBool GetMenuItemMarked( int itemid );

  virtual void AddMenu( int menuid, int submenuid, int pos = -1 );
  virtual void AddMenuItem( int menuid, int itemid, int pos = -1 );
  virtual void AddSeparator( int menuid, int pos = -1 );
  virtual void RemoveMenu( int menuid );
  virtual void RemoveMenuItem( int itemid );

  virtual void PopUp( GtkWidget * inside, int x, int y );

protected:
  MenuRecord * getMenuRecord( int menuid );
  ItemRecord * getItemRecord( int itemid );
  MenuRecord * createMenuRecord( char * name );
  ItemRecord * createItemRecord( char * name );

  void traverseBuild( GtkWidget * parent, MenuRecord * menu, int indent );
  GtkWidget * build( GtkWidget * parent );

  void selection( int itemid );
  static void selectionCB( GtkWidget *, gpointer );

private:
  void createSeparator( ItemRecord * item );
  void createMenuItem( ItemRecord * item );

  SbPList * menus;
  SbPList * items;
  GtkWidget * popup;
  SbBool dirty;

}; // class SoGtkPopupMenu

// *************************************************************************

#endif // ! SOGTK_POPUPMENU_H
