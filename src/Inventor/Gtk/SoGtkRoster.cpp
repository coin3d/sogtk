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
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtkvbox.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoType.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/SbPList.h>

#include <sogtkdefs.h>

#include <Inventor/Gtk/SoGtkComponent.h>
#include <Inventor/Gtk/SoGtkRoster.h>

// *************************************************************************

/*!
  \class SoGtkRoster Inventor/Gtk/SoGtkRoster.h
  \brief The SoGtkRoster class is a component class for tracking the
  creation and destruction of SoGtk components.
  \ingroup misc

  This component class is not part of the original SoXt API - it is a SIM extension.
*/

/*!
  \enum SoGtkRoster::BuildFlags
  FIXME: write doc
*/

/*!
  \var SoGtkRoster::BuildFlags SoGtkRoster::MENUBAR
  FIXME: write doc
*/

/*!
  \var SoGtkRoster::BuildFlags SoGtkRoster::ROSTERLIST
  FIXME: write doc
*/

/*!
  \var SoGtkRoster::BuildFlags SoGtkRoster::STATUSBAR
  FIXME: write doc
*/

/*!
  \var SoGtkRoster::BuildFlags SoGtkRoster::BUILD_ALL
  FIXME: write doc
*/


// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkRoster);

// *************************************************************************

/*!
  Public constructor.
*/

SoGtkRoster::SoGtkRoster(
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const int flags)
: inherited(parent, name, embed)
{
  this->buildflags = flags;
  this->constructor(TRUE);
} // SoGtkRoster()

/*!
  Protected constructor for derived classes.
*/

SoGtkRoster::SoGtkRoster(// protected
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const int flags,
  const SbBool build)
: inherited(parent, name, embed)
{
  this->buildflags = flags;
  this->constructor(build);
} // SoGtkRoster()

/*!
  Common constructor code.
*/

void
SoGtkRoster::constructor(// private
  const SbBool build)
{
  this->rosterbase = (GtkWidget *) NULL;
  this->menubar = (GtkWidget *) NULL;
  this->rosterlist = (GtkWidget *) NULL;
  this->listwidget = (GtkWidget *) NULL;
  this->statusbar = (GtkWidget *) NULL;
  this->horizontal = (GtkAdjustment *) NULL;
  this->vertical = (GtkAdjustment *) NULL;

  SoGtk::addComponentActionCallback(
    SoGtkRoster::componentActionCB, (void *) this);

  this->setSize(SbVec2s(250, 250));

  if (! build) return;
  GtkWidget * parent = this->getParentWidget();
  this->rosterbase = this->buildWidget(parent);
  this->setBaseWidget(this->rosterbase);
  gtk_widget_show(this->rosterbase);
} // constructor()

/*!
  Destructor.
*/

SoGtkRoster::~SoGtkRoster(
  void)
{
  SoGtk::removeComponentActionCallback(
    SoGtkRoster::componentActionCB, (void *) this);
} // ~SoGtkRoster()

// *************************************************************************

/*!
  Function for generating roster list.
*/

GtkWidget *
SoGtkRoster::buildWidget(// virtual, protected
  GtkWidget * parent)
{
  GtkWidget * compound = gtk_vbox_new(FALSE, 0);
  if (this->buildflags & MENUBAR) {
    this->menubar = this->buildMenuBarWidget(compound);
    gtk_widget_set_usize(GTK_WIDGET(this->menubar), 0, 30);
    gtk_widget_show(this->menubar);
    gtk_box_pack_start(GTK_BOX(compound), GTK_WIDGET(this->menubar), FALSE, TRUE, 0);
  }
  if (this->buildflags & ROSTERLIST) {
    this->rosterlist = this->buildRosterListWidget(compound);
    gtk_widget_show(this->rosterlist);
    gtk_box_pack_start(GTK_BOX(compound), GTK_WIDGET(this->rosterlist), TRUE, TRUE, 0);
  }
  if (this->buildflags & STATUSBAR) {
    this->statusbar = this->buildStatusBarWidget(compound);
    gtk_widget_set_usize(GTK_WIDGET(this->statusbar), 0, 30);
    gtk_widget_show(this->statusbar);
    gtk_box_pack_end(GTK_BOX(compound), GTK_WIDGET(this->statusbar), FALSE, TRUE, 0);
  }
  this->buildRosterList();
  return compound;
} // buildWidget()

/*!
  FIXME: write doc
*/

static void
create_component(
  GtkObject * obj,
  gpointer closure)
{
  SoType type = SoType::fromName(SbName((const char *) closure));
  assert(type.canCreateInstance());
  SoDebugError::postInfo("create_component", "creating %s", type.getName().getString());
  SoGtkComponent * component = (SoGtkComponent *) type.createInstance();
  component->show();
} // create_component()

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkRoster::buildMenuBarWidget(// virtual, protected
  GtkWidget * parent)
{
  GtkWidget * menubar = GTK_WIDGET(gtk_menu_bar_new());

  GtkWidget * createitem = GTK_WIDGET(gtk_menu_item_new_with_label(_("Create")));
  gtk_widget_show(createitem);
  gtk_menu_bar_append(GTK_MENU_BAR(menubar), GTK_WIDGET(createitem));

  GtkWidget * createmenu = GTK_WIDGET(gtk_menu_new());
  SoTypeList types;
  const int num = SoType::getAllDerivedFrom(SoGtkComponent::getClassTypeId(), types);
  for (int i = 0; i < num; i++) {
    SoType type = types[i];
    SbName name = type.getName();
    if (type.canCreateInstance()) {
      GtkWidget * item = GTK_WIDGET(gtk_menu_item_new_with_label(name.getString()));
      gtk_signal_connect(GTK_OBJECT(item), "activate",
        GTK_SIGNAL_FUNC(create_component), (gpointer) type.getName().getString());
      gtk_widget_show(item);
      gtk_menu_append(GTK_MENU(createmenu), item);
    }
  }

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(createitem), createmenu);

  return menubar;
} // buildMenuBarWidget()

/*!
  FIXME: write doc
*/

void
SoGtkRoster::buildRosterList(
  void)
{
  SbPList components;
  const int numComponents = SoGtk::getComponents(components);
  for (int i = 0; i < numComponents; i++) {
    SoGtkComponent * component = (SoGtkComponent *) components[i];
    char * title = (char *) component->getTitle();
    char * classname = (char *) component->getTypeId().getName().getString();
    char * columns[] = { title, classname, (char *) NULL };
    gtk_clist_append(GTK_CLIST(this->listwidget), columns);
  }
} // buildRosterList()

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkRoster::buildRosterListWidget(// virtual, protected
  GtkWidget * parent)
{
  this->vertical = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, 1000, 10, 100, 100));
  this->horizontal = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, 1000, 10, 100, 1000));

  GtkWidget * rosterlist = GTK_WIDGET(gtk_scrolled_window_new(horizontal, vertical));
  gtk_scrolled_window_set_policy(
    GTK_SCROLLED_WINDOW(rosterlist), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  char * titles[] = { _("Title"), _("Class"), (char *) NULL };
  this->listwidget = GTK_WIDGET(gtk_clist_new_with_titles(2, titles));
  gtk_clist_column_titles_show(GTK_CLIST(this->listwidget));
  gtk_clist_set_column_visibility(GTK_CLIST(this->listwidget), 1, TRUE);
  gtk_clist_set_column_visibility(GTK_CLIST(this->listwidget), 2, TRUE);
  gtk_widget_show(this->listwidget);

  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(rosterlist), this->listwidget);
  return rosterlist;
} // buildRosterListWidget()

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkRoster::buildStatusBarWidget(// virtual, protected
  GtkWidget * parent)
{
  GtkWidget * statusbar = GTK_WIDGET(gtk_hbox_new(FALSE, 0));
  return statusbar;
} // buildStatusBarWidget()

// *************************************************************************

// doc in super
void
SoGtkRoster::sizeChanged(const SbVec2s & size)
{
  // SoDebugError::postInfo("SoGtkRoster::sizeChanged", "[invoked]");
} // sizeChanged()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkRoster::componentCreated(// virtual, protected
  SoGtkComponent * component)
{
#if SOGTK_DEBUG && 0
  SoDebugError::postInfo("SoGtkRoster::componentCreated",
    "component = %p", component);
#endif
  char * title = (char *) component->getTitle();
  char * classname = (char *) component->getTypeId().getName().getString();
  char * strings[] = { title, classname, (char *) NULL };
  gtk_clist_append(GTK_CLIST(this->listwidget), strings);
} // componentCreated()

/*!
  FIXME: write doc
*/

void
SoGtkRoster::componentDestroyed(// virtual, protected
  SoGtkComponent * component)
{
#if SOGTK_DEBUG && 0
  SoDebugError::postInfo("SoGtkRoster::componentDestroyed",
    "component = %p", component);
#endif
} // componentDestroyed()

/*!
  FIXME: write doc
*/

void
SoGtkRoster::componentChanged(// virtual, protected
  SoGtkComponent * component)
{
#if SOGTK_DEBUG && 0
  SoDebugError::postInfo("SoGtkRoster::componentChanged",
    "component = %p", component);
#endif
} // componentChanged()

/*!
  FIXME: write doc
*/

void
SoGtkRoster::componentActionCB(// static, private
  SoGtkComponent * component,
  SoGtk::SoGtkComponentAction action,
  void * closure)
{
  SoGtkRoster * const roster = (SoGtkRoster *) closure;
  switch (action) {
  case SoGtk::CREATION:
    roster->componentCreated(component);
    break;
  case SoGtk::DESTRUCTION:
    roster->componentChanged(component);
    break;
  case SoGtk::CHANGE:
    roster->componentDestroyed(component);
    break;
  default:
#if SOGTK_DEBUG
    SoDebugError::postInfo("SoGtkRoster::componentActionCB", "unknown action");
#endif
    break;
  } // switch (action)
} // componentActionCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkRoster::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoGtkRoster";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoGtkRoster::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = N_("Roster");
  return _(defaultTitle);
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoGtkRoster::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = N_("Roster");
  return _(defaultIconTitle);
} // getDefaultIconTitle()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkRosterRCSId(void) { return rcsid; }
#endif

