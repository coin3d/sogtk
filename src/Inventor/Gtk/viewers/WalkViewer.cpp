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

// ************************************************************************

// Class is documented in common/viewers/SoGuiWalkViewer.cpp.in.

// ************************************************************************

#if HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/events/SoEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>
#include <Inventor/Gtk/viewers/SoGtkWalkViewer.h>
#include <Inventor/Gtk/viewers/SoGuiWalkViewerP.h>

// ************************************************************************

class SoGtkWalkViewerP : public SoGuiWalkViewerP {
public:
  SoGtkWalkViewerP(SoGtkWalkViewer * publ);
  ~SoGtkWalkViewerP();
};

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

// ************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkWalkViewer);

// ************************************************************************

/*!
  The public constructor.
*/

SoGtkWalkViewer::SoGtkWalkViewer(GtkWidget * parent,
                                 const char * name,
                                 SbBool embed,
                                 SoGtkFullViewer::BuildFlag flag,
                                 SoGtkViewer::Type type)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(TRUE);
}

/*!
  A protected constructor.
*/

SoGtkWalkViewer::SoGtkWalkViewer(GtkWidget * parent,
                                 const char * name,
                                 SbBool embed,
                                 SoGtkFullViewer::BuildFlag flag,
                                 SoGtkViewer::Type type,
                                 SbBool build)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(build);
}

/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkWalkViewer::constructor(const SbBool build)
{
  PRIVATE(this) = new SoGtkWalkViewerP(this);

#if NOTYET
  GdkColormap *colormap = gtk_widget_get_colormap (this->getParentWidget());

  this->orthopixmap =
      gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
        &this->orthomask, (GdkColor *) 0, ortho_xpm);
#endif

  this->setClassName("SoGtkWalkViewer");

//  this->addVisibilityChangeCallback(SoGtkWalkViewer::visibilityCB, this);

  this->setPopupMenuString(_("Walk Viewer"));

  this->setLeftWheelString(_("Tilt"));
  this->setBottomWheelString(_("Rotate"));
  this->setRightWheelString(_("Dolly"));

  if (! build) return;
  GtkWidget * viewer = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(viewer);
}

/*!
  The destructor.
*/

SoGtkWalkViewer::~SoGtkWalkViewer()
{
  delete PRIVATE(this);
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setViewing(SbBool enable)
{
  inherited::setViewing(enable);
}

// ************************************************************************

// Documented in superclass.
void
SoGtkWalkViewer::setCamera(SoCamera * camera)
{
  inherited::setCamera(camera);
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setCursorEnabled(SbBool enable)
{
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setCameraType(SoType type)
{
}

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkWalkViewer::getDefaultWidgetName(void) const
{
  static const char defaultWidgetName[] = N_("Walk Viewer");
  return _(defaultWidgetName);
}

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkWalkViewer::getDefaultTitle(void) const
{
  static const char defaultTitle[] = N_("Walk Viewer");
  return _(defaultTitle);
}

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkWalkViewer::getDefaultIconTitle(void) const
{
  static const char defaultIconTitle[] = N_("Walk Viewer");
  return _(defaultIconTitle);
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::processEvent(GdkEvent * event)
{
  inherited::processEvent(event);
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setSeekMode(SbBool enable)
{
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::actualRedraw(void)
{
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::rightWheelMotion(float value)
{
}

// ************************************************************************

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkWalkViewer::buildLeftTrim(GtkWidget * parent)
{
  // get the examiner viewer's left vbox
  GtkWidget *trim = inherited::buildLeftTrim(parent);

  GtkWidget *uleftWheel = gtk_thumbwheel_new(1);   
  gtk_misc_set_padding(GTK_MISC(uleftWheel), 2, 2);
  gtk_box_pack_end(GTK_BOX(trim), uleftWheel, FALSE, TRUE, 0);
  gtk_widget_show(uleftWheel);

#if NOTYET
  gtk_signal_connect(GTK_OBJECT(uleftWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelPressedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(uleftWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelMovedCB), (gpointer) this);
  gtk_signal_connect(GTK_OBJECT(uleftWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelReleasedCB), (gpointer) this);
#endif
  GtkWidget *uleftStr = gtk_label_new(_("H"));
  gtk_box_pack_end(GTK_BOX(trim), uleftStr, FALSE, TRUE, FALSE);
  gtk_widget_show(uleftStr);

  // HACK: reorder the leftWheel to the end of the vbox
  gtk_box_reorder_child(GTK_BOX(trim), this->leftWheel, INT_MAX);

  return trim ;
}

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::openViewerHelpCard(void)
{
  this->openHelpCard("SoGtkWalkViewer.help");
}

// ************************************************************************
//
//  Private implementation
//

SoGtkWalkViewerP::SoGtkWalkViewerP(SoGtkWalkViewer * publ)
  : SoGuiWalkViewerP(publ)
{
}

SoGtkWalkViewerP::~SoGtkWalkViewerP()
{
}

// *************************************************************************
