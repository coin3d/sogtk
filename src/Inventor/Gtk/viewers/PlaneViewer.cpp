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

// *************************************************************************

// Class documentation in common/viewers/SoGuiPlaneViewer.cpp.in.

// *************************************************************************

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewerP.h>
#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>

#include <Inventor/Gtk/SoGtkCursor.h>
#include <Inventor/Gtk/common/pixmaps/x.xpm>
#include <Inventor/Gtk/common/pixmaps/y.xpm>
#include <Inventor/Gtk/common/pixmaps/z.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>
#include <Inventor/Gtk/common/pixmaps/ortho.xpm>

// *************************************************************************

// SoGtkPlaneViewerP "private implementation" class.

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

enum {
  X_BUTTON = 0,
  Y_BUTTON = 1,
  Z_BUTTON = 2,
  CAMERA_BUTTON = 3,

  FIRST_BUTTON = 0,
  LAST_BUTTON = CAMERA_BUTTON
};

struct SoGtkViewerButton
SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[] = {
  { // plane X button
    N_("x"), "X",
    (GtkSignalFunc) SoGtkPlaneViewerP::xbuttonCB,
    x_xpm
  },
  { // plane Y button
    N_("y"), "Y",
    (GtkSignalFunc) SoGtkPlaneViewerP::ybuttonCB,
    y_xpm
  },
  { // plane Z button
    N_("z"), "Z",
    (GtkSignalFunc) SoGtkPlaneViewerP::zbuttonCB,
    z_xpm
  },
  { // camera type button
    N_("camera"), "C",
    (GtkSignalFunc) SoGtkPlaneViewerP::camerabuttonCB,
    perspective_xpm
  }
};

SoGtkPlaneViewerP::SoGtkPlaneViewerP(SoGtkPlaneViewer * publ)
  : SoGuiPlaneViewerP(publ)
{
  GdkColormap * colormap = gtk_widget_get_colormap(PUBLIC(this)->getParentWidget());

  this->orthopixmap =
    gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
                                          &this->orthomask, (GdkColor *) 0,
                                          // FIXME: nasty cast, get
                                          // rid of it. 20020320 mortene.
                                          (gchar **)ortho_xpm);

  this->perspectivepixmap =
    gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
                                          &this->perspectivemask,
                                          (GdkColor *) 0,
                                          // FIXME: nasty cast, get
                                          // rid of it. 20020320 mortene.
                                          (gchar **)perspective_xpm);
}

SoGtkPlaneViewerP::~SoGtkPlaneViewerP()
{
  // Button pixmaps.
  gdk_pixmap_unref(this->orthopixmap);
  gdk_bitmap_unref(this->orthomask);  
  gdk_pixmap_unref(this->perspectivepixmap);
  gdk_bitmap_unref(this->perspectivemask);  
}

void
SoGtkPlaneViewerP::constructor(const SbBool build)
{
  this->commonConstructor(); // set up generic stuff

  PUBLIC(this)->setClassName("SoGtkPlaneViewer");

  PUBLIC(this)->setPopupMenuString(_("Plane Viewer"));
  PUBLIC(this)->setPrefSheetString(_("Plane Viewer Preference Sheet")); 

  PUBLIC(this)->setLeftWheelString(_("Translate Y"));
  PUBLIC(this)->setBottomWheelString(_("Translate X"));
  PUBLIC(this)->setRightWheelString(_("Zoom"));

  if (! build) return;
  GtkWidget * viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
  PUBLIC(this)->setBaseWidget(viewer);
//  PUBLIC(this)->setSize(SbVec2s(500, 460));
}


void
SoGtkPlaneViewerP::xbuttonCB(GtkWidget * button, gpointer closure)
{
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  PRIVATE(viewer)->viewPlaneX();
}

void
SoGtkPlaneViewerP::ybuttonCB(GtkWidget * button, gpointer closure)
{
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  PRIVATE(viewer)->viewPlaneY();
}

void
SoGtkPlaneViewerP::zbuttonCB(GtkWidget * button, gpointer closure)
{
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  PRIVATE(viewer)->viewPlaneZ();
}

void
SoGtkPlaneViewerP::camerabuttonCB(GtkWidget * button, gpointer closure)
{
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  if (viewer->getCamera()) { viewer->toggleCameraType(); }
}


// *************************************************************************

// Documented in common/viewers/SoGuiPlaneViewer.cpp.in.
SoGtkPlaneViewer::SoGtkPlaneViewer(GtkWidget * parent,
                                   const char * const name,
                                   const SbBool embed,
                                   const SoGtkFullViewer::BuildFlag flag,
                                   const SoGtkViewer::Type type)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoGtkPlaneViewerP(this);
  PRIVATE(this)->constructor(TRUE);
}

// Documented in common/viewers/SoGuiPlaneViewer.cpp.in.
SoGtkPlaneViewer::SoGtkPlaneViewer(GtkWidget * parent,
                                   const char * const name,
                                   const SbBool embed,
                                   const SoGtkFullViewer::BuildFlag flag,
                                   const SoGtkViewer::Type type,
                                   const SbBool build)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoGtkPlaneViewerP(this);
  PRIVATE(this)->constructor(build);
}

// Documented in common/viewers/SoGuiPlaneViewer.cpp.in.
SoGtkPlaneViewer::~SoGtkPlaneViewer()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoGtkPlaneViewer::setCamera(SoCamera * newCamera)
{
  if (newCamera) {
    const SoType camtype = newCamera->getTypeId() ;
    const SbBool orthotype =
      camtype.isDerivedFrom(SoOrthographicCamera::getClassTypeId());

    this->setRightWheelString(orthotype ? _("Zoom") : _("Dolly"));
    if (PRIVATE(this)->cameratogglebutton) {
      GtkWidget * label = GTK_BIN(PRIVATE(this)->cameratogglebutton)->child;
      gtk_pixmap_set(GTK_PIXMAP(label),
        (orthotype ? PRIVATE(this)->orthopixmap : PRIVATE(this)->perspectivepixmap),
        (orthotype ? PRIVATE(this)->orthomask : PRIVATE(this)->perspectivemask));
    }
  }
  inherited::setCamera(newCamera);
}

// *************************************************************************

// Documented in superclass.
GtkWidget *
SoGtkPlaneViewer::buildWidget(GtkWidget * parent)
{
  return inherited::buildWidget(parent);
}

// *************************************************************************

// Documented in superclass.
void
SoGtkPlaneViewer::createViewerButtons(GtkWidget * parent,
                                      SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);

  GtkTooltips * tooltips = gtk_tooltips_new();
  GdkColormap * colormap = gtk_widget_get_colormap(parent);

  const size_t buttons = sizeof(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons)
                         / sizeof(SoGtkViewerButton);
  for (size_t button = 0; button < buttons; button++) {
    GtkWidget * widget = (GtkWidget*) 0 ;

    switch(button)
    {
    case CAMERA_BUTTON :
      PRIVATE(this)->cameratogglebutton = widget = gtk_button_new();
      break ;
    default: 
      widget = gtk_button_new();
      break ;
    }

    GTK_WIDGET_UNSET_FLAGS(widget,GTK_CAN_FOCUS);
    gtk_tooltips_set_tip(tooltips, widget, 
      _(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].keyword), NULL);

    GdkPixmap * gdk_pixmap = (GdkPixmap *) 0;
    GdkBitmap * gdk_mask   = (GdkBitmap *) 0;

    switch(button)
    {
    case CAMERA_BUTTON :
      {
        SoType t = this->getCameraType();
        if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
        {
          gdk_pixmap = PRIVATE(this)->orthopixmap;
          gdk_mask   = PRIVATE(this)->orthomask;
        }
        else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        {
          gdk_pixmap = PRIVATE(this)->perspectivepixmap;
          gdk_mask   = PRIVATE(this)->perspectivemask;
        }
        else assert(0 && "unsupported cameratype");
      }
      break;
    default:
      gdk_pixmap =
        gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
                                              &gdk_mask, (GdkColor *) 0,
                                              // FIXME: nasty cast,
                                              // get rid of
                                              // it. 20020320 mortene.
                                              (gchar **)SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].xpm_data);
      break;
    }


    GtkWidget * label = gtk_pixmap_new(gdk_pixmap, gdk_mask);
    gtk_widget_show(label);

    switch(button)
    {
    case CAMERA_BUTTON:
      break ;
    default :
      gdk_pixmap_unref(gdk_pixmap);
      gdk_bitmap_unref(gdk_mask);
      break ;
    };

    gtk_container_add(GTK_CONTAINER(widget), GTK_WIDGET(label));
    if (SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].pressed != NULL) {
      gtk_signal_connect(GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].pressed),
        (gpointer) this);
    }
    buttonlist->append(widget);
  }
}

// ************************************************************************

// Documented in superclass.
void
SoGtkPlaneViewer::createPrefSheet(void)
{
  SOGTK_STUB();
  inherited::createPrefSheet();
}

// ************************************************************************
