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
#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>

#include <Inventor/Gtk/common/pixmaps/ortho.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkExaminerViewer);

// *************************************************************************

class SoGtkExaminerViewerP {
public:
  SoGtkExaminerViewerP(SoGtkExaminerViewer * publ);
  ~SoGtkExaminerViewerP(void);

  static SoGtkViewerButton SoGtkExaminerViewerButtons[];

  GdkPixmap * orthopixmap, * perspectivepixmap;
  GdkBitmap * orthomask, * perspectivemask;

  GtkWidget * cameratogglebutton;
  GtkWidget * feedbacklabel1;
  GtkWidget * feedbacklabel2;
  GtkWidget * feedbackwheel;
  GtkWidget * feedbackedit;
  void setEnableFeedbackControls(const SbBool flag);

  // Pref sheet.
  static void spinAnimationToggled(GtkToggleButton *, gpointer);
  static void feedbackVisibilityToggled(GtkToggleButton *, gpointer);
  static void feedbackEditPressed(GtkEntry *, gpointer);
  static void feedbackWheelPressed(GtkWidget *, gpointer);
  static void feedbackSizeChanged(GtkWidget *, gpointer);
  static void feedbackWheelReleased(GtkWidget *, gpointer);

  // Button row.
  static void camerabuttonCB(GtkWidget *, gpointer);

private:
  SoGtkExaminerViewer * pub;

};

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

#define THIS (PRIVATE(this))

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
  this->constructor(TRUE);
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
  this->constructor(build);
}

/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkExaminerViewer::constructor(const SbBool build)
{
  this->genericConstructor();

  this->pimpl = new SoGtkExaminerViewerP(this);

  this->setClassName("SoGtkExaminerViewer");

//  this->addVisibilityChangeCallback(SoGtkExaminerViewerP::visibilityCB, this);

  this->setPopupMenuString(_("Examiner Viewer"));
  this->setPrefSheetString(_("Examiner Viewer Preference Sheet"));

  this->setLeftWheelString(_("Rotx"));
  this->setBottomWheelString(_("Roty"));
  this->setRightWheelString(_("Dolly"));

  if (! build) return;
  GtkWidget * viewer = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(viewer);
}

/*!
  Destructor.
*/

SoGtkExaminerViewer::~SoGtkExaminerViewer()
{
  this->genericDestructor();
  delete this->pimpl;
}

// *************************************************************************

/*!
  FIXME: write doc
*/
void
SoGtkExaminerViewer::setAnimationEnabled(const SbBool enable)
{ // FIXME: make this virtual?  20001230 larsa
  // FIXME: update pref-sheet widget with the value. 20020603 mortene.
  this->setGenericAnimationEnabled(enable);
}
 

// *************************************************************************

/*!
  FIXME: write doc
*/

// make this virtual?  20001230 larsa
void
SoGtkExaminerViewer::setFeedbackSize(const int size)
{
  // FIXME: update pref-sheet widget with the value. 20020603 mortene.
  this->setGenericFeedbackSize(size);
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

/*!
  Overloaded to add preferences settings for examiner viewer
  specific stuff (enable/disable possible spin animation,
  enable/disable/configure axis cross graphics).
*/

GtkWidget *
SoGtkExaminerViewer::makeSubPreferences(GtkWidget * parent)
{
  GtkWidget* form1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form1);
  gtk_container_add (GTK_CONTAINER (parent), form1);

  GtkWidget *checkbutton1 = gtk_check_button_new_with_label (
    _("Enable spin animation"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), 
     this->isAnimationEnabled());
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (form1), checkbutton1, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(checkbutton1), "toggled", 
    GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::spinAnimationToggled), this);

  // Do the single widget on the second row (a checkbutton).
  GtkWidget *checkbutton2 = gtk_check_button_new_with_label (
    _("Show point of rotation axes"));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), 
     this->isFeedbackVisible());
  gtk_widget_show (checkbutton2);
  gtk_box_pack_start (GTK_BOX (form1), checkbutton2, FALSE, FALSE, 0);
  gtk_signal_connect(GTK_OBJECT(checkbutton2), "toggled", 
    GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::feedbackVisibilityToggled), this);

  GtkWidget* hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (parent), hbox);

  // Do the four widgets on the third row (label, thumbwheel,
  // lineedit, label).

  PRIVATE(this)->feedbacklabel1 = gtk_label_new(_("axes size"));
  gtk_box_pack_start (GTK_BOX (hbox), PRIVATE(this)->feedbacklabel1, FALSE, FALSE, 0);

  PRIVATE(this)->feedbackwheel = gtk_thumbwheel_new(0);
  gtk_box_pack_start (GTK_BOX (hbox), PRIVATE(this)->feedbackwheel, FALSE, FALSE, 0);
  gtk_thumbwheel_set_range_boundary_handling(
    GTK_THUMBWHEEL(PRIVATE(this)->feedbackwheel), GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE);

  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->feedbackwheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::feedbackWheelPressed), this);
  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->feedbackwheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::feedbackSizeChanged), this);
  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->feedbackwheel), "released",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::feedbackWheelReleased), this);

  gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(this)->feedbackwheel),
    float(this->getFeedbackSize())/10.0f);

  PRIVATE(this)->feedbackedit = gtk_entry_new();
  gtk_box_pack_start (GTK_BOX (hbox), PRIVATE(this)->feedbackedit, FALSE, FALSE, 0);
  gtk_widget_set_usize (PRIVATE(this)->feedbackedit, 48, 24);

  gtk_signal_connect(GTK_OBJECT(PRIVATE(this)->feedbackedit), "activate",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewerP::feedbackEditPressed), this);

  char buffer[16] ;
  sprintf(buffer, "%d", this->getFeedbackSize());
  gtk_entry_set_text(GTK_ENTRY(PRIVATE(this)->feedbackedit), buffer);

  PRIVATE(this)->feedbacklabel2 = gtk_label_new(_("pixels"));
  gtk_box_pack_start (GTK_BOX (hbox), PRIVATE(this)->feedbacklabel2, FALSE, FALSE, 0);

  gtk_widget_show_all(hbox);

  PRIVATE(this)->setEnableFeedbackControls(this->isFeedbackVisible());

  return form1;
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
{
  this->pub = publ;

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
SoGtkExaminerViewerP::setEnableFeedbackControls(const SbBool flag)
{
  gtk_widget_set_sensitive(this->feedbacklabel1, flag);
  gtk_widget_set_sensitive(this->feedbacklabel2, flag);
  gtk_widget_set_sensitive(this->feedbackwheel, flag);
  gtk_widget_set_sensitive(this->feedbackedit, flag);
}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::spinAnimationToggled(GtkToggleButton * w,  
                                           gpointer closure)
{
  assert(closure != NULL);
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure;
  SbBool flag = gtk_toggle_button_get_active(w) ? TRUE : FALSE;

  viewer->setAnimationEnabled(flag);
}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::feedbackVisibilityToggled(GtkToggleButton * w,  
                                                gpointer closure)
{
  assert(closure != NULL);
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure;
  SbBool flag = gtk_toggle_button_get_active(w) ? TRUE : FALSE;

  viewer->setFeedbackVisibility(flag);
  PRIVATE(viewer)->setEnableFeedbackControls(flag);
}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::feedbackEditPressed(GtkEntry * w,
                                          gpointer closure)
{
  assert(closure != NULL);
  SoGtkExaminerViewer	*viewer = (SoGtkExaminerViewer*) closure;

  char *s = gtk_editable_get_chars(GTK_EDITABLE(w), 0, -1);
  int val;
  if ((sscanf(s, "%d", &val) == 1) && (val > 0)) {
    gtk_thumbwheel_set_value(GTK_THUMBWHEEL(PRIVATE(viewer)->feedbackwheel), 
      float(val)/10.0f);
    viewer->setFeedbackSize(val);
  }
  g_free(s);

  /* else */
  {
    char buffer[16];
    sprintf(buffer, "%d", viewer->getFeedbackSize());
    gtk_entry_set_text(GTK_ENTRY(w), buffer);
  }
}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::feedbackWheelPressed(GtkWidget * w,
                                           gpointer closure)
{
  assert(closure != NULL);
  GtkThumbWheel *thumbwheel = (GtkThumbWheel *) w;
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure;
  viewer->interactiveCountInc();
}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::feedbackWheelReleased(GtkWidget * w,
                                            gpointer closure)
{
  assert(closure != NULL);
  GtkThumbWheel *thumbwheel = (GtkThumbWheel *) w;
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure;
  viewer->interactiveCountDec();
}

// *************************************************************************

/*
  internal
*/

void
SoGtkExaminerViewerP::feedbackSizeChanged(GtkWidget * w,
                                          gpointer closure)
{
  assert(closure != NULL);
  GtkThumbWheel		*thumbwheel = (GtkThumbWheel*) w;
  SoGtkExaminerViewer	*viewer = (SoGtkExaminerViewer*) closure;

  gfloat val = gtk_thumbwheel_get_value(thumbwheel);
  if (val < 0.1f) {
    val = 0.1f;
    gtk_thumbwheel_set_value(thumbwheel, val);
  }

  viewer->setFeedbackSize(int(val * 10.0f));

  char buffer[16];
  sprintf(buffer, "%d", viewer->getFeedbackSize());
  gtk_entry_set_text(GTK_ENTRY(PRIVATE(viewer)->feedbackedit), buffer);
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
