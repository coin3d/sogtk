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

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <string.h>

#include <gtk/gtkmenu.h>
#include <gtk/gtkcheckmenuitem.h>
#include <gtk/gtkhseparator.h>

#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkBasic.h>
#include <Inventor/Gtk/widgets/SoAnyPopupMenu.h>
#include <Inventor/Gtk/widgets/SoGtkPopupMenu.h>

// *************************************************************************

struct MenuRecord {
  int menuid;
  int pos;
  char * name;
  char * title;
  GtkWidget * item;
  GtkWidget * menu;
  MenuRecord * parent;
}; // struct MenuRecord

struct ItemRecord {
  SoGtkPopupMenu * context;
  int itemid;
  int flags;
  int pos;
  char * name;
  char * title;
  GtkWidget * item;
  MenuRecord * parent;
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
  this->dirty = TRUE;
  this->popup = (GtkWidget *) NULL;
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
    delete [] rec->name;
    delete [] rec->title;
    delete rec;
  }
  delete this->items;
  const int nummenus = this->menus->getLength();
  for ( i = 0; i < nummenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    delete [] rec->name;
    delete [] rec->title;
    delete rec;
  }
  delete this->menus;
} // ~SoGtkPopupMenu()

// *************************************************************************

/*!
*/

int
SoGtkPopupMenu::newMenu(
  const char * name,
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
} // newMenu()

/*!
*/

int
SoGtkPopupMenu::getMenu(
  const char * name )
{
  const int numMenus = this->menus->getLength();
  int i;
  for ( i = 0; i < numMenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    if ( strcmp( rec->name, name ) == 0 )
      return rec->menuid;
  }
  return -1;
} // getMenu()

/*!
*/

void
SoGtkPopupMenu::setMenuTitle(
  int menuid,
  const char * title )
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
} // setMenuTitle()

/*!
*/

const char *
SoGtkPopupMenu::getMenuTitle(
  int menuid )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
  if ( rec )
    return rec->title;
  return (const char *) NULL;
} // getMenuTitle()

// *************************************************************************

/*!
*/

int
SoGtkPopupMenu::newMenuItem(
  const char * name,
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
} // newMenuItem()

/*!
*/

int
SoGtkPopupMenu::getMenuItem(
  const char * name )
{
  const int numItems = this->items->getLength();
  int i;
  for ( i = 0; i < numItems; i++ ) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    if ( strcmp( rec->name, name ) == 0 )
      return rec->itemid;
  }
  return -1;
} // getMenuItem()

/*!
*/

void
SoGtkPopupMenu::setMenuItemTitle(
  int itemid,
  const char * title )
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
} // setMenuItemTitle()

/*!
*/

const char *
SoGtkPopupMenu::getMenuItemTitle(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec )
    return rec->title;
  return (const char *) NULL;
} // getMenuItemTitle()

/*!
*/

void
SoGtkPopupMenu::setMenuItemEnabled(
  int itemid,
  SbBool enabled )
{
  SOGTK_STUB();
} // setMenuItemEnabled()

/*!
*/

SbBool
SoGtkPopupMenu::getMenuItemEnabled(
  int itemid )
{
  SOGTK_STUB();
  return FALSE;
} // getMenuItemEnabled()

/*!
*/

void
SoGtkPopupMenu::_setMenuItemMarked(
  int itemid,
  SbBool marked )
{
#if SOGTK_DEBUG && 0
  SoDebugError::postInfo( "SoGtkPopupMenu::SetMenuItemMarked", "item %d to %s",
    itemid, marked ? "true" : "false" );
#endif // SOGTK_DEBUG && 0
  ItemRecord * item = getItemRecord( itemid );
  if ( ! item ) {
    SoDebugError::postInfo( "SoGtkPopupMenu::SetMenuItemMarked",
      "no such item (%d).", itemid );
    return;
  }
  if ( item->item ) {
    gtk_signal_handler_block_by_data( GTK_OBJECT(item->item), (gpointer) item );
    if ( marked )
      gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(item->item), TRUE );
    else
      gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(item->item), FALSE );
    gtk_signal_handler_unblock_by_data( GTK_OBJECT(item->item), (gpointer) item );
  }
  if ( marked )
    item->flags |= ITEM_MARKED;
  else
    item->flags &= ~ITEM_MARKED;
} // _setMenuItemMarked()

/*!
*/

SbBool
SoGtkPopupMenu::getMenuItemMarked(
  int itemid )
{
  ItemRecord * item = getItemRecord( itemid );
  if ( ! item ) {
    SoDebugError::postInfo( "SoGtkPopupMenu::SetMenuItemMarked",
      "no such item (%d).", itemid );
    return FALSE;
  }
  if ( item->flags & ITEM_MARKED )
    return TRUE;
  return FALSE;
} // getMenuItemMarked()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::addMenu(
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
  if ( pos == -1 ) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == super && rec->pos >= max )
        max = rec->pos + 1;
    }
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == super && rec->pos >= max )
        max = rec->pos + 1;
    }
    sub->pos = max;
    sub->parent = super;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == super && rec->pos >= pos )
        rec->pos = rec->pos + 1;
    }
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == super && rec->pos >= pos )
        rec->pos = rec->pos + 1;
    }
    sub->pos = pos;
    sub->parent = super;
  }
} // addMenu()

/*!
*/

void
SoGtkPopupMenu::addMenuItem(
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
  if ( pos == -1 ) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu && rec->pos >= max )
        max = rec->pos + 1;
    }
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu && rec->pos >= max )
        max = rec->pos + 1;
    }
    item->pos = max;
    item->parent = menu;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu && rec->pos >= pos )
        rec->pos = rec->pos + 1;
    }
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu && rec->pos >= pos )
        rec->pos = rec->pos + 1;
    }
    item->pos = pos;
    item->parent = menu;
  }
} // addMenuItem()

/*!
*/

void
SoGtkPopupMenu::addSeparator(
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
  ItemRecord * sep = createItemRecord( "separator" );
  sep->flags |= ITEM_SEPARATOR;
  if ( pos == -1 ) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu && rec->pos >= max )
        max = rec->pos + 1;
    }
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu && rec->pos >= max )
        max = rec->pos + 1;
    }
    sep->pos = max;
    sep->parent = menu;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu && rec->pos >= pos )
        rec->pos = rec->pos + 1;
    }
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu && rec->pos >= pos )
        rec->pos = rec->pos + 1;
    }
    sep->pos = pos;
    sep->parent = menu;
  }
  this->items->append( sep );
} // addSeparator()

/*!
*/

void
SoGtkPopupMenu::removeMenu(
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
} // removeMenu()

/*!
*/

void
SoGtkPopupMenu::removeMenuItem(
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
} // removeMenuItem()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::popUp(
  GtkWidget * inside,
  int x,
  int y )
{
  MenuRecord * root = this->getMenuRecord( 0 );
  if ( ! root ) {
#if SOGTK_DEBUG
    SoDebugError::postInfo( "SoGtkPopupMenu::PopUp", "no root menu" );
#endif // SOGTK_DEBUG
    return;
  }

  if ( this->dirty ) {
    if ( this->popup != (GtkWidget *) NULL ) {
      // destroy existing menu
    }
    this->popup = this->build( inside );
  }
  this->dirty = FALSE;
  gtk_menu_popup( GTK_MENU(this->popup), (GtkWidget *) NULL, (GtkWidget *) NULL,
    (GtkMenuPositionFunc) NULL, NULL, 2, 0 );
} // popUp()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::createSeparator( // private
  ItemRecord * item )
{
  item->item = GTK_WIDGET(gtk_menu_item_new());
  GtkWidget * separator = GTK_WIDGET(gtk_hseparator_new());
  gtk_container_add( GTK_CONTAINER(item->item), GTK_WIDGET(separator) );
  gtk_widget_show(separator);
} // createSeparator()

/*!
*/

void
SoGtkPopupMenu::createMenuItem( // private
  ItemRecord * item )
{
  item->item = GTK_WIDGET(gtk_check_menu_item_new_with_label(item->title));
  if ( item->flags & ITEM_MARKED )
    gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(item->item), TRUE );
  gtk_signal_connect( GTK_OBJECT(item->item), "activate",
    GTK_SIGNAL_FUNC(SoGtkPopupMenu::selectionCB), (gpointer) item );
} // createMenuItem()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::traverseBuild(
  GtkWidget * parent,
  MenuRecord * menu,
  int indent )
{
  int i, j;
//  char pre[24];
//  for ( i = 0; i < indent; i++ ) pre[i] = ' ';
//  pre[i] = '\0';
  j = 0;
  MenuRecord * sub;
  ItemRecord * item;
  do {
    sub = (MenuRecord *) NULL;
    item = (ItemRecord *) NULL;
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      sub = (MenuRecord *) (*this->menus)[i];
      if ( sub->pos == j && sub->parent == menu ) {
//        fprintf( stderr, "%s%s {\n", pre, sub->name );
        assert( menu->title != NULL );
        menu->item = GTK_WIDGET(gtk_menu_item_new_with_label( sub->title ));
        gtk_container_add( GTK_CONTAINER(parent), GTK_WIDGET(menu->item) );
        gtk_widget_show( GTK_WIDGET(menu->item) );
        GtkWidget * submenu = GTK_WIDGET(gtk_menu_new());
        this->traverseBuild( submenu, sub, indent + 2 );
        gtk_menu_item_set_submenu( GTK_MENU_ITEM(menu->item), GTK_WIDGET(submenu) );
//        fprintf( stderr, "%s}\n", pre );
        break;
      } else {
        sub = (MenuRecord *) NULL;
      }
    }
    if ( sub == NULL ) {
      const int numItems = this->items->getLength();
      for ( i = 0; i < numItems; i++ ) {
        item = (ItemRecord *) (*this->items)[i];
        if ( item->pos == j && item->parent == menu ) {
//          fprintf( stderr, "%s%s [%s]\n", pre, item->title, item->name );
          if ( item->flags & ITEM_SEPARATOR ) {
            createSeparator( item );
          } else {
            createMenuItem( item );
          }
          gtk_container_add( GTK_CONTAINER(parent), GTK_WIDGET(item->item) );
          gtk_widget_show( GTK_WIDGET(item->item) );
          break;
        } else {
          item = (ItemRecord *) NULL;
        }
      }
    }
    j++;
  } while ( sub != NULL || item != NULL );
} // traverseBuild()

/*!
*/

GtkWidget *
SoGtkPopupMenu::build(
  GtkWidget * )
{
  MenuRecord * root = this->getMenuRecord( 0 );
  assert( root != NULL );
  GtkWidget * popup = GTK_WIDGET(gtk_menu_new());
//  fprintf( stderr, "%s {\n", root->name );
  this->traverseBuild( popup, root, 2 );
//  fprintf( stderr, "}\n" );
  return popup;
} // build()

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
  const char * name )
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->pos = -1;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
  rec->menu = (GtkWidget *) NULL;
  rec->item = (GtkWidget *) NULL;
  rec->parent = (MenuRecord *) NULL;
  return rec;
} // createMenuRecord()

/*!
*/

ItemRecord *
SoGtkPopupMenu::createItemRecord(
  const char * name )
{
  ItemRecord * rec = new ItemRecord;
  rec->context = this;
  rec->itemid = -1;
  rec->flags = 0;
  rec->pos = -1;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
  rec->item = (GtkWidget *) NULL;
  rec->parent = (MenuRecord *) NULL;
  return rec;
} // createItemRecord()

// *************************************************************************

void
SoGtkPopupMenu::selection(
  int itemid )
{
  this->invokeMenuSelection( itemid );
} // selection()

void
SoGtkPopupMenu::selectionCB( // static
  GtkWidget *,
  gpointer closure )
{
  ItemRecord * item = (ItemRecord *) closure;
  item->context->selection( item->itemid );
} // selectionCB()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkPopupMenuRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

