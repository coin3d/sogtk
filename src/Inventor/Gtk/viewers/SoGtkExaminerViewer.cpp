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
  \ingroup viewers

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

SOGTK_OBJECT_SOURCE(SoGtkExaminerViewer);

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

//  this->defaultcursor = NULL;
//  this->rotateCursor = NULL;
//  this->panCursor = NULL;
//  this->zoomCursor = NULL;

//  this->pixmaps.orthogonal = new QPixmap((const char **)ortho_xpm);
//  this->pixmaps.perspective = new QPixmap((const char **)perspective_xpm);
//  assert(this->poxmaps.orthogonal->size() == this->pixmaps.perspective->size());

  this->setClassName("SoGtkExaminerViewer");

//  this->addVisibilityChangeCallback( SoGtkExaminerViewer::visibilityCB, this );

  this->setPopupMenuString( _( "Examiner Viewer" ) );
  this->setPrefSheetString( _( "Examiner Viewer Preference Sheet" ) );

  this->setLeftWheelString( _( "Rotx" ) );
  this->setBottomWheelString( _( "Roty" ) );
  this->setRightWheelString( _( "Dolly" ) );

  if ( ! build ) return;
  GtkWidget * parent = this->getParentWidget();
  GtkWidget * viewer = this->buildWidget( parent );
  this->setBaseWidget( viewer );
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
  this->common->setMode( enable ?
                         SoAnyExaminerViewer::EXAMINE :
                         SoAnyExaminerViewer::INTERACT );
  inherited::setViewing( enable );
} // setViewing()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkExaminerViewer::setAnimationEnabled(
  const SbBool enable )
{
  common->setAnimationEnabled( enable );
} // setAnimationEnabled()
 
/*!
  FIXME: write doc
*/

SbBool
SoGtkExaminerViewer::isAnimationEnabled(
  void ) const
{
  return common->isAnimationEnabled();
} // isAnimationEnabled()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkExaminerViewer::stopAnimating(
  void )
{
  common->stopAnimating();
} // stopAnimating()

/*!
  FIXME: write doc
*/

SbBool
SoGtkExaminerViewer::isAnimating(
  void ) const
{
  return common->isAnimating();
} // isAnimating()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkExaminerViewer::setFeedbackVisibility(
  const SbBool enable )
{
  common->setFeedbackVisibility( enable );
} // setFeedbackVisibility()

/*!
  FIXME: write doc
*/

SbBool
SoGtkExaminerViewer::isFeedbackVisible(
  void ) const
{
  return common->isFeedbackVisible();
} // isFeedbackVisible()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkExaminerViewer::setFeedbackSize(
  const int size )
{
  common->setFeedbackSize( size );
} // setFeedbackSize()

/*!
  FIXME: write doc
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
  SoCamera * const camera )
{
  if ( camera ) {
    const SoType camtype( camera->getTypeId() );
    const SbBool orthotype =
      camtype.isDerivedFrom( SoOrthographicCamera::getClassTypeId() );

    this->setRightWheelString( orthotype ? _( "Zoom" ) : _( "Dolly" ));
//    if (this->cameratogglebutton) {
//      this->cameratogglebutton->setPixmap( orthotype ?
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

void
SoGtkExaminerViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
  this->setCursorRepresentation(this->common->currentmode);
} // setCursorEnabled

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
  GtkWidget* form1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (form1);
  gtk_container_add (GTK_CONTAINER (parent), form1);

  GtkWidget *checkbutton1 = gtk_check_button_new_with_label ( 
    _( "Enable spin animation" ) );
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), 
     this->isAnimationEnabled());
  gtk_widget_show (checkbutton1);
  gtk_box_pack_start (GTK_BOX (form1), checkbutton1, FALSE, FALSE, 0);
  gtk_signal_connect( GTK_OBJECT(checkbutton1), "toggled", 
    GTK_SIGNAL_FUNC(SoGtkExaminerViewer::spinAnimationToggled), this );

  // Do the single widget on the second row (a checkbutton).
  GtkWidget *checkbutton2 = gtk_check_button_new_with_label ( 
    _( "Show point of rotation axes" ) );
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton2), 
     this->isFeedbackVisible());
  gtk_widget_show (checkbutton2);
  gtk_box_pack_start (GTK_BOX (form1), checkbutton2, FALSE, FALSE, 0);
  gtk_signal_connect( GTK_OBJECT(checkbutton2), "toggled", 
    GTK_SIGNAL_FUNC(SoGtkExaminerViewer::feedbackVisibilityToggled), this );

  GtkWidget* hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (parent), hbox);

  // Do the four widgets on the third row (label, thumbwheel,
  // lineedit, label).

  this->feedbacklabel1 = gtk_label_new( _( "axes size" ) );
  gtk_box_pack_start (GTK_BOX (hbox), this->feedbacklabel1, FALSE, FALSE, 0);

  this->feedbackwheel = gtk_thumbwheel_new( 0 );
  gtk_box_pack_start (GTK_BOX (hbox), this->feedbackwheel, FALSE, FALSE, 0);
  gtk_thumbwheel_set_range_boundary_handling( 
    GTK_THUMBWHEEL(this->feedbackwheel), GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE );

  gtk_signal_connect( GTK_OBJECT(this->feedbackwheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewer::feedbackWheelPressed), this );
  gtk_signal_connect( GTK_OBJECT(this->feedbackwheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewer::feedbackSizeChanged), this );
  gtk_signal_connect( GTK_OBJECT(this->feedbackwheel), "released",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewer::feedbackWheelReleased), this );

  gtk_thumbwheel_set_value( GTK_THUMBWHEEL(this->feedbackwheel),
    float(common->getFeedbackSize())/10.0f);

  this->feedbackedit = gtk_entry_new();
  gtk_box_pack_start (GTK_BOX (hbox), this->feedbackedit, FALSE, FALSE, 0);
  gtk_widget_set_usize (this->feedbackedit, 48, 24);

  gtk_signal_connect(GTK_OBJECT(this->feedbackedit), "activate",
    GTK_SIGNAL_FUNC(SoGtkExaminerViewer::feedbackEditPressed), this );

  char buffer[16] ;
  sprintf( buffer, "%d", common->getFeedbackSize());
  gtk_entry_set_text( GTK_ENTRY(this->feedbackedit), buffer );

  this->feedbacklabel2 = gtk_label_new( _( "pixels" ) );
  gtk_box_pack_start (GTK_BOX (hbox), this->feedbacklabel2, FALSE, FALSE, 0);

  gtk_widget_show_all(hbox);

  this->setEnableFeedbackControls(common->isFeedbackVisible());

  return form1;
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
  static const char defaultTitle[] = N_( "Examiner Viewer" );
  return _( defaultTitle );
} // getDefaultTitle()

/*!
  Overloaded to provide ``title'' of class.
*/

const char *
SoGtkExaminerViewer::getDefaultIconTitle(void) const
{
  static const char defaultIconTitle[] = N_( "Examiner Viewer" );
  return _( defaultIconTitle );
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
  FIXME: write doc
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
  SbBool on )
{
#if SOGTK_DEBUG
  if ( on == this->isSeekMode() ) {
    SoDebugError::postWarning( "SoGtkExaminerViewer::setSeekMode",
                               "seek mode already %sset", on ? "" : "un");
    return;
  }
#endif // SOGTK_DEBUG
  if ( common->isAnimating() ) common->stopAnimating();
  inherited::setSeekMode( on );
  this->common->setMode(on ? 
                        SoAnyExaminerViewer::WAITING_FOR_SEEK : 
                        SoAnyExaminerViewer::EXAMINE);
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

  Set cursor graphics according to mode.
*/

void
SoGtkExaminerViewer::setCursorRepresentation(
  int mode )
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
  case SoAnyExaminerViewer::INTERACT:
    w->setCursor(arrowCursor);
    break;

  case SoAnyExaminerViewer::EXAMINE:
  case SoAnyExaminerViewer::DRAGGING:
    w->setCursor(* this->rotatecursor);
    break;

  case SoAnyExaminerViewer::ZOOMING:
    w->setCursor(* this->zoomcursor);
    break;

  case SoAnyExaminerViewer::WAITING_FOR_SEEK:
    w->setCursor(crossCursor);
    break;

  case SoAnyExaminerViewer::WAITING_FOR_PAN:
  case SoAnyExaminerViewer::PANNING:
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
  gtk_widget_set_sensitive( this->feedbacklabel1, flag );
  gtk_widget_set_sensitive( this->feedbacklabel2, flag );
  gtk_widget_set_sensitive( this->feedbackwheel, flag );
  gtk_widget_set_sensitive( this->feedbackedit, flag );
} // setEnableFeedbackControls()

// *************************************************************************
/*!
  \internal
  Pref sheet Gtk Signal Handler.
*/
void
SoGtkExaminerViewer::spinAnimationToggled(
  GtkToggleButton     *w,  
  gpointer            closure )
{
  assert( closure != NULL );
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure ;
  SbBool flag = gtk_toggle_button_get_active(w) ? TRUE : FALSE ;

  viewer->common->setAnimationEnabled(flag);
}

// *************************************************************************
/*!
  \internal
  Pref sheet Gtk Signal Handler.
*/
void
SoGtkExaminerViewer::feedbackVisibilityToggled(
  GtkToggleButton     *w,  
  gpointer            closure )
{
  assert( closure != NULL );
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure ;
  SbBool flag = gtk_toggle_button_get_active(w) ? TRUE : FALSE ;

  viewer->common->setFeedbackVisibility(flag);
  viewer->setEnableFeedbackControls(flag);
}

// *************************************************************************
/*!
  \internal
  Pref sheet Gtk Signal Handler.
*/
void
SoGtkExaminerViewer::feedbackEditPressed(
  GtkEntry	*w,
  gpointer	closure )
{
  assert( closure != NULL );
  SoGtkExaminerViewer	*viewer = (SoGtkExaminerViewer*) closure ;

  char *s = gtk_editable_get_chars( GTK_EDITABLE(w), 0, -1 );
  int val;
  if ((sscanf(s, "%d", &val) == 1) && (val > 0)) {
    gtk_thumbwheel_set_value( GTK_THUMBWHEEL(viewer->feedbackwheel), 
      float(val)/10.0f );
    viewer->setFeedbackSize(val);
  }
  g_free(s) ;

  /* else */
  {
    char buffer[16] ;
    sprintf( buffer, "%d", viewer->getFeedbackSize());
    gtk_entry_set_text( GTK_ENTRY(w), buffer );
  }
}

// *************************************************************************
/*!
  \internal
  Pref sheet Gtk Signal Handler.
*/
void
SoGtkExaminerViewer::feedbackWheelPressed(
  GtkWidget	*w,
  gpointer	closure )
{
  assert( closure != NULL );
  GtkThumbWheel *thumbwheel = (GtkThumbWheel *) w ;
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure ;
  viewer->interactiveCountInc();
}

// *************************************************************************

/*!
  \internal
  Pref sheet GtkSignalHandler.
*/

void
SoGtkExaminerViewer::feedbackWheelReleased(
  GtkWidget	*w,
  gpointer	closure )
{
  assert( closure != NULL );
  GtkThumbWheel *thumbwheel = (GtkThumbWheel *) w ;
  SoGtkExaminerViewer *viewer = (SoGtkExaminerViewer *) closure ;
  viewer->interactiveCountDec();
}

// *************************************************************************
/*!
  \internal
  Pref sheet Gtk Signal Handler.
*/
void
SoGtkExaminerViewer::feedbackSizeChanged(
  GtkWidget		*w,
  gpointer		closure )
{
  assert( closure != NULL );
  GtkThumbWheel		*thumbwheel = (GtkThumbWheel*) w ;
  SoGtkExaminerViewer	*viewer = (SoGtkExaminerViewer*) closure ;

  gfloat val = gtk_thumbwheel_get_value( thumbwheel );
  if (val < 0.1f) {
    val = 0.1f;
    gtk_thumbwheel_set_value( thumbwheel, val );
  }

  viewer->setFeedbackSize(int(val * 10.0f));

  char buffer[16] ;
  sprintf( buffer, "%d", viewer->getFeedbackSize() );
  gtk_entry_set_text( GTK_ENTRY(viewer->feedbackedit), buffer );
}

// *************************************************************************
/*!
  \internal
  Pref sheet slot.
*/

void
SoGtkExaminerViewer::cameratoggleClicked()
{
  if ( this->getCamera() ) this->toggleCameraType();
} // cameratoggleClicked()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkExaminerViewer::camerabuttonClicked(
  void )
{
  SOGTK_STUB();
} // camerabuttonClicked()

/*!
  FIXME: write doc
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
  FIXME: write doc
*/

void
SoGtkExaminerViewer::afterRealizeHook( // virtual
  void )
{
  this->setCursorRepresentation( this->common->currentmode );
  inherited::afterRealizeHook();
} // afterRealizeHook()

/*!
  FIXME: write doc
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

