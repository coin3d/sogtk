/**************************************************************************\
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
\**************************************************************************/

#if SOGTK_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOGTK_DEBUG

#include <assert.h>

#include <GL/gl.h>

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkCursors.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <Inventor/Gtk/viewers/SoAnyExaminerViewer.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>

// Icon graphic for the camera button.
#include <Inventor/Gtk/common/pixmaps/ortho.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>

///////// FIXME start //////////////////////////////////////////////////
// Do something clever about this Qt layout assistant code.. (the code
// for expandSize() is inside SoGtkFullViewer.cpp). 990222 mortene.
enum LayoutOrientation { LayoutVertical, LayoutHorizontal };

// extern void expandSize(QSize & result, const QSize & addend,
//                        LayoutOrientation o);
///////// FIXME end ////////////////////////////////////////////////////

/*!
  \class SoGtkExaminerViewer SoGtkExaminerViewer.h Inventor/Qt/viewers/SoGtkExaminerViewer.h
  \brief The SoGtkExaminerViewer class is a full-fledged model viewer
  with functionality for rotation, pan, zoom, etc.
  \ingroup qtviewers

  TODO: more doc
  ...overview of what this class provides over parent class...
  ...keyboard combinations...mousebutton combinations...explain pan,
  rotate, zoom, dolly, seek...explain the fact that we're moving the
  camera, not the scene...
  ...viewer modes (examine vs interact) w/ explanation of what they are
  useful for...
  ...screenshot...
  ...example code...

  \sa SoGtkWalkViewer, SoGtkFlyViewer, SoGtkPlaneViewer, SoGtkCollisionViewer
*/

/*¡
  SoGtkExaminerViewer is missing the class description documentation.
*/

// *************************************************************************

/*!
  Constructor. See parent class for explanation of arguments.
  Calling this constructor will make sure the examiner viewer widget
  will be built immediately.
*/
SoGtkExaminerViewer::SoGtkExaminerViewer(
  GtkWidget * parent,
  const char * name,
  SbBool buildInsideParent,
  SoGtkFullViewer::BuildFlag b,
  SoGtkViewer::Type t )
: inherited(parent, name, buildInsideParent, b, t, FALSE)
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor(TRUE);
}

// *************************************************************************
/*!
  Constructor. See parent class for explanation of arguments.
*/
SoGtkExaminerViewer::SoGtkExaminerViewer(
  GtkWidget * parent,
  const char * name,
  SbBool buildInsideParent,
  SoGtkFullViewer::BuildFlag b,
  SoGtkViewer::Type t,
  SbBool buildNow )
: inherited(parent, name, buildInsideParent, b, t, FALSE)
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor(buildNow);
}

// *************************************************************************
/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkExaminerViewer::constructor(
  SbBool buildNow )
{
  // FIXME: use a smaller sphere than the default one to have a larger
  // area close to the borders that gives us "z-axis rotation"?
  // 990425 mortene.

  this->currentMode = EXAMINE;
//  this->defaultcursor = NULL;
//  this->rotateCursor = NULL;
//  this->panCursor = NULL;
//  this->zoomCursor = NULL;

//  this->pixmaps.orthogonal = new QPixmap((const char **)ortho_xpm);
//  this->pixmaps.perspective = new QPixmap((const char **)perspective_xpm);
//  assert(this->poxmaps.orthogonal->size() == this->pixmaps.perspective->size());

  this->setClassName("SoGtkExaminerViewer");

  this->addVisibilityChangeCallback( SoGtkExaminerViewer::visibilityCB, this );

  this->setPopupMenuString( "Examiner Viewer" );
  this->setPrefSheetString( "Examiner Viewer Preference Sheet" );

  this->setLeftWheelString( "Rotx" );
  this->setBottomWheelString( "Roty" );
  this->setRightWheelString( "Dolly" );

  if ( buildNow ) {
    GtkWidget * viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
  }
}

// *************************************************************************
/*!
  Destructor.
*/
SoGtkExaminerViewer::~SoGtkExaminerViewer(
  void )
{
  // Cursors.
//  delete this->zoomCursor;
//  delete this->panCursor;
//  delete this->rotateCursor;
//  delete this->defaultCursor;

  // Button pixmaps.
  delete this->pixmaps.orthogonal;
  delete this->pixmaps.perspective;

  // Variables used in the spin animation code.
//  delete this->spinDetectTimer;
}

// *************************************************************************

/*!
  This method overloaded from parent class to make sure the mouse
  pointer cursor is updated.
*/

void
SoGtkExaminerViewer::setViewing(SbBool on)
{
  this->setMode(on ? EXAMINE : INTERACT);
  inherited::setViewing(on);
} // setViewing()

// *************************************************************************
/*!
  This method overloaded from parent class to toggle the camera type
  selection button pixmap and string of the zoom/dolly thumbwheel.
*/

void
SoGtkExaminerViewer::setCamera(
  SoCamera * newCamera )
{
  if (newCamera) {
    SoType camtype = newCamera->getTypeId();
    SbBool orthotype =
      camtype.isDerivedFrom( SoOrthographicCamera::getClassTypeId() );

    this->setRightWheelString( orthotype ? "Zoom" : "Dolly");
    if (this->cameraToggleButton) {
//      this->cameraToggleButton->setPixmap( orthotype ?
//        *(this->pixmaps.orthogonal) : *(this->pixmaps.perspective) );
    }
  }

  inherited::setCamera(newCamera);
}

// *************************************************************************
/*!
  Decide whether or not the mouse pointer cursor should be visible in the
  rendering canvas.
*/
void
SoGtkExaminerViewer::setCursorEnabled(SbBool on)
{
  inherited::setCursorEnabled(on);
  this->setCursorRepresentation(this->currentMode);
}

// *************************************************************************
/*!
  Overloaded to provide the examiner viewer functionality on the left
  thumbwheel (x axis rotation).
*/
void
SoGtkExaminerViewer::leftWheelMotion(float val)
{
  common->reorientCamera(SbRotation(SbVec3f(1.0f, 0.0f, 0.0f),
                                  val - this->getLeftWheelValue()));
  inherited::leftWheelMotion(val);
}

// *************************************************************************
/*!
  Overloaded to provide the examiner viewer functionality on the bottom
  thumbwheel (y axis rotation).
*/
void
SoGtkExaminerViewer::bottomWheelMotion(float val)
{
  common->reorientCamera(SbRotation(SbVec3f(0.0f, 1.0f, 0.0f),
                                  this->getBottomWheelValue() - val));
  inherited::bottomWheelMotion(val);
}

// *************************************************************************
/*!
  Overloaded to provide the examiner viewer functionality on the left
  thumbwheel (dolly/zoom).
*/
void
SoGtkExaminerViewer::rightWheelMotion(float val)
{
  common->zoom(val - this->getRightWheelValue());
  inherited::rightWheelMotion(val);
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
/*
  GtkWidget * w = new GtkWidget(parent);
//  w->setBackgroundColor( QColor( 250, 0, 0 ) );

  // Initialize objects keeping track of geometry data.

  QSize totalsize(0, 0);
  QVBoxLayout * toplayout = new QVBoxLayout(w);

  // First, do the single widget on the uppermost row (a checkbox).

  QCheckBox * c1 = new QCheckBox("Enable spin animation", w);
  c1->adjustSize();
  c1->setChecked(this->animatingallowed);
  QObject::connect(c1, SIGNAL(toggled(bool)),
                   this, SLOT(spinAnimationToggled(bool)));

  // Layout row 1.
  toplayout->addWidget(c1, c1->height());
  expandSize(totalsize, c1->size(), LayoutVertical);

  // Do the single widget on the second row (a checkbox).

  QCheckBox * c2 = new QCheckBox("Show point of rotation axes", w);
  c2->adjustSize();
  c2->setChecked(this->isFeedbackVisible());
  QObject::connect(c2, SIGNAL(toggled(bool)),
                   this, SLOT(feedbackVisibilityToggle(bool)));

  // Layout row 2.
  toplayout->addWidget(c2, c2->height());
  expandSize(totalsize, c2->size(), LayoutVertical);


  // Do the four widgets on the third row (label, thumbwheel,
  // lineedit, label).

  QSize tmpsize = QSize(0, 0);

  this->feedbacklabel1 = new QLabel("axes size", w);
  this->feedbacklabel1->adjustSize();
  expandSize(tmpsize, this->feedbacklabel1->size(), LayoutHorizontal);

  this->feedbackwheel = new QtThumbwheel(QtThumbwheel::Horizontal, w);
  QObject::connect(this->feedbackwheel, SIGNAL(wheelMoved(float)),
                   this, SLOT(feedbackSizeChanged(float)));
  this->feedbackwheel->setValue(float(this->getFeedbackSize())/10.0f);
  this->feedbackwheel->adjustSize();
  expandSize(tmpsize, this->feedbackwheel->size(), LayoutHorizontal);

  this->feedbackedit = new QLineEdit(w);
  QObject::connect(this->feedbackedit, SIGNAL(returnPressed()),
                   this, SLOT(feedbackEditPressed()));
  QString s;
  s.setNum(this->getFeedbackSize());
  this->feedbackedit->setText(s);
  this->feedbackedit->adjustSize();
  expandSize(tmpsize, this->feedbackedit->size(), LayoutHorizontal);

  this->feedbacklabel2 = new QLabel("size", w);
  this->feedbacklabel2->adjustSize();
  expandSize(tmpsize, this->feedbacklabel2->size(), LayoutHorizontal);

  // Layout row 3.
  QHBoxLayout * layout = new QHBoxLayout;
  toplayout->addLayout(layout, tmpsize.height());
  layout->addWidget(this->feedbacklabel1, this->feedbacklabel1->width());
  layout->addWidget(this->feedbackwheel, this->feedbackwheel->width());
  layout->addWidget(this->feedbackedit, this->feedbackedit->width());
  layout->addWidget(this->feedbacklabel2, this->feedbacklabel2->width());
  expandSize(totalsize, tmpsize, LayoutVertical);

  w->resize(totalsize);
  toplayout->activate();

  this->setEnableFeedbackControls(this->isFeedbackVisible());

  return w;
*/
  return NULL;
}

// *************************************************************************

/*!
  Overloaded from parent class so we can append the camera type switch
  button in the rightside button column.
*/

void
SoGtkExaminerViewer::createViewerButtons(
  GtkWidget * parent,
  SbPList * buttonlist )
{
  inherited::createViewerButtons(parent, buttonlist);

/*
  this->cameratogglebutton = new QPushButton(parent);
  assert(this->perspectivepixmap);
  assert(this->orthopixmap);
  // Doesn't really matter which pixmap, this is just to make sure
  // that the adjustSize() call will come out right.
  QPixmap * p = this->orthopixmap;
  SoCamera * cam = this->getCamera();
  if (cam) {
    SoType t = cam->getTypeId();
    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
      p = this->orthopixmap;
    else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
      p = this->perspectivepixmap;
    else assert(0);
  }

  this->cameratogglebutton->setPixmap(*p);
  this->cameratogglebutton->adjustSize();

  QObject::connect(this->cameratogglebutton, SIGNAL(clicked()),
                   this, SLOT(cameratoggleClicked()));

  buttonlist->append(this->cameratogglebutton);
*/
}

// *************************************************************************

/*!
  Overloaded to provide name of class.
*/

const char *
SoGtkExaminerViewer::getDefaultWidgetName(void) const
{
  return "SoGtkExaminerViewer";
}

// *************************************************************************

/*!
  Overloaded to provide ``title'' of class.
*/

const char *
SoGtkExaminerViewer::getDefaultTitle(void) const
{
  return "Examiner Viewer";
}

// *************************************************************************

/*!
  Overloaded to provide ``title'' of class.
*/

const char *
SoGtkExaminerViewer::getDefaultIconTitle(void) const
{
  return "Examiner Viewer";
}

// *************************************************************************
/*!
  Pops up the examiner viewer help card.
*/
void
SoGtkExaminerViewer::openViewerHelpCard(void)
{
  this->openHelpCard("SoGtkExaminerViewer.help");
}

// *************************************************************************

SbBool
SoGtkExaminerViewer::processSoEvent( // virtual
  const SoEvent * const event )
{
  if ( common->processSoEvent(event) ) return TRUE;
  return inherited::processSoEvent(event);
} // processSoEvent()

/*!
  Overloaded from parent class to take care of any model interaction
  events.
*/

void
SoGtkExaminerViewer::processEvent( // virtual
  GdkEvent * event )
{
  inherited::processEvent(event);
} // processEvent()

// *************************************************************************

/*!
  Overload this method to make sure any animations are stopped before
  we go into seek mode.
*/

void
SoGtkExaminerViewer::setSeekMode( // virtual
  SbBool enable )
{
#if SOGTK_DEBUG
  if ( enable == this->isSeekMode() ) {
    SoDebugError::postWarning( "SoGtkExaminerViewer::setSeekMode",
                               "seek mode already %sset", enable ? "" : "un");
    return;
  }
#endif // SOGTK_DEBUG
  if ( common->isAnimating() )
    common->stopAnimating();
  inherited::setSeekMode( enable );
//  this->setMode(on ? WAITING_FOR_SEEK : EXAMINE);
} // setSeekMode()

// *************************************************************************

/*!
  Overload this method to be able to draw the axis cross if selected
  in the preferences sheet.
*/

void
SoGtkExaminerViewer::actualRedraw(
  void )
{
  common->actualRedraw();
  inherited::actualRedraw();
  if (common->isFeedbackVisible())
    common->drawAxisCross();
  if (common->isAnimating())
    this->scheduleRedraw();
} // actualRedraw()

// *************************************************************************
/*!
  \internal

  Set the viewer mode based on the given mouse and keyboard state setting.
*/
void
SoGtkExaminerViewer::setModeFromState(const unsigned int state)
{
/*
  ViewerMode mode;
  const unsigned int maskedstate =
    state & (LeftButton|MidButton|ControlButton);

  switch (maskedstate) {
  case 0:
    mode = EXAMINE;
    break;

  case LeftButton:
    mode = DRAGGING;
    break;

  case MidButton:
  case (LeftButton|ControlButton):
    mode = PANNING;
    break;

  case ControlButton:
    mode = WAITING_FOR_PAN;
    break;

  case (MidButton|ControlButton):
  case (LeftButton|MidButton):
  case (LeftButton|MidButton|ControlButton):
    mode = ZOOMING;
    break;

  default: assert(0); break;
  }

  this->setMode(mode);
*/
}

// *************************************************************************
/*!
  \internal

  The viewer is a state machine, and all changes to the current state
  are made through this call.
*/
void
SoGtkExaminerViewer::setMode(const ViewerMode mode)
{
  this->setCursorRepresentation(mode);

  switch (mode) {
  case INTERACT:
    if (common->isAnimating()) common->stopAnimating();
    while (this->getInteractiveCount()) this->interactiveCountDec();
    break;

  case DRAGGING:
    common->spinprojector->project(common->lastmouseposition);
    break;

  case PANNING:
    {
      // The plane we're projecting the mouse coordinates to get 3D
      // coordinates should stay the same during the whole pan
      // operation, so we should calculate this value here.
      SoCamera * cam = this->getCamera();
      SbViewVolume vv = cam->getViewVolume(this->getGLAspectRatio());
      common->panningplane = vv.getPlane(cam->focalDistance.getValue());
    }
    break;

  default: /* include default to avoid compiler warnings. */
    break;
  }

  this->currentMode = mode;
}

// *************************************************************************
/*!
  \internal

  Set cursor graphics according to mode.
*/
void
SoGtkExaminerViewer::setCursorRepresentation(const ViewerMode mode)
{
/*
  GtkWidget * w = this->getRenderAreaWidget();
  assert(w);

  if (!this->defaultcursor) {
    this->defaultcursor = new QCursor(w->cursor());

    QBitmap zoomBtm(so_qt_zoom_width, so_qt_zoom_height,
                    (uchar*)so_qt_zoom_bitmap, FALSE);
    QBitmap zoomMask(so_qt_zoom_width, so_qt_zoom_height,
                     (uchar*)so_qt_zoom_mask_bitmap, FALSE);
    QBitmap panBtm(so_qt_pan_width, so_qt_pan_height,
                   (uchar*)so_qt_pan_bitmap, FALSE);
    QBitmap panMask(so_qt_pan_width, so_qt_pan_height,
                    (uchar*)so_qt_pan_mask_bitmap, FALSE);
    QBitmap rotateBtm(so_qt_rotate_width, so_qt_rotate_height,
                      (uchar*)so_qt_rotate_bitmap, FALSE);
    QBitmap rotateMask(so_qt_rotate_width, so_qt_rotate_height,
                       (uchar*)so_qt_rotate_mask_bitmap, FALSE);

    this->zoomcursor = new QCursor(zoomBtm, zoomMask,
                                   so_qt_zoom_x_hot, so_qt_zoom_y_hot);
    this->pancursor = new QCursor(panBtm, panMask,
                                  so_qt_pan_x_hot, so_qt_pan_y_hot);
    this->rotatecursor = new QCursor(rotateBtm, rotateMask,
                                     so_qt_rotate_x_hot, so_qt_rotate_y_hot);
  }



  if (!this->isCursorEnabled()) {
    w->setCursor(blankCursor);
    return;
  }

  switch (mode) {
  case INTERACT:
    w->setCursor(arrowCursor);
    break;

  case EXAMINE:
  case DRAGGING:
    w->setCursor(* this->rotatecursor);
    break;

  case ZOOMING:
    w->setCursor(* this->zoomcursor);
    break;

  case WAITING_FOR_SEEK:
    w->setCursor(crossCursor);
    break;

  case WAITING_FOR_PAN:
  case PANNING:
    w->setCursor(* this->pancursor);
    break;

  default: assert(0); break;
  }
*/
}

// *************************************************************************

/*!
  \internal
*/
void
SoGtkExaminerViewer::setEnableFeedbackControls(const SbBool flag)
{
//  this->feedbackLabel1->setEnabled(flag);
//  this->feedbackLabel2->setEnabled(flag);
//  this->feedbackWheel->setEnabled(flag);
//  this->feedbackEdit->setEnabled(flag);
}

// *************************************************************************
/*!
  \internal

  This is the regularly called code which makes the spin animation run.
*/
/*
void
SoGtkExaminerViewer::timertriggeredCB(void * data, SoSensor *)
{
  SoGtkExaminerViewer * thisp = (SoGtkExaminerViewer *)data;

#if 0 // debug
  SoDebugError::postInfo("SoGtkExaminerViewer::timertriggeredCB",
                         "spin samples: %d", thisp->spinsamplecounter);
#endif // debug

  if (thisp->spinSampleCounter < 2) {
    // FIXME: won't the first check here always equal TRUE? 990501
    // mortene.
    if (thisp->isAnimating()) thisp->stopAnimating();
#if 0 // check hypothesis from above FIXME statement.
    else thisp->timerTrigger->unschedule();
#else
    else assert(0);
#endif
    return;
  }

  thisp->reorientCamera(thisp->spinIncrement);
}
*/

// *************************************************************************
void
SoGtkExaminerViewer::visibilityCallback(
  SbBool visible )
{
  if ( common->isAnimating() ) {
/*
    if ( visible )
      common->timertrigger->schedule();
    else
      common->timertrigger->unschedule();
*/
  }
} // visibilityCallback()


/*!
  \internal

  This gets called whenever the visibility status of the viewer widget
  changes (for instance on iconization/deiconization).
*/

void
SoGtkExaminerViewer::visibilityCB(void * data, SbBool visible)
{
  ((SoGtkExaminerViewer *) data)->visibilityCallback( visible);
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/
void
SoGtkExaminerViewer::spinAnimationToggled(
  SbBool flag )
{
  common->setAnimationEnabled(flag);
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/
void
SoGtkExaminerViewer::feedbackVisibilityToggle(
  SbBool flag )
{
  common->setFeedbackVisibility(flag);
  this->setEnableFeedbackControls(flag);
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/
void
SoGtkExaminerViewer::feedbackEditPressed()
{
/*
  int val;
  if ((sscanf(this->feedbackedit->text(), "%d", &val) == 1) && (val > 0.0f)) {
    this->feedbackwheel->setValue(float(val)/10.0f);
    this->setFeedbackSize(val);
  }
  else {
    QString s;
    s.setNum(this->getFeedbackSize());
    this->feedbackedit->setText(s);
  }
*/
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/
void
SoGtkExaminerViewer::feedbackWheelPressed()
{
  this->interactiveCountInc();
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/
void
SoGtkExaminerViewer::feedbackWheelReleased()
{
  this->interactiveCountDec();
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/
void
SoGtkExaminerViewer::feedbackSizeChanged(float val)
{
/*
  if (val <= 0.0f) {
    val = 0.1f;
    this->feedbackwheel->setValue(val);
  }

  this->setFeedbackSize(int(val * 10));

  QString s;
  s.setNum(this->getFeedbackSize());
  this->feedbackedit->setText(s);
*/
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/

void
SoGtkExaminerViewer::cameratoggleClicked()
{
  this->toggleCameraType();
}

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkExaminerViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

