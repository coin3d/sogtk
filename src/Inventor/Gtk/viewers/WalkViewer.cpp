/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

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
