/**************************************************************************
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
 **************************************************************************/

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkBasic.h>
#include <Inventor/Gtk/widgets/SoAnyPopupMenu.h>
#include <Inventor/Gtk/widgets/SoGtkPopupMenu.h>

// *************************************************************************

struct MenuRecord {
  int menuid;
  char * name;
  char * title;
//  QPopupMenu * menu;
//  QPopupMenu * parent;
}; // struct MenuRecord

struct ItemRecord {
  int itemid;
  int flags;
  char * name;
  char * title;
//  QPopupMenu * parent;
}; // struct ItemRecord

#define ITEM_MARKED       0x0001
#define ITEM_SEPARATOR    0x0002
#define ITEM_ENABLED      0x0004

// *************************************************************************

/*!
  Constructor.
*/

SoGtkPopupMenu::SoGtkPopupMenu(
  void )
{
  this->items = new SbPList;
  this->menus = new SbPList;
} // SoGtkPopupMenu()

/*!
  Destructor.
*/

SoGtkPopupMenu::~SoGtkPopupMenu(
  void )
{
  int i;
  const int numitems = this->items->getLength();
  for ( i = 0; i < numitems; i++ ) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    delete rec;
  }
  delete this->items;
  const int nummenus = this->menus->getLength();
  for ( i = 0; i < nummenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    delete rec;
  }
  delete this->menus;
} // ~SoGtkPopupMenu()

// *************************************************************************

/*!
*/

int
SoGtkPopupMenu::NewMenu(
  char * name,
  int menuid )
{
  int id = menuid;
  if ( id == -1 ) {
    id = 1;
    while ( this->getMenuRecord( id ) != NULL ) id++;
  } else {
    if ( this->getMenuRecord( id ) != NULL ) {
#if SOGTK_DEBUG
      SoDebugError::postInfo( "SoGtkPopupMenu::NewMenu",
        "requested menuid already taken" );
#endif // SOGTK_DEBUG
      return -1;
    }
  }
  // id contains ok ID
  MenuRecord * rec = createMenuRecord( name );
  rec->menuid = id;
  this->menus->append( (void *) rec );
  return id;
} // NewMenu()

/*!
*/

int
SoGtkPopupMenu::GetMenu(
  char * name )
{
  const int numMenus = this->menus->getLength();
  int i;
  for ( i = 0; i < numMenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    if ( strcmp( rec->name, name ) == 0 )
      return rec->menuid;
  }
  return -1;
} // GetMenu()

/*!
*/

void
SoGtkPopupMenu::SetMenuTitle(
  int menuid,
  char * title )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
  if ( rec == NULL ) {
    SoDebugError::postWarning( "SoGtkPopupMenu::SetMenuTitle",
      "no such menu (%d.title = \"%s\")", menuid, title );
    return;
  }
  delete [] rec->title;
  rec->title = strcpy( new char [strlen(title)+1], title );
/*
  if ( rec->parent )
    rec->parent->changeItem( rec->menuid, rec->title );
*/
} // SetMenuTitle()

/*!
*/

char *
SoGtkPopupMenu::GetMenuTitle(
  int menuid )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
  if ( rec )
    return rec->title;
  return NULL;
} // GetMenuTitle()

// *************************************************************************

/*!
*/

int
SoGtkPopupMenu::NewMenuItem(
  char * name,
  int itemid )
{
  int id = itemid;
  if ( id == -1 ) {
    id = 1;
    while ( this->getItemRecord( itemid ) != NULL )
      id++;
  } else {
    if ( this->getItemRecord( itemid ) != NULL ) {
#if SOGTK_DEBUG
      SoDebugError::postInfo( "SoGtkPopupMenu::NewMenuItem",
        "requested itemid already taken" );
#endif // SOGTK_DEBUG
      return -1;
    }
  }
  ItemRecord * rec = createItemRecord( name );
  rec->itemid = id;
  this->items->append( rec );
  return id;
} // NewMenuItem()

/*!
*/

int
SoGtkPopupMenu::GetMenuItem(
  char * name )
{
  const int numItems = this->items->getLength();
  int i;
  for ( i = 0; i < numItems; i++ ) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    if ( strcmp( rec->name, name ) == 0 )
      return rec->itemid;
  }
  return -1;
} // GetMenuItem()

/*!
*/

void
SoGtkPopupMenu::SetMenuItemTitle(
  int itemid,
  char * title )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL )
    return;
  delete [] rec->title;
  rec->title = strcpy( new char [strlen(title)+1], title );
/*
  if ( rec->parent )
    rec->parent->changeItem( rec->itemid, rec->title );
*/
} // SetMenuItemTitle()

/*!
*/

char *
SoGtkPopupMenu::GetMenuItemTitle(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec )
    return rec->title;
  return NULL;
} // GetMenuItemTitle()

/*!
*/

void
SoGtkPopupMenu::SetMenuItemEnabled(
  int itemid,
  SbBool enabled )
{
  SOGTK_STUB();
} // SetMenuItemEnabled()

/*!
*/

SbBool
SoGtkPopupMenu::GetMenuItemEnabled(
  int itemid )
{
  SOGTK_STUB();
  return FALSE;
} // GetMenuItemEnabled()

/*!
*/

void
SoGtkPopupMenu::SetMenuItemMarked(
  int itemid,
  SbBool marked )
{
  SOGTK_STUB();
} // SetMenuItemMarked()

/*!
*/

SbBool
SoGtkPopupMenu::GetMenuItemMarked(
  int itemid )
{
  SOGTK_STUB();
  return FALSE;
} // GetMenuItemMarked()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::AddMenu(
  int menuid,
  int submenuid,
  int pos )
{
  MenuRecord * super = this->getMenuRecord( menuid );
  MenuRecord * sub = this->getMenuRecord( submenuid );
  if ( super == NULL || sub == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::AddMenu",
      "no such menu (super = 0x%08x, sub = 0x%08x)", super, sub );
#endif // SOGTK_DEBUG
    return;
  }
/*
  if ( pos == -1 )
    super->menu->insertItem( QString( sub->title ), sub->menu, sub->menuid );
  else
    super->menu->insertItem( QString( sub->title ),
                             sub->menu, sub->menuid, pos );
  sub->parent = super->menu;
*/
} // AddMenu()

/*!
*/

void
SoGtkPopupMenu::AddMenuItem(
  int menuid,
  int itemid,
  int pos )
{
  MenuRecord * menu = this->getMenuRecord( menuid );
  ItemRecord * item = this->getItemRecord( itemid );
  if ( menu == NULL || item == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::AddMenuItem",
      "no such item (menu = 0x%08x, item = 0x%08x)", menu, item );
#endif // SOGTK_DEBUG
    return;
  }
/*
  if ( pos == -1 )
    menu->menu->insertItem( QString( item->title ), item->itemid );
  else
    menu->menu->insertItem( QString( item->title ), item->itemid, pos );
  item->parent = menu->menu;
  if ( item->flags & ITEM_MARKED )
    item->parent->setItemChecked( item->itemid, true );
*/
} // AddMenuItem()

/*!
*/

void
SoGtkPopupMenu::AddSeparator(
  int menuid,
  int pos )
{
  MenuRecord * menu = this->getMenuRecord( menuid );
  if ( menu == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postWarning( "SoGtkPopupMenu::AddSeparator",
      "no such menu (%d)", menuid );
#endif // SOGTK_DEBUG
    return;
  }
  ItemRecord * rec = createItemRecord( "separator" );
/*
  menu->menu->insertSeparator( pos );
*/
  rec->flags |= ITEM_SEPARATOR;
  this->items->append( rec );
} // AddSeparator()

/*!
*/

void
SoGtkPopupMenu::RemoveMenu(
  int menuid )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
/*
  if ( rec == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::RemoveMenu", "no such menu" );
#endif // SOGTK_DEBUG
    return;
  }
  if ( rec->menuid == 0 ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::RemoveMenu", "can't remove root" );
#endif // SOGTK_DEBUG
    return;
  }
  if ( rec->parent == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::RemoveMenu", "menu not attached" );
#endif // SOGTK_DEBUG
    return;
  }
  rec->parent->removeItem( rec->menuid );
  rec->parent = NULL;
*/
} // RemoveMenu()

/*!
*/

void
SoGtkPopupMenu::RemoveMenuItem(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::RemoveMenu", "no such item" );
#endif // SOGTK_DEBUG
    return;
  }
/*
  if ( rec->parent == NULL ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::RemoveMenu", "item not attached" );
#endif // SOGTK_DEBUG
    return;
  }
  rec->parent->removeItem( rec->itemid );
  rec->parent = NULL;
*/
} // RemoveMenuItem()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::PopUp(
  GtkWidget * inside,
  int x,
  int y )
{
  MenuRecord * rec = this->getMenuRecord( 0 );
/*
  rec->menu->popup( x, y );
*/
} // PopUp()

// *************************************************************************

/*!
*/

MenuRecord *
SoGtkPopupMenu::getMenuRecord(
  int menuid )
{
  const int nummenus = this->menus->getLength();
  for ( int i = 0; i < nummenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    if ( rec->menuid == menuid )
      return rec;
  }
  return (MenuRecord *) NULL;
} // getMenuRecord()

/*!
*/

ItemRecord *
SoGtkPopupMenu::getItemRecord(
  int itemid )
{
  const int numitems = this->items->getLength();
  for ( int i = 0; i < numitems; i++ ) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    if ( rec->itemid == itemid )
      return rec;
  }
  return (ItemRecord *) NULL;
} // getItemRecord()

/*!
*/

MenuRecord *
SoGtkPopupMenu::createMenuRecord(
  char * name )
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
//  rec->parent = NULL;
  return rec;
} // createMenuRecord()

/*!
*/

ItemRecord *
SoGtkPopupMenu::createItemRecord(
  char * name )
{
  ItemRecord * rec = new ItemRecord;
  rec->itemid = -1;
  rec->flags = 0;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
//  rec->parent = NULL;
  return rec;
} // createItemRecord()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkPopupMenuRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

