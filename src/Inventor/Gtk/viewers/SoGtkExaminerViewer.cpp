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

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkCursors.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <Inventor/Gtk/viewers/SoAnyExaminerViewer.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>

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
  \class SoGtkExaminerViewer Inventor/Gtk/viewers/SoGtkExaminerViewer.h
  \brief The SoGtkExaminerViewer class is a full-fledged model viewer
  with functionality for rotation, pan, zoom, etc.
  \ingroup gtkviewers

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

// *************************************************************************

/*!
  Constructor. See parent class for explanation of arguments.
  Calling this constructor will make sure the examiner viewer widget
  will be built immediately.
*/

SoGtkExaminerViewer::SoGtkExaminerViewer(
  GtkWidget * parent,
  const char * name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag flags,
  SoGtkViewer::Type type )
: inherited( parent, name, embed, flags, type, FALSE )
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor( TRUE );
} // SoGtkExaminerViewer()

/*!
  Constructor. See parent class for explanation of arguments.
*/

SoGtkExaminerViewer::SoGtkExaminerViewer( // protected
  GtkWidget * parent,
  const char * name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag flags,
  SoGtkViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, flags, type, FALSE )
, common( new SoAnyExaminerViewer( this ) )
{
  this->constructor( build );
} // SoGtkExaminerViewer()

/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkExaminerViewer::constructor( // private
  const SbBool build )
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

//  this->addVisibilityChangeCallback( SoGtkExaminerViewer::visibilityCB, this );

  this->setPopupMenuString( "Examiner Viewer" );
  this->setPrefSheetString( "Examiner Viewer Preference Sheet" );

  this->setLeftWheelString( "Rotx" );
  this->setBottomWheelString( "Roty" );
  this->setRightWheelString( "Dolly" );

  if ( build ) {
    GtkWidget * viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
  }
} // constructor()

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
//  delete this->pixmaps.orthogonal;
//  delete this->pixmaps.perspective;

  // Variables used in the spin animation code.
//  delete this->spinDetectTimer;
} // ~SoGtkExaminerViewer()

// *************************************************************************

/*!
  This method overloaded from parent class to make sure the mouse
  pointer cursor is updated.
*/

void
SoGtkExaminerViewer::setViewing( // virtual
  SbBool enable )
{
  this->setMode( enable ? EXAMINE : INTERACT );
  inherited::setViewing( enable );
} // setViewing()

// *************************************************************************

/*!
*/

void
SoGtkExaminerViewer::setAnimationEnabled(
  const SbBool enable )
{
  common->setAnimationEnabled( enable );
} // setAnimationEnabled()
 
/*!
*/

SbBool
SoGtkExaminerViewer::isAnimationEnabled(
  void ) const
{
  return common->isAnimationEnabled();
} // isAnimationEnabled()

// *************************************************************************

/*!
*/

void
SoGtkExaminerViewer::stopAnimating(
  void )
{
  common->stopAnimating();
} // stopAnimating()

/*!
*/

SbBool
SoGtkExaminerViewer::isAnimating(
  void ) const
{
  return common->isAnimating();
} // isAnimating()

// *************************************************************************

/*!
*/

void
SoGtkExaminerViewer::setFeedbackVisibility(
  const SbBool enable )
{
  common->setFeedbackVisibility( enable );
} // setFeedbackVisibility()

/*!
*/

SbBool
SoGtkExaminerViewer::isFeedbackVisible(
  void ) const
{
  return common->isFeedbackVisible();
} // isFeedbackVisible()

// *************************************************************************

/*!
*/

void
SoGtkExaminerViewer::setFeedbackSize(
  const int size )
{
  common->setFeedbackSize( size );
} // setFeedbackSize()

/*!
*/

int
SoGtkExaminerViewer::getFeedbackSize(
  void ) const
{
  return common->getFeedbackSize();
} // getFeedbackSize()

// *************************************************************************

/*!
  This method overloaded from parent class to toggle the camera type
  selection button pixmap and string of the zoom/dolly thumbwheel.
*/

void
SoGtkExaminerViewer::setCamera( // virtual
  SoCamera * camera )
{
  if ( camera ) {
    const SoType camtype( camera->getTypeId() );
    const SbBool orthotype =
      camtype.isDerivedFrom( SoOrthographicCamera::getClassTypeId() );

    this->setRightWheelString( orthotype ? "Zoom" : "Dolly");
//    if (this->cameraToggleButton) {
//      this->cameraToggleButton->setPixmap( orthotype ?
//        *(this->pixmaps.orthogonal) : *(this->pixmaps.perspective) );
//    }
  }
  inherited::setCamera( camera );
} // setCamera()

// *************************************************************************

/*!
  Decide whether or not the mouse pointer cursor should be visible in the
  rendering canvas.
*/

/*
void
SoGtkExaminerViewer::setCursorEnabled(SbBool on)
{
  inherited::setCursorEnabled(on);
  this->setCursorRepresentation(this->currentMode);
}
*/

// *************************************************************************

/*!
  Overloaded to stop spin animation when wheel is being handled.
*/

void
SoGtkExaminerViewer::leftWheelStart( // virtual
  void )
{
  if ( common->isAnimating() )
    common->stopAnimating();
  inherited::leftWheelStart();
} // leftWheelStart()

/*!
  Overloaded to provide the examiner viewer functionality on the left
  thumbwheel (x axis rotation).
*/

void
SoGtkExaminerViewer::leftWheelMotion( // virtual
  float value )
{
  inherited::leftWheelMotion(
    common->rotXWheelMotion( value, this->getLeftWheelValue() ) );
} // leftWheelMotion()

/*!
  Overloaded to stop spin animation when wheel is being handled.
*/

void
SoGtkExaminerViewer::bottomWheelStart( // virtual
  void )
{
  if ( this->isAnimating() )
    common->stopAnimating();
  inherited::bottomWheelStart();
} // bottomWheelStart()

/*!
  Overloaded to provide the examiner viewer functionality on the bottom
  thumbwheel (y axis rotation).
*/

void
SoGtkExaminerViewer::bottomWheelMotion( // virtual
  float value )
{
  inherited::bottomWheelMotion(
    common->rotYWheelMotion( value, this->getBottomWheelValue() ) );
} // bottomWheelMotion()

/*!
  Overloaded to provide the examiner viewer functionality on the left
  thumbwheel (dolly/zoom).
*/

void
SoGtkExaminerViewer::rightWheelMotion( // virtual
  float value )
{
  common->zoom( this->getRightWheelValue() - value );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

// *************************************************************************

/*!
  Overloaded to add preferences settings for examiner viewer
  specific stuff (enable/disable possible spin animation,
  enable/disable/configure axis cross graphics).
*/

GtkWidget *
SoGtkExaminerViewer::makeSubPreferences(
  GtkWidget * parent )
{
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
}

// *************************************************************************

/*!
  Overloaded to provide name of class.
*/

const char *
SoGtkExaminerViewer::getDefaultWidgetName(void) const
{
  static const char defaultWidgetName[] = "SoGtkExaminerViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  Overloaded to provide ``title'' of class.
*/

const char *
SoGtkExaminerViewer::getDefaultTitle(void) const
{
  static const char defaultTitle[] = "Examiner Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
  Overloaded to provide ``title'' of class.
*/

const char *
SoGtkExaminerViewer::getDefaultIconTitle(void) const
{
  static const char defaultIconTitle[] = "Examiner Viewer";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************
/*!
  Pops up the examiner viewer help card.
*/
void
SoGtkExaminerViewer::openViewerHelpCard(void)
{
  this->openHelpCard("SoGtkExaminerViewer.help");
} // openViewerHelpCard()

// *************************************************************************

/*!
*/

SbBool
SoGtkExaminerViewer::processSoEvent( // virtual
  const SoEvent * const event )
{
  if ( common->processSoEvent(event) )
    return TRUE;
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
  if ( common->isFeedbackVisible() )
    common->drawAxisCross();
  if ( common->isAnimating() )
    this->scheduleRedraw();
} // actualRedraw()

// *************************************************************************

/*!
  \internal

  Set the viewer mode based on the given mouse and keyboard state setting.
*/

void
SoGtkExaminerViewer::setModeFromState(
  const unsigned int state )
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
SoGtkExaminerViewer::setMode(
  const ViewerMode mode )
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
} // setMode()

// *************************************************************************

/*!
  \internal

  Set cursor graphics according to mode.
*/

void
SoGtkExaminerViewer::setCursorRepresentation(
  const ViewerMode mode )
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
SoGtkExaminerViewer::setEnableFeedbackControls(
  const SbBool flag )
{
//  this->feedbackLabel1->setEnabled(flag);
//  this->feedbackLabel2->setEnabled(flag);
//  this->feedbackWheel->setEnabled(flag);
//  this->feedbackEdit->setEnabled(flag);
} // setEnableFeedbackControls()

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

/*!
*/

void
SoGtkExaminerViewer::camerabuttonClicked(
  void )
{
  SOGTK_STUB();
} // camerabuttonClicked()

/*!
*/

void
SoGtkExaminerViewer::camerabuttonClickedCB(
  GtkWidget *,
  gpointer closure )
{
  assert( closure != NULL );
  SoGtkExaminerViewer * viewer = (SoGtkExaminerViewer *) closure;
  viewer->camerabuttonClicked();
} // camerabuttonClickedCB()

// *************************************************************************

/*!
*/

void
SoGtkExaminerViewer::afterRealizeHook( // virtual
  void )
{
} // afterRealizeHook()

/*!
*/

void
SoGtkExaminerViewer::createPrefSheet( // virtual
  void )
{
} // createPrefSheet()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkExaminerViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

