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
#endif

#include <string.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoMaterial.h>

#include <sogtkdefs.h>

#include <Inventor/Gtk/SoAnyMaterialList.h>
#include <Inventor/Gtk/SoGtkMaterialList.h>

/*!
  \class SoGtkMaterialList Inventor/Gtk/SoGtkMaterialList.h
  \brief The SoGtkMaterialList class is a component for selecting a material
  from a list of predefined materials.
*/

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkMaterialList);

// *************************************************************************

SoGtkMaterialList::SoGtkMaterialList(
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const char * const dir)
: inherited(parent, name, embed)
, common(new SoAnyMaterialList(dir))
{
  this->constructor(TRUE);
} // SoGtkMaterialList()

SoGtkMaterialList::SoGtkMaterialList(// protected
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const char * const dir,
  const SbBool build )
: inherited(parent, name, embed)
, common(new SoAnyMaterialList(dir))
{
  this->constructor(build);
} // SoGtkMaterialList()

void
SoGtkMaterialList::constructor(// private
  const SbBool build)
{
  this->materiallist = NULL;
  this->setClassName("SoGtkMaterialList");
  this->setSize(SbVec2s(200, 300));
  if (! build) return;
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * materiallist = this->buildWidget(parent);
  this->setBaseWidget(materiallist);
} // constructor()

SoGtkMaterialList::~SoGtkMaterialList(
  void)
{
  delete this->common;
} // ~SoGtkMaterialList()

// *************************************************************************

/*!
*/

GtkWidget *
SoGtkMaterialList::buildWidget(// protected
  GtkWidget * parent)
{
  GtkWidget * listroot = GTK_WIDGET(gtk_vbox_new(FALSE, 0));
  GtkWidget * menubar = GTK_WIDGET(gtk_menu_bar_new());
  gtk_widget_set_usize(menubar, 0, 30);
  gtk_widget_show(menubar);
  gtk_box_pack_start(GTK_BOX(listroot), GTK_WIDGET(menubar), 0, 0, 0);
  GtkWidget * materials = 
    gtk_menu_item_new_with_label(_("Materials"));
  gtk_widget_show(materials);
  gtk_menu_bar_append(GTK_MENU_BAR(menubar), materials);

  GtkWidget * menu = this->buildPulldownMenu(materials);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(materials), menu);

  /* value, lower, upper, step_increment, page_increment, page_size */
  GtkAdjustment * vertical = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, 1000, 10, 100, 100));
  GtkAdjustment * horizontal = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, 1000, 10, 100, 1000));

  GtkWidget * scrolled = gtk_scrolled_window_new(horizontal, vertical);
  gtk_scrolled_window_set_policy(
    GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_widget_show(scrolled);

  this->materiallist = GTK_WIDGET(gtk_clist_new(1));
  gtk_widget_show(this->materiallist);
  gtk_signal_connect(GTK_OBJECT(this->materiallist), "select_row",
    GTK_SIGNAL_FUNC(SoGtkMaterialList::itemactivationCB), (gpointer) this);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled), this->materiallist);

  SoGtkMaterialDirectory * dir = common->getMaterialDirectory();
  if (dir && dir->numGroups > 0) {
    SoGtkMaterialGroup * group = dir->groups[dir->current];
    for (int i = 0; i < group->numMaterials; i++) {
      const char ** ptr = &(group->materials[i]->name);
      gtk_clist_append(GTK_CLIST(this->materiallist), (char **) ptr);
    }
  }

  gtk_box_pack_start(GTK_BOX(listroot), scrolled, 1, 1, 0);

  return listroot;
} // buildWidget()

/*!
*/

GtkWidget *
SoGtkMaterialList::buildPulldownMenu(// protected
  GtkWidget * parent)
{
  GtkWidget * menu = GTK_WIDGET(gtk_menu_new());
  gtk_widget_show(menu);

  SoGtkMaterialDirectory * data = common->getMaterialDirectory();
  if (data) {
    for (int i = 0; i < data->numGroups; i++) {
      GtkWidget * menuitem = GTK_WIDGET(gtk_menu_item_new_with_label(data->groups[i]->name));
      gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
        GTK_SIGNAL_FUNC(SoGtkMaterialList::menuactivationCB), (gpointer) this);
      gtk_widget_show(menuitem);
      gtk_menu_append(GTK_MENU(menu), menuitem);
      data->groups[i]->menuitem = menuitem;
    }
  }

  return menu;
} // buildPulldownMenu()

// *************************************************************************

void
SoGtkMaterialList::addCallback(
  SoGtkMaterialListCB * const callback,
  void * const closure)
{
  common->addCallback(callback, closure);
} // addCallback()

void
SoGtkMaterialList::removeCallback(
  SoGtkMaterialListCB * const callback,
  void * const closure)
{
  common->removeCallback(callback, closure);
} // removeCallback()

// *************************************************************************

void
SoGtkMaterialList::menuactivation(// private
  GtkWidget * menuitem)
{
  SoGtkMaterialDirectory * dir = common->getMaterialDirectory();
  if (dir) {
    for (int i = 0; i < dir->numGroups; i++) {
      SoGtkMaterialGroup * group = dir->groups[i];
      if (group->menuitem == menuitem) {
        gtk_clist_clear(GTK_CLIST(this->materiallist));
        for (int j = 0; j < group->numMaterials; j++) {
          const char ** ptr = &(group->materials[j]->name);
          gtk_clist_append(GTK_CLIST(this->materiallist), (char **) ptr);
        }
        dir->current = i;
        return;
      }
    }
  }
  SoDebugError::postWarning("SoGtkMaterialList::menuactivation", 
    _("invalid menu item"));
} // menuactivation()

void
SoGtkMaterialList::menuactivationCB(// static, private
  GtkObject * obj,
  gpointer closure)
{
  assert(closure != NULL);
  SoGtkMaterialList * component = (SoGtkMaterialList *) closure;
  component->menuactivation(GTK_WIDGET(obj));
} // menuactivationCB()

// *************************************************************************

void
SoGtkMaterialList::itemactivation(// private
  int materialid)
{
  SoGtkMaterialDirectory * data = common->getMaterialDirectory();
  assert(data != NULL);
  assert(materialid >= 0 &&
          materialid < data->groups[data->current]->numMaterials);
  const char * materialdata =
    data->groups[data->current]->materials[materialid]->data;

  SoInput reader;
  if (data->flags & SOGTK_BUILTIN_MATERIALS) {
    reader.setBuffer((void *) materialdata, strlen(materialdata));
  } else {
    if (! reader.openFile(materialdata, FALSE)) {
      SoDebugError::postWarning("SoGtkMaterialList::itemactivation",
        "could not open file: \"%s\"", materialdata);
      return;
    }
  }

  SoNode * material = NULL;
  if (! SoDB::read(&reader, material)) {
    SoDebugError::postWarning("SoGtkMaterialList::itemactivation",
      "failed to read material");
    return;
  }
  if (! material) {
    SoDebugError::postWarning("SoGtkMaterialList::itemactivation",
      "read returned no data");
    return;
  }

  material->ref();
  if (! material->isOfType(SoMaterial::getClassTypeId())) {
    SoDebugError::postWarning("SoGtkMaterialList::itemactivation",
      "not a material node!");
    material->unref();
    return;
  }
  common->invokeCallbacks((SoMaterial *) material);
  material->unref();
} // itemactivation()

void
SoGtkMaterialList::itemactivationCB(// static, private
  GtkObject * obj,
  gint row,
  gint column,
  GdkEvent * event,
  gpointer closure)
{
  assert(closure != NULL);
  SoGtkMaterialList * component = (SoGtkMaterialList *) closure;
  component->itemactivation(row);
} // itemactivationCB()

// *************************************************************************

const char *
SoGtkMaterialList::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoGtkMaterialList";
  return defaultWidgetName;
} // getDefaultWidgetName()

const char *
SoGtkMaterialList::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = N_("Material List");
  return _(defaultTitle);
} // getDefaultTitle()

const char *
SoGtkMaterialList::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = N_("Material List");
  return _(defaultIconTitle);
} // getDefaultIconTitle()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkMaterialListRCSId(void) { return rcsid; }
#endif
