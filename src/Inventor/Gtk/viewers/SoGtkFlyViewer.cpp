/**************************************************************************
 *
 *  This file is part of the Coin SoGtk GUI binding library.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
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
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/events/SoEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/viewers/SoAnyFlyViewer.h>
#include <Inventor/Gtk/viewers/SoGtkFlyViewer.h>

// ************************************************************************

/*!
  \class SoGtkFlyViewer Inventor/Gtk/viewers/SoGtkFlyViewer.h
  \brief The SoGtkFlyViewer class is yet to be documented.
  \ingroup components viewers
*/

// ************************************************************************
// ************************************************************************

class SoGtkFlyViewerP {
public:
  SoGtkFlyViewerP(SoGtkFlyViewer * publ);
  ~SoGtkFlyViewerP(void);

private:
  SoGtkFlyViewer * pub;

}; // class SoGtkFlyViewerP

// ************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkFlyViewer);

// ************************************************************************

/*!
  Public constructor.
*/

SoGtkFlyViewer::SoGtkFlyViewer(// public
  GtkWidget * parent,
  const char * name, 
  SbBool embed, 
  SoGtkFullViewer::BuildFlag flag,
  SoGtkViewer::Type type)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(TRUE);
} // SoGtkFlyViewer()

/*!
  Protected constructor for derivees.
*/

SoGtkFlyViewer::SoGtkFlyViewer(// protected
  GtkWidget * parent,
  const char * const name,
  SbBool embed, 
  SoGtkFullViewer::BuildFlag flag, 
  SoGtkViewer::Type type, 
  SbBool build)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(build);
} // SoGtkFlyViewer()

/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkFlyViewer::constructor(// private
  const SbBool build)
{
  this->pimpl = new SoGtkFlyViewerP(this);
  this->common = new SoAnyFlyViewer(this);

  // Cursors.
#if NOTYET
  GdkColormap *colormap = gtk_widget_get_colormap (this->getParentWidget());

  this->orthopixmap =
      gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
        &this->orthomask, (GdkColor *) 0, ortho_xpm);
#endif

  this->setClassName("SoGtkFlyViewer");

//  this->addVisibilityChangeCallback(SoGtkFlyViewer::visibilityCB, this);

  this->setPopupMenuString(_("Fly Viewer"));
  this->setPrefSheetString(_("Fly Viewer Preference Sheet"));

  this->setLeftWheelString(_("Tilt"));
  this->setBottomWheelString(_("Rotate"));
  this->setRightWheelString(_("Dolly"));   

  if (! build) return;
  GtkWidget * viewer = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(viewer);
} // constructor()

/*!
  Destructor.
*/

SoGtkFlyViewer::~SoGtkFlyViewer(
  void)
{
  delete this->common;
  delete this->pimpl;
} // ~SoGtkFlyViewer()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::setViewing(// virtual
  SbBool enable)
{
} // setViewing()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::resetToHomePosition(// virtual
  void)
{
} // resetToHomePosition()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::setCamera(// virtual
  SoCamera * camera)
{
  inherited::setCamera(camera);
} // setCamera()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::setCursorEnabled(// virtual
  SbBool enable)
{
} // setCursorEnabled()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::setCameraType(// virtual
  SoType type)
{
} // setCameraType()

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkFlyViewer::getDefaultWidgetName(// virtual
  void) const
{
  static const char defaultWidgetName[] = "SoGtkFlyViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkFlyViewer::getDefaultTitle(// virtual
  void) const
{
  static const char defaultTitle[] = N_("Fly Viewer");
  return _(defaultTitle);
} // getDefaultTitle()

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkFlyViewer::getDefaultIconTitle(// virtual
  void) const
{
  static const char defaultIconTitle[] = N_("Fly Viewer");
  return _(defaultIconTitle);
} // getDefaultIconTitle()

// *************************************************************************

/*!
  FIXME: write doc
*/

SbBool
SoGtkFlyViewer::processSoEvent(// virtual, protected
  const SoEvent * const event)
{
  if (common->processSoEvent(event))
    return TRUE;
  return inherited::processSoEvent(event);
} // processSoEvent()

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::processEvent(// virtual, protected
  GdkEvent * event)
{
  // SoDebugError::postInfo("SoGtkFlyViewer::processEvent", "[invoked]");
  if (SoGtkViewer::processCommonEvents(event))
    return;

  inherited::processEvent(event);
} // processEvent()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::setSeekMode(// virtual
  SbBool enable)
{
} // setSeekMode()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::actualRedraw(// virtual
  void)
{
  inherited::actualRedraw();
} // actualRedraw()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::rightWheelMotion(// virtual
  float value)
{
} // rightWheelMotion()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::createPrefSheet(// virtual
  void)
{
} // createPrefSheet()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkFlyViewer::openViewerHelpCard(// virtual
  void)
{
  this->openHelpCard("SoGtkFlyViewer.help");
} // openViewerHelpCard()

// ************************************************************************

/*!
  Overloaded to add preferences settings for fly viewer
  specific stuff.
*/

GtkWidget *
SoGtkFlyViewer::makeSubPreferences(
  GtkWidget * parent)
{
  GtkWidget* hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (parent), hbox);

  GtkWidget *label1 = gtk_label_new(_("Flying speed:"));
  gtk_box_pack_start (GTK_BOX (hbox), label1, FALSE, FALSE, 0);

  GtkWidget *incbutton = gtk_button_new_with_label(_("increase"));
  gtk_box_pack_start (GTK_BOX (hbox), incbutton, FALSE, FALSE, 0);

  GtkWidget *decbutton = gtk_button_new_with_label(_("decrease"));
  gtk_box_pack_start (GTK_BOX (hbox), decbutton, FALSE, FALSE, 0);

#if NOTYET
  gtk_signal_connect(GTK_OBJECT(incbutton), "activate",
    GTK_SIGNAL_FUNC(SoGtkFlyViewer::increaseCB), this);
  gtk_signal_connect(GTK_OBJECT(decbutton), "activate",
    GTK_SIGNAL_FUNC(SoGtkFlyViewer::decreaseCB), this);
#endif

  gtk_widget_show_all(hbox);

  return hbox;
}

// *************************************************************************
//
//  Private implementation
//

SoGtkFlyViewerP::SoGtkFlyViewerP(
  SoGtkFlyViewer * publ)
{
  this->pub = publ;
} // SoGtkFlyViewerP()

SoGtkFlyViewerP::~SoGtkFlyViewerP(
  void)
{
} // ~SoGtkFlyViewerP()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkFlyViewerRCSId(void) { return rcsid; }
#endif

