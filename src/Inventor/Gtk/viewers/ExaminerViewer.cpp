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

// Class documentation in common/viewers/SoGuiExaminerViewer.cpp.in.

// *************************************************************************

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkCursor.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewerP.h>
#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>

#include <Inventor/Gtk/common/pixmaps/ortho.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkExaminerViewer);

// *************************************************************************

enum {
  CAMERA_BUTTON = 0,

  FIRST_BUTTON = 0,
  LAST_BUTTON = CAMERA_BUTTON
};

struct SoGtkViewerButton
SoGtkExaminerViewerP::SoGtkExaminerViewerButtons[] = {
  { // camera type button
    N_("camera"), "C",   
    (GtkSignalFunc) SoGtkExaminerViewerP::camerabuttonCB,
    perspective_xpm
  }
};

// *************************************************************************

/*!
  Constructor. See parent class for explanation of arguments.
  Calling this constructor will make sure the examiner viewer widget
  will be built immediately.
*/

SoGtkExaminerViewer::SoGtkExaminerViewer(GtkWidget * parent,
                                         const char * name,
                                         SbBool embed,
                                         SoGtkFullViewer::BuildFlag flags,
                                         SoGtkViewer::Type type)
  : inherited(parent, name, embed, flags, type, FALSE)
{
  PRIVATE(this) = new SoGtkExaminerViewerP(this);
  PRIVATE(this)->constructor(TRUE);
}

/*!
  Constructor. See parent class for explanation of arguments.
*/

SoGtkExaminerViewer::SoGtkExaminerViewer(GtkWidget * parent,
                                         const char * name,
                                         SbBool embed,
                                         SoGtkFullViewer::BuildFlag flags,
                                         SoGtkViewer::Type type,
                                         SbBool build)
  : inherited(parent, name, embed, flags, type, FALSE)
{
  PRIVATE(this) = new SoGtkExaminerViewerP(this);
  PRIVATE(this)->constructor(build);
}

/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkExaminerViewerP::constructor(const SbBool build)
{
  this->genericConstructor();

  PUBLIC(this)->setClassName("SoGtkExaminerViewer");

//  this->addVisibilityChangeCallback(SoGtkExaminerViewerP::visibilityCB, this);

  PUBLIC(this)->setPopupMenuString(_("Examiner Viewer"));

  PUBLIC(this)->setLeftWheelString(_("Rotx"));
  PUBLIC(this)->setBottomWheelString(_("Roty"));
  PUBLIC(this)->setRightWheelString(_("Dolly"));

  if (! build) return;
  GtkWidget * viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
  PUBLIC(this)->setBaseWidget(viewer);
}

/*!
  Destructor.
*/

SoGtkExaminerViewer::~SoGtkExaminerViewer()
{
  PRIVATE(this)->genericDestructor();
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoGtkExaminerViewer::setCamera(SoCamera * newCamera)
{
  // This method overloaded from parent class to toggle the camera
  // type selection button pixmap and string of the zoom/dolly
  // thumbwheel.

  if (newCamera) {
    const SoType camtype(newCamera->getTypeId());
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

// Documented in superclass. Overridden so we can append the camera
// type switch button in the rightside button column.
void
SoGtkExaminerViewer::createViewerButtons(GtkWidget * parent,
                                         SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);
  
  GtkTooltips * tooltips = gtk_tooltips_new();
  GdkColormap * colormap = gtk_widget_get_colormap(parent);

  const size_t buttons = sizeof(SoGtkExaminerViewerP::SoGtkExaminerViewerButtons)
    / sizeof(SoGtkViewerButton);
  for (size_t button = 0; button < buttons; button++) {
    GtkWidget	*widget = (GtkWidget*) 0;

    switch(button)
      {
      case CAMERA_BUTTON :
        PRIVATE(this)->cameratogglebutton = widget = gtk_button_new();
        break;
      default:
        break;
      }

    GTK_WIDGET_UNSET_FLAGS (widget, GTK_CAN_FOCUS);
    gtk_tooltips_set_tip(tooltips, widget, 
                         _(SoGtkExaminerViewerP::SoGtkExaminerViewerButtons[button].keyword), NULL);

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
          else
            assert(0 && "unsupported cameratype");
        }
        break;
      default:
        break;
      }

    GtkWidget * label = gtk_pixmap_new(gdk_pixmap, gdk_mask);
    gtk_widget_show(label);

    gtk_container_add(GTK_CONTAINER(widget), GTK_WIDGET(label));
    if (SoGtkExaminerViewerP::SoGtkExaminerViewerButtons[button].pressed != NULL) {
      gtk_signal_connect(GTK_OBJECT(widget), "pressed",
                         GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::SoGtkExaminerViewerButtons[button].pressed),
                         (gpointer) this);
    }
    buttonlist->append(widget);
  }
}

// *************************************************************************
//
//  Private implementation
//

SoGtkExaminerViewerP::SoGtkExaminerViewerP(SoGtkExaminerViewer * publ)
  : SoGuiExaminerViewerP(publ)
{
  GdkColormap * colormap = gtk_widget_get_colormap (PUBLIC(this)->getParentWidget());

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

SoGtkExaminerViewerP::~SoGtkExaminerViewerP()
{
  // Button pixmaps.
  gdk_pixmap_unref(this->orthopixmap);
  gdk_bitmap_unref(this->orthomask);

  gdk_pixmap_unref(this->perspectivepixmap);
  gdk_bitmap_unref(this->perspectivemask);

}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::camerabuttonCB(GtkWidget *,
                                     gpointer closure)
{
  assert(closure != NULL);
  SoGtkExaminerViewer * viewer = (SoGtkExaminerViewer *) closure;
  if (viewer->getCamera())
    viewer->toggleCameraType();
}

// *************************************************************************
