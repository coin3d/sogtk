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

static const char rcsid[] =
  "$Id$";

#include <assert.h>

#include <GL/gl.h>

#include <gtkgl/gtkglarea.h>

#include <Inventor/SoDB.h>
#include <Inventor/SbBasic.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoCallbackList.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/SoPickedPoint.h>

#include <Inventor/Gtk/viewers/SoGtkViewer.h>

// *************************************************************************

/*!
  \class SoGtkViewer SoGtkViewer.h Inventor/Gtk/viewers/SoGtkViewer.h
  \brief The SoGtkViewer class is the top level base viewer class.
  \ingroup qtviewers

  TODO: more doc
  ...overview of what this class provides over parent class...
  ...keyboard combinations: Home, s, arrow keys (but only during
  viewing), Esc...
  ...explain pan, rotate, zoom, dolly, seek...
  ..explain the fact that we're moving the camera, not the scene...
  ...viewer modes (examine vs interact) w/ explanation of what they are
  useful for...

  \sa SoGtkFullViewer
*/

// *************************************************************************

/*!
  \enum SoGtkViewer::Type
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkViewer::Type SoGtkViewer::BROWSER
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::Type SoGtkViewer::EDITOR
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoGtkViewer::DecimationStrategy
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkViewer::DecimationStrategy SoGtkViewer::NORMAL
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DecimationStrategy SoGtkViewer::FIXED_NUM_TRIANGLES
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DecimationStrategy SoGtkViewer::FRAMES_PER_SECOND
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DecimationStrategy SoGtkViewer::FIXED_PERCENTAGE
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoGtkViewer::DrawStyle
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_AS_IS
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_HIDDEN_LINE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_NO_TEXTURE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_LOW_COMPLEXITY
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_LINE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_POINT
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_BBOX
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_LOW_RES_LINE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_LOW_RES_POINT
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::VIEW_SAME_AS_STILL
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawStyle SoGtkViewer::ALL_STYLES_COMBINED
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoGtkViewer::DrawType
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkViewer::DrawType SoGtkViewer::STILL
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::DrawType SoGtkViewer::INTERACTIVE
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoGtkViewer::BufferType
  FIXME: write documentation for enum
*/

/*!
  \var SoGtkViewer::BufferType SoGtkViewer::BUFFER_SINGLE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::BufferType SoGtkViewer::BUFFER_DOUBLE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoGtkViewer::BufferType SoGtkViewer::BUFFER_INTERACTIVE
  FIXME: write documentation for enum definition
*/

// *************************************************************************

/*!
  \internal

  Return the parent node in the scene graph of the given \a node.
  NB: this is just a quick'n'dirty thing for often executed code,
  and doesn't cover cases where nodes have multiple parents.
*/

static inline SoGroup *
getParentOfNode(
  SoNode * root,
  SoNode * node )
{
  SoSearchAction search;
  assert( node != NULL );
  search.setNode(node);
  assert( root != NULL );
  search.apply(root);
  assert( search.getPath() != NULL );
  return (SoGroup *) ((SoFullPath *)search.getPath())->getNodeFromTail( 1 );
} // getParentOfNode()
 
// *************************************************************************

/*!
  Constructor. \a parent, \a name and \a buildInsideParent are passed on
  to SoGtkRenderArea, so see the documentation for that constructor for
  for more information on those.
*/

SoGtkViewer::SoGtkViewer(
  GtkWidget * parent,
  const char * name,
  SbBool buildInsideParent,
  SoGtkViewer::Type type,
  SbBool buildNow )
: inherited( parent, name, buildInsideParent, TRUE, TRUE, FALSE )
{
  this->viewerType = type;
  this->viewMode = TRUE;
  this->cursorOn = TRUE;

  this->cameraType = SoPerspectiveCamera::getClassTypeId();
  this->camera = NULL;
  this->deleteCamera = FALSE;
  this->bufferType = this->isDoubleBuffer() ? BUFFER_DOUBLE : BUFFER_SINGLE;

  this->interactionStartCallbacks = new SoCallbackList;
  this->interactionEndCallbacks = new SoCallbackList;
  this->interactionNesting = 0;

  this->seekDistance = 50.0f;
  this->seekDistanceAbs = TRUE;
  this->seektopoint = TRUE;
  this->seekPeriod = 2.0f;
  this->inSeekMode = FALSE;
  this->seekSensor = new SoTimerSensor( SoGtkViewer::seekSensorCB, this );

  this->userRoot = NULL;

  this->viewerRoot = new SoSeparator;
  this->viewerRoot->ref();
  this->viewerRoot->renderCaching.setValue( SoSeparator::OFF );
  this->viewerRoot->renderCulling.setValue( SoSeparator::OFF );

  // Drawstyle subgraph.
  {
    this->drawStyleRoot = new SoSwitch;

    this->lightModel = new SoLightModel;
    this->lightModel->setOverride(TRUE);
    this->lightModel->model = SoLightModel::BASE_COLOR;

    this->drawStyle = new SoDrawStyle;
    this->drawStyle->setOverride(TRUE);
    this->drawStyle->pointSize.setIgnored(TRUE);
    this->drawStyle->lineWidth.setIgnored(TRUE);
    this->drawStyle->linePattern.setIgnored(TRUE);

    this->complexity = new SoComplexity;
    this->complexity->setOverride(TRUE);
    this->complexity->textureQuality = 0.0f;
    this->complexity->value = 0.1f;

    this->drawStyleRoot->addChild(this->lightModel);
    this->drawStyleRoot->addChild(this->drawStyle);
    this->drawStyleRoot->addChild(this->complexity);
    this->viewerRoot->addChild(this->drawStyleRoot);
    this->drawStyleRoot->whichChild = SO_SWITCH_NONE;
  }

  // Hidden line rendering subgraph.
  {
    this->hiddenLineRoot = new SoSwitch;

    this->baseColor = new SoBaseColor;
    this->baseColor->setOverride(TRUE);
    this->hiddenLineRoot->addChild(this->baseColor);

    this->materialBinding = new SoMaterialBinding;
    this->materialBinding->setOverride(TRUE);
    this->materialBinding->value = SoMaterialBinding::OVERALL;
    this->hiddenLineRoot->addChild(this->materialBinding);

    this->polygonOffsetParent = new SoSwitch;
    this->polygonOffsetParent->whichChild = SO_SWITCH_NONE;
    this->polygonOffset = new SoPolygonOffset;
    this->polygonOffsetParent->addChild(this->polygonOffset);
    this->hiddenLineRoot->addChild(this->polygonOffsetParent);

    this->drawStyleRoot->addChild(this->hiddenLineRoot);
    this->hiddenLineRoot->whichChild = SO_SWITCH_NONE;
  }

  this->drawStyles[STILL] = VIEW_AS_IS;
  this->drawStyles[INTERACTIVE] = VIEW_SAME_AS_STILL;

  this->lightOn = TRUE;
  this->lightRoot = NULL;

  this->addStartCallback(SoGtkViewer::interactiveStartCB);
  this->addFinishCallback(SoGtkViewer::interactiveEndCB);

  this->adjustClipPlanes = TRUE;
  this->autoClipBox = NULL;

  if ( buildNow )
    this->setBaseWidget( this->buildWidget( this->getParentWidget() ) );
} // SoGtkViewer()

// *************************************************************************

/*!
  Destructor.
*/

SoGtkViewer::~SoGtkViewer( // virtual
  void )
{
  delete this->autoClipBox;

  delete this->interactionStartCallbacks;
  delete this->interactionEndCallbacks;

  delete this->seekSensor;

  if (this->userRoot) this->setSceneGraph(NULL);
  if (this->lightRoot) this->lightRoot->unref();
  this->viewerRoot->unref();
} // ~SoGtkViewer()

// *************************************************************************

/*!
  Set the camera we want to view the scene with. This will
  set it up with the headlight and register it for later retrieval
  (from getCamera()) and subsequent use.

  \sa getCamera()
*/

void
SoGtkViewer::setCamera( // virtual
  SoCamera * cam )
{
  // Store for re-enabling after camera is attached.
  SbBool headlightflag = this->isHeadlight();

  if (this->camera) {
    // Detach headlight.
    if (this->isHeadlight()) this->setHeadlight(FALSE);

    // If we made the camera, detach it. Otherwise just leave it in
    // the graph.
    if (this->deleteCamera) {
      SoGroup * cameraparent = getParentOfNode(this->viewerRoot, this->camera);
      cameraparent->removeChild(this->camera);
      this->deleteCamera = FALSE;
    }

    this->camera->unref();
  }

  this->camera = cam;

  if (this->camera) {
    this->camera->ref();
    this->saveHomePosition();
    if (headlightflag) this->setHeadlight(TRUE);
  }
} // setCamera()

// *************************************************************************

/*!
  Returns the camera currently used by the viewer for rendering
  the scene.

  \sa setCamera()
*/

SoCamera *
SoGtkViewer::getCamera(
  void ) const
{
  return this->camera;
} // getCamera()

// *************************************************************************

/*!
  When the viewer has to make its own camera as a result of the graph
  passed to setSceneGraph() not containing any camera nodes, this call
  can be made in advance to decide which type the camera will be of.

  Default is to use an SoPerspectiveCamera.

  \sa getCameraType()
*/

void
SoGtkViewer::setCameraType( // virtual
  SoType t )
{
#if SOGTK_DEBUG
  SbBool valid = TRUE;
  if (t == SoType::badType()) valid = FALSE;
  if (valid) {
    valid = FALSE;
    if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) valid = TRUE;
    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) valid = TRUE;
  }

  if (!valid) {
    SoDebugError::post("SoGtkViewer::setCameraType",
                       "not a valid camera type: '%s'",
                       t == SoType::badType() ?
                       "badType" : t.getName().getString());
    return;
  }
#endif // SOGTK_DEBUG

  this->cameraType = t;
} // setCameraType()

// *************************************************************************

/*!
  Returns camera type which will be used when the viewer has to make its
  own camera.

  Note that this call does \e not return the current cameratype, as one
  might expect. Use getCamera() and SoType::getTypeId() for that inquiry.

  \sa setCameraType()
*/

SoType
SoGtkViewer::getCameraType(
  void ) const
{
  return this->cameraType;
} // getCameraType()

// *************************************************************************

/*!
  Reposition the current camera so we can see the complete scene.
*/

void
SoGtkViewer::viewAll( // virtual
  void )
{
  assert(this->camera);
  this->camera->viewAll(this->userRoot, this->getViewportRegion());
} // viewAll()

// *************************************************************************

/*!
  Store the current camera settings for later retrieval with
  resetToHomePosition().

  \sa resetToHomePosition()
*/

void
SoGtkViewer::saveHomePosition( // virtual
  void )
{
  assert(this->camera);

  this->storedOrientation = this->camera->orientation.getValue();
  this->storedPosition = this->camera->position.getValue();

  SoType t = this->camera->getTypeId();
  if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {
    this->storedHeightVal =
      ((SoOrthographicCamera *)this->camera)->height.getValue();
  }
  else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) {
    this->storedHeightVal =
      ((SoPerspectiveCamera *)this->camera)->heightAngle.getValue();
  }
} // saveHomePosition()

// *************************************************************************

/*!
  Restore the saved camera settings.

  \sa saveHomePosition()
*/

void
SoGtkViewer::resetToHomePosition( // virtual
  void )
{
  assert( this->camera != NULL );

  this->camera->orientation = this->storedOrientation;
  this->camera->position = this->storedPosition;

  SoType type = this->camera->getTypeId();
  if ( type.isDerivedFrom( SoOrthographicCamera::getClassTypeId() ) )
    ((SoOrthographicCamera *)this->camera)->height = this->storedHeightVal;
  else if ( type.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) )
    ((SoPerspectiveCamera *)this->camera)->heightAngle = this->storedHeightVal;

  this->setClippingPlanes();
  this->camera->focalDistance =
    (this->camera->farDistance.getValue() +
     this->camera->nearDistance.getValue())/2.0;
} // resetToHomePosition()

// *************************************************************************

/*!
  Turn the camera headlight on or off.

  Default is to have a headlight turned on.

  \sa isHeadlight(), getHeadlight()
*/

void
SoGtkViewer::setHeadlight( // virtual
  SbBool on )
{
#if SOGTK_DEBUG
  if (this->lightRoot &&
      ((on && this->isHeadlight()) || (!on && !this->isHeadlight()))) {
    SoDebugError::postWarning("SoGtkViewer::setHeadlight",
                              "headlight already turned %s",
                              on ? "on" : "off");
    return;
  }
#endif // SOGTK_DEBUG

  SoSearchAction search;

  if (on) {
    if (!this->lightRoot) {
      this->lightRoot = new SoGroup;
      this->lightRoot->ref();
      this->lightRoot->addChild(new SoRotation);
      SoDirectionalLight * dl = new SoDirectionalLight;
      dl->direction.setValue(1, -1, -10);
      this->lightRoot->addChild(dl);
      this->lightRoot->addChild(new SoResetTransform);
    }

    assert(this->camera);
    SoGroup * cameraparent = getParentOfNode(this->viewerRoot, this->camera);
    int idx = cameraparent->findChild(this->camera);
    cameraparent->insertChild(this->lightRoot, idx+1);

    SoRotation * lorient = (SoRotation *) this->lightRoot->getChild(0);
    lorient->rotation.connectFrom(& this->camera->orientation);
  } else {
    assert(this->lightRoot);
    SoGroup * lightparent = getParentOfNode(this->viewerRoot, this->lightRoot);
    lightparent->removeChild(this->lightRoot);

    SoRotation * lorient = (SoRotation *) this->lightRoot->getChild(0);
    lorient->rotation.disconnect();
  }

  this->lightOn = on;
} // setHeadlight()

// *************************************************************************

/*!
  Returns status of the viewer headlight, whether it is on or off.

  Note that the returned flag does not check the state of the \a on
  field of the SoDirectionalLight headlight node, it only indicates
  whether or not a headlight is part of the scene graph.

  \sa setHeadlight(), getHeadlight()
*/

SbBool
SoGtkViewer::isHeadlight(
  void ) const
{
  return this->lightOn;
} // isHeadlight()

// *************************************************************************

/*!
  Returns the a pointer to the directional light node which is the
  viewer headlight.

  The fields of the node is available for user editing.

  \sa isHeadlight(), setHeadlight()
*/

SoDirectionalLight *
SoGtkViewer::getHeadlight(
  void ) const
{
  if (!this->lightRoot) return NULL;

  SoDirectionalLight * dl =
    (SoDirectionalLight *) this->lightRoot->getChild(1);
  assert(dl->isOfType(SoDirectionalLight::getClassTypeId()));
  return dl;
} // getHeadlight()

// *************************************************************************

/*!
  Set up a drawing style. The \a type argument specifies if the given
  \a style should be interpreted as the drawstyle during animation or
  when the camera is static.

  Default values for the drawing style is to render the scene "as is"
  in both still mode and while the camera is moving.

  See the documentation for the \a DrawType and \a DrawStyle for more
  information.

  \sa getDrawStyle()
*/

void
SoGtkViewer::setDrawStyle( // virtual
  SoGtkViewer::DrawType type,
  SoGtkViewer::DrawStyle style )
{
#if SOGTK_DEBUG
  if ((type != STILL) && (type != INTERACTIVE)) {
    SoDebugError::postWarning("SoGtkViewer::setDrawStyle",
                              "unknown drawstyle type setting 0x%x", type);
    return;
  }
  int s = style & ALL_STYLES_COMBINED;
  int bits = 0;
  while (s) { if (s & 0x0001) bits++; s >>= 1; }
  if (bits != 1) {
    SoDebugError::postWarning("SoGtkViewer::setDrawStyle",
                              "unknown drawstyle setting 0x04%x", style);
    return;
  }
#endif // SOGTK_DEBUG

  if (style == this->getDrawStyle(type)) {
#if 0 // SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkViewer::setDrawStyle",
                              "drawstyle for type 0x%02x already 0x%02x",
                              type, style);
#endif // SOGTK_DEBUG
    return;
  }

  this->drawStyles[type] = style;
  this->changeDrawStyle(this->currentDrawStyle());
} // setDrawStyle()

// *************************************************************************

/*!
  Return current drawstyles for the given type (\a STILL or
  \a INTERACTIVE).

  \sa setDrawStyle()
*/

SoGtkViewer::DrawStyle
SoGtkViewer::getDrawStyle(
  const SoGtkViewer::DrawType type ) const
{
#if SOGTK_DEBUG
  if ((type != STILL) && (type != INTERACTIVE)) {
    SoDebugError::postWarning("SoGtkViewer::setDrawStyle",
                              "unknown drawstyle type setting 0x%x", type);
    return this->drawStyles[STILL];
  }
#endif // SOGTK_DEBUG
  return this->drawStyles[type];
} // getDrawStyle()

// *************************************************************************

/*!
  Set the viewer's buffer type.

  Default is to use a double buffering scheme.

  \sa getBufferingType()
*/

void
SoGtkViewer::setBufferingType( // virtual
  SoGtkViewer::BufferType type )
{
#if 0 // SOGTK_DEBUG
  if (type == this->bufferType) {
    SoDebugError::postWarning("SoGtkViewer::setBufferingType",
                              "buffer type 0x%x already set", type);
    return;
  }

  if (type != BUFFER_SINGLE &&
      type != BUFFER_DOUBLE &&
      type != BUFFER_INTERACTIVE) {
    SoDebugError::postWarning("SoGtkViewer::setBufferingType",
                              "unknown buffer type 0x%x", type);
    return;
  }
#endif // SOGTK_DEBUG

  this->bufferType = type;
  inherited::setDoubleBuffer(type == BUFFER_DOUBLE ? TRUE : FALSE);
} // setBufferingType()

// *************************************************************************

/*!
  Return the viewer's buffer type.

  \sa setBufferingType()
*/

SoGtkViewer::BufferType
SoGtkViewer::getBufferingType(
  void ) const
{
  return this->bufferType;
} // getBufferingType()

// *************************************************************************

/*!
  Set view mode.

  If the view mode is on, user events will be caught and used to
  influence the camera position/orientation. If view mode is off,
  all events in the viewer canvas will be passed along to the scene
  graph.

  Default is to have the view mode active.

  \sa getViewing()
*/

void
SoGtkViewer::setViewing( // virtual
  SbBool on )
{
#if SOGTK_DEBUG
  if (this->viewMode == on) {
    SoDebugError::postWarning("SoGtkViewer::setViewing", "unnecessary called");
    return;
  }
#endif // SOGTK_DEBUG

  // Turn off the selection indicators when we go back from picking
  // mode into viewing mode.
  if (on) {
    SoGLRenderAction * action = this->getGLRenderAction();
    if (action) SoLocateHighlight::turnOffCurrentHighlight(action);
  }

  this->viewMode = on;
} // setViewing()

// *************************************************************************

/*!
  Return state of view mode.

  \sa setViewing()
*/

SbBool
SoGtkViewer::isViewing(
  void ) const
{
  return this->viewMode;
} // isViewing()

// *************************************************************************

/*!
  Set whether or not the mouse cursor representation should be visible
  in the viewer canvas.

  Default value is on.

  \sa getCursorEnabled()
*/

void
SoGtkViewer::setCursorEnabled( // virtual
  SbBool on )
{
  this->cursorOn = on;
} // setCursorEnabled()

// *************************************************************************

/*!
  Returns visibility status of mouse cursor.

  \sa setCursorEnabled()
*/

SbBool
SoGtkViewer::isCursorEnabled(
  void ) const
{
  return this->cursorOn;
} // isCursorEnabled()

// *************************************************************************

/*!
  Turn on or off continuous automatic adjustments of the near
  and far clipping planes.

  Automatic clipping is on as default.

  \sa getAutoClipping()
*/

void
SoGtkViewer::setAutoClipping(
  SbBool on )
{
#if SOGTK_DEBUG
  if (this->adjustClipPlanes == on) {
    SoDebugError::postWarning("SoGtkViewer::setAutoClipping",
                              "unnecessary called");
    return;
  }
#endif // SOGTK_DEBUG
  this->adjustClipPlanes = on;
  if (on) this->scheduleRedraw();
} // setAutoClipping()

// *************************************************************************

/*!
  Return value of the automatic near/far clipplane adjustment indicator.

  \sa setAutoClipping()
*/

SbBool
SoGtkViewer::isAutoClipping(
  void ) const
{
  return this->adjustClipPlanes;
} // isAutoClipping()

// *************************************************************************

/*!
  Turn stereo viewing on. Default is off.

  NB: stereo viewing has not been implemented for this component library
  yet.

  \sa isStereoViewing()
*/

void
SoGtkViewer::setStereoViewing( // virtual
  SbBool /*on*/ )
{
  // FIXME: implement. 990507 mortene.
  SoDebugError::postInfo("SoGtkViewer::setStereoViewing",
                         "stereo viewing not implemented yet");
} // setStereoViewing()

// *************************************************************************

/*!
  Returns a boolean indicating whether or not we're in stereo viewing
  mode.

  \sa setStereoViewing()
*/

SbBool
SoGtkViewer::isStereoViewing( // virtual
  void ) const
{
  // FIXME: implement. 990507 mortene.
  return FALSE;
} // isstereoViewing()

// *************************************************************************

/*!
  Set the offset between the two viewpoints when in stereo mode.

  NB: stereo viewing has not been implemented for this component library
  yet.

  \sa getStereoOffset()
*/

void
SoGtkViewer::setStereoOffset(
  const float /*dist*/)
{
  // FIXME: implement. 990507 mortene.
  SoDebugError::postInfo("SoGtkViewer::setStereoOffset",
                         "stereo viewing not implemented yet");
} // setStereoOffset()

// *************************************************************************

/*!
  Return the offset distance between the two viewpoints when in stereo mode.

  \sa setStereoOffset()
*/

float
SoGtkViewer::getStereoOffset(
  void ) const
{
  // FIXME: implement. 990507 mortene.
  return 0.0f;
} // getStereoOffset()

// *************************************************************************

/*!
  Toggle between seeking to a point or seeking to an object.

  Default is to seek to a point.

  \sa isDetailSeek()
*/

void
SoGtkViewer::setDetailSeek(
  const SbBool on )
{
#if SOGTK_DEBUG
  if (this->seektopoint == on) {
    SoDebugError::postWarning("SoGtkViewer::setDetailSeek",
                              "unnecessary called");
    return;
  }
#endif // SOGTK_DEBUG
  this->seektopoint = on;
} // setDetailSeek()

// *************************************************************************

/*!
  Returns a value indicating whether or not seeks will be performed
  to the exact point of picking or just towards the picked object.

  \sa setDetailSeek()
*/

SbBool
SoGtkViewer::isDetailSeek(
  void ) const
{
  return this->seektopoint;
} // isDetailSeek()

// *************************************************************************

/*!
  Set the duration of animating the camera repositioning
  after a successful seek. Call with \a seconds equal to \a 0.0 to make
  the camera jump immediately to the correct spot.

  Default value is 2 seconds.

  \sa getSeekTime()
*/

void
SoGtkViewer::setSeekTime(
  const float seconds )
{
#if SOGTK_DEBUG
  if (seconds < 0.0f) {
    SoDebugError::postWarning("SoGtkViewer::setSeekTime",
                              "an attempt was made to set a negative seek "
                              "time duration");
    return;
  }
#endif SOGTK_DEBUG
  this->seekPeriod = seconds;
} // setSeekTime()

// *************************************************************************

/*!
  Returns the camera repositioning duration following a seek action.

  \sa setSeekTime()
*/

float
SoGtkViewer::getSeekTime(
  void ) const
{
  return this->seekPeriod;
} // getSeekTime()

// *************************************************************************

/*!
  Add a function to call when user interaction with the scene starts.

  \sa removeStartCallback(), addFinishCallback()
*/

void
SoGtkViewer::addStartCallback(
  SoGtkViewerCB * func,
  void * data )
{
  this->interactionStartCallbacks->addCallback(
    (SoCallbackListCB *) func, data );
} // addStartCallback()

// *************************************************************************

/*!
  Add a function to call when user interaction with the scene ends.

  \sa removeFinishCallback(), addStartCallback()
*/

void
SoGtkViewer::addFinishCallback(
  SoGtkViewerCB * func,
  void * data )
{
  this->interactionEndCallbacks->addCallback( (SoCallbackListCB *) func, data );
} // addFinishCallback()

// *************************************************************************

/*!
  Remove one of the functions which has been set up to be called when user
  interaction with the scene starts.

  \sa addStartCallback(), removeFinishCallback()
*/
void
SoGtkViewer::removeStartCallback(SoGtkViewerCB * func, void * data)
{
  this->interactionStartCallbacks->removeCallback(
    (SoCallbackListCB *) func, data );
}

/*!
  Remove one of the functions which has been set up to be called when user
  interaction with the scene ends.

  \sa addFinishCallback(), removeStartCallback()
*/

void
SoGtkViewer::removeFinishCallback(
  SoGtkViewerCB * func,
  void * data )
{
  this->interactionEndCallbacks->removeCallback(
    (SoCallbackListCB *) func, data );
} // removeFinishCallback()

// *************************************************************************

/*!
  Copy the current camera to the system clipboard. This makes it
  easy to synchronize camera settings over different applications.

  Note: has not been implemented yet.

  \sa pasteView()
*/

void
SoGtkViewer::copyView(
  const SbTime /*eventTime*/ )
{
  // FIXME: not implemented. Needs SoGtkClipboard implementation
  // first. 990507 mortene.

  SoDebugError::postInfo("SoGtkViewer::copyView",
                         "clipboard support not implemented yet");
} // copyView()

// *************************************************************************

/*!
  Set a new camera from the system clipboard.

  Note: has not been implemented yet.

  \sa copyView()
*/

void
SoGtkViewer::pasteView(
  const SbTime /*eventTime*/ )
{
  // FIXME: not implemented. Needs SoGtkClipboard implementation
  // first. 990507 mortene.

  SoDebugError::postInfo("SoGtkViewer::pasteView",
                         "clipboard support not implemented yet");
} // pasteView()

// *************************************************************************

/*!
  Not used, only included for compatibility reasons.
*/

void
SoGtkViewer::recomputeSceneSize( // virtual
  void )
{
  SoDebugError::postInfo("SoGtkViewer::recomputeSceneSize",
                         "this method is obsoleted, don't use it");
} // recomputeSceneSize()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setDecimationStrategy(
  const SoGtkViewer::DecimationStrategy /*strategy*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setDecimationStrategy",
                         "not implemented yet");
} // setDecimationStrategy()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

SoGtkViewer::DecimationStrategy
SoGtkViewer::getDecimationStrategy(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::getDecimationStrategy",
                         "not implemented yet");
  return NORMAL;
} // getDecimationStrategy()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setGoalNumberOfTriangles(
  const int32_t /*goal*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setGoalNumberOfTriangles",
                         "not implemented yet");
} // setGoalNumberOfTriangles()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

int32_t
SoGtkViewer::getGoalNumberOfTriangles(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::getGoalNumberOfTriangles",
                         "not implemented yet");
  return -1;
} // getGoalNumberOfTriangles()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setGoalFramesPerSecond(
  const float /*goal*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setGoalFramesPerSecond",
                         "not implemented yet");
} // setGoalFramesPerSecond()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

float
SoGtkViewer::getGoalFramesPerSecond(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::getGoalFramesPerSecond",
                         "not implemented yet");
  return 72.0f;
} // getGoalFramesPerSecond()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setFixedPercentage(
  const float /*percent*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setFixedPercentage",
                         "not implemented yet");
} // setFixedPercentage()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

float
SoGtkViewer::getFixedPercentage(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::getFixedPercentage",
                         "not implemented yet");
  return 100.0f;
} // getFixedPercentage()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::enableFullRenderingWhenStill(
  const SbBool /*on*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::enableFullRenderingWhenStill",
                         "not implemented yet");
} // enableFullRenderingWhenStill()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

SbBool
SoGtkViewer::isFullRenderingWhenStill(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::isFullRenderingWhenStill",
                         "not implemented yet");
  return TRUE;
} // isFullRenderingWhenStill()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

SbBool
SoGtkViewer::isStillNow(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::isStillNow",
                         "not implemented yet");
  return this->getInteractiveCount() == 0;
} // isStillNow()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setFramesPerSecondCallback(
  SoGtkViewerFPSCB * /*callback*/,
  void * /*data*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setFramesPerSecondCallback",
                         "not implemented yet");
} // setFramesPerSecondCallback()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setNumSamples(
  const int /*numFrames*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setNumSamples",
                         "not implemented yet");
} // setNumSamples()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

int
SoGtkViewer::getNumSamples(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::getNumSamples",
                         "not implemented yet");
  return 10;
} // getNumSamples()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

void
SoGtkViewer::setDecimationPercentageCallback(
  SoGtkViewerDecimationPercentageCB * /*cb*/,
  void * /*data*/ )
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::setDecimationPercentageCallback",
                         "not implemented yet");
} // setDecimationPercentageCallback()

// *************************************************************************

/*!
  Large Model Visualization; not implemented yet.
*/

float
SoGtkViewer::getCurrentDecimationPercentage(
  void ) const
{
  // FIXME: implement. 990508 mortene.
  SoDebugError::postInfo("SoGtkViewer::getCurrentDecimationPercentage",
                         "not implemented yet");
  return 100.0f;
} // getCurrentDecimationPercentage()

// *************************************************************************

/*!
  Overloaded to update the local bufferingtype variable.

  \sa setBufferingType(), getBufferingType()
*/

void
SoGtkViewer::setDoubleBuffer( // virtual
  SbBool on )
{
  this->bufferType = on ? BUFFER_DOUBLE : BUFFER_SINGLE;
  inherited::setDoubleBuffer(on);
} // setDoubleBuffer()

// *************************************************************************

/*!
  Give the viewer a scenegraph to render and interact with. Overloaded
  from parent class so the viewer can add it's own nodes to control
  rendering in different styles, rendering with a headlight, etc.

  The \a root node will be inserted under the \e viewer's root node,
  which also covers the nodes necessary to implement the different
  preferences drawing style settings.

  If no camera is part of the scene graph under \a root, one will be
  added automatically.

  \sa getSceneGraph(), setCameraType()
*/

void
SoGtkViewer::setSceneGraph( // virtual
  SoNode * root )
{
  if ( ! inherited::getSceneGraph() )
    inherited::setSceneGraph( this->viewerRoot );

  if ( this->userRoot ) {
    if ( this->getCamera() )
      this->setCamera( NULL );
    this->viewerRoot->removeChild( this->userRoot );
  }

  this->userRoot = root;
  if ( ! root )
    return;

  this->viewerRoot->addChild( this->userRoot );

  // Search for a camera already present.
  SoSearchAction search;
  search.setType( SoCamera::getClassTypeId() );
  search.apply( this->userRoot );
  SoCamera * scenecamera = NULL;
  SoFullPath * fullpath = (SoFullPath *) search.getPath();
  if ( fullpath )
    scenecamera = (SoCamera *) fullpath->getTail();

#if 0 // debug
  SoDebugError::postInfo("SoGtkViewer::setSceneGraph",
                         "camera %sfound in graph",
                         scenecamera ? "" : "not ");
#endif // debug

  // Make our own camera if none was available.
  if ( ! scenecamera ) {
    scenecamera = (SoCamera *) this->cameraType.createInstance();
    this->deleteCamera = TRUE;

    if (this->viewerType == SoGtkViewer::BROWSER) {
      this->viewerRoot->insertChild(scenecamera, 1);
    }
    else {
      if (this->userRoot->isOfType(SoGroup::getClassTypeId())) {
        ((SoGroup *)this->userRoot)->insertChild(scenecamera, 0);
      }
      else {
        SoGroup * g = new SoGroup;
        g->addChild(scenecamera);
        g->addChild(this->userRoot);
        this->viewerRoot->removeChild(this->userRoot);
        this->viewerRoot->addChild(g);
        this->userRoot = g;
      }
    }

    scenecamera->viewAll(this->userRoot, this->getViewportRegion());
  }

  // This will set up the headlight
  this->setCamera(scenecamera);
} // setSceneGraph()

// *************************************************************************

/*!
  Overloaded from parent class to return the root of the scene graph
  set by the user, without the extras added by the viewer to control
  rendering.

  \sa setSceneGraph()
*/

SoNode *
SoGtkViewer::getSceneGraph( // virtual
  void )
{
  return this->userRoot;
} // getSceneGraph()

// *************************************************************************

/*!
  Put the viewer in or out of seek mode.

  If the user performs a mouse button click when the viewer is in seek
  mode, the camera will be repositioned so the camera focal point lies
  on the point of the geometry under the mouse cursor.

  \sa isSeekMode(), setDetailSeek()
*/

void
SoGtkViewer::setSeekMode( // virtual
  SbBool on )
{
  assert(this->isViewing());

  if (!on && this->seekSensor->isScheduled()) {
    this->seekSensor->unschedule();
    this->interactiveCountDec();
  }

  this->inSeekMode = on;
} // setSeekMode()

// *************************************************************************

/*!
  Return a flag which indicates whether or not the viewer is in seek mode.

  \sa setSeekMode()
*/

SbBool
SoGtkViewer::isSeekMode(
  void ) const
{
  return this->inSeekMode;
} // isSeekMode()

// *************************************************************************

/*!
  Call this method to initiate a seek action towards the 3D intersection
  of the scene and the ray from the screen coordinate's point and in the same
  direction as the camera is pointing.

  Returns \a TRUE if the ray from the \a screenpos position intersect with
  any parts of the onscreen geometry, otherwise FALSE.
*/

SbBool
SoGtkViewer::seekToPoint(
  const SbVec2s screenpos )
{
  assert(this->camera);

  SoRayPickAction rpaction(this->getViewportRegion());
  rpaction.setPoint(screenpos);
  rpaction.setRadius(2);
  rpaction.apply(this->viewerRoot);

  SoPickedPoint * picked = rpaction.getPickedPoint();
  if (!picked) {
    this->setSeekMode(FALSE);
    return FALSE;
  }

  SbVec3f hitpoint;
  if (this->seektopoint) {
    hitpoint = picked->getPoint();
  } else {
    SoGetBoundingBoxAction bbaction(this->getViewportRegion());
    bbaction.apply(picked->getPath());
    SbBox3f bbox = bbaction.getBoundingBox();
    hitpoint = bbox.getCenter();
  }

  this->cameraStartPosition = this->camera->position.getValue();

  float fd = this->seekDistance;
  if (this->seekDistanceAbs)
    fd *= (hitpoint - this->camera->position.getValue()).length()/100.0f;
  this->camera->focalDistance = fd;

  SbVec3f dir;
  this->camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  this->cameraEndPosition = hitpoint - fd * dir;

  if (!this->seekSensor->isScheduled()) {
    this->seekSensor->setBaseTime(SbTime::getTimeOfDay());
    this->seekSensor->schedule();
    this->interactiveCountInc();
  }

  return TRUE;
} // seekToPoint()

// *************************************************************************

/*!
  Overloaded from parent class to be able to do the necessary two-pass
  rendering if the drawing style is \e hidden \e line.
*/

void
SoGtkViewer::actualRedraw( // virtual
  void )
{
  // Recalculate near/far planes.
  if (this->isAutoClipping()) this->setClippingPlanes();

  if (this->drawAsHiddenLine()) {
    this->drawStyle->style = SoDrawStyle::FILLED;
    this->materialBinding->value.setIgnored(FALSE);
    this->baseColor->rgb.setValue(this->getBackgroundColor());
    this->baseColor->rgb.setIgnored(FALSE);

    this->polygonOffsetParent->whichChild = SO_SWITCH_ALL;

    this->getSceneManager()->render(this->isClearBeforeRender(), TRUE);

    this->drawStyle->style = SoDrawStyle::LINES;
    this->materialBinding->value.setIgnored(TRUE);
    this->baseColor->rgb.setIgnored(TRUE);
    this->polygonOffsetParent->whichChild = SO_SWITCH_NONE;

    this->getSceneManager()->render(FALSE, FALSE);

    return;
  }

  DrawStyle style = this->currentDrawStyle();
  SbBool clearzbuffer =
    (style & (VIEW_LOW_RES_LINE|VIEW_LOW_RES_POINT|VIEW_BBOX)) ? FALSE : TRUE;
  this->getSceneManager()->render(isClearBeforeRender(), clearzbuffer);
} // actualRedraw()

// *************************************************************************

/*!
  This method should be called from subclasses to handle events
  with common interpretation for all viewer classes.
*/

SbBool
SoGtkViewer::processCommonEvents(
  GdkEvent * e )
{
  // Check if the application wants to "steal" the event.
  if ( inherited::invokeAppCB(e) )
    return TRUE;

/*
  // Hit Escape to toggle between interact and examine viewer modes.
  if ( e->type() == Event_KeyPress ) {
    QKeyEvent * ke = (QKeyEvent *)e;
    if (ke->key() == Key_Escape) {
      this->setViewing(this->isViewing() ? FALSE : TRUE);
      return TRUE;
    }
  }

  // If we're in interact mode, don't catch anything else than Esc
  // presses, everything else should be passed on to the scene graph.
  if (!this->isViewing()) {
    inherited::processEvent(e);
    return TRUE;
  }

  if (e->type() == Event_KeyPress) {
    QKeyEvent * ke = (QKeyEvent *)e;
    SbVec2f pos(0.0f, 0.0f);

    switch (ke->key()) {
    case Key_Home:
      this->resetToHomePosition();
      return TRUE;

    case Key_S:
      this->setSeekMode(this->isSeekMode() ? FALSE : TRUE);
      return TRUE;

    case Key_Left:
      pos.setValue(-0.1f, 0.0f);
      break;
    case Key_Down:
      pos.setValue(0.0f, -0.1f);
      break;
    case Key_Right:
      pos.setValue(0.1f, 0.0f);
      break;
    case Key_Up:
      pos.setValue(0.0f, 0.1f);
      break;

    default:
      return FALSE;
    }

    // Arrow keys falls through to this code.
    assert(pos != SbVec2f(0.0f, 0.0f));
    this->moveCameraScreen(pos);
    return TRUE;
  }
*/

  return FALSE;
} // processCommonEvents()

// *************************************************************************

/*!
  To be able to trigger callback functions when user interaction starts
  and/or stops, we need to keep track of the viewer state (i.e. are we in
  still mode or in animation mode?).

  SoGtkViewer automatically adds callbacks to switch between still and
  moving draw style, and to switch between single/double buffer when
  the buffer type is \a INTERACTIVE.

  \sa interactiveCountDec(), getInteractiveCount()
  \sa addStartCallback(), addFinishCallback()
  \sa removeStartCallback(), removeFinishCallback()
  \sa setDrawStyle(), setBufferingType()
*/

void
SoGtkViewer::interactiveCountInc(
  void )
{
  // Catch problems with missing interactiveCountDec() calls.
  assert(this->interactionNesting < 100);

  if (++(this->interactionNesting) == 1)
    this->interactionStartCallbacks->invokeCallbacks(this);

#if 0 // debug
  SoDebugError::postInfo("SoGtkViewer::interactiveCountInc", "%d -> %d",
                         this->interactionNesting - 1,
                         this->interactionNesting);
#endif // debug
} // interactiveCountInc()

// *************************************************************************

/*!
  To be able to trigger callback functions when user interaction starts
  and/or stops, we need to keep track of the viewer state (i.e. are we in
  still mode or in animation mode?).

  SoGtkViewer automatically adds callbacks to switch between still and
  moving draw style, and to switch between single/double buffer when
  the buffer type is \a INTERACTIVE.

  \sa interactiveCountInc(), getInteractiveCount()
  \sa addStartCallback(), addFinishCallback()
  \sa removeStartCallback(), removeFinishCallback()
  \sa setDrawStyle(), setBufferingType()
*/

void
SoGtkViewer::interactiveCountDec(
  void )
{
#if SOGTK_DEBUG
  // Catch problems with missing interactiveCountInc() calls.
  assert(this->interactionNesting > 0);

  if (--(this->interactionNesting) == 0) {
    this->interactionEndCallbacks->invokeCallbacks(this);
  }
#else // no debug
  if (--(this->interactionNesting) <= 0) {
    this->interactionEndCallbacks->invokeCallbacks(this);
    this->interactionNesting = 0;
  }
#endif // nodebug

#if 0 // debug
  SoDebugError::postInfo("SoGtkViewer::interactiveCountDec", "%d -> %d",
                         this->interactionnesting + 1,
                         this->interactionnesting);
#endif // debug
} // interactiveCountDec()

// *************************************************************************

/*!
  Return current interaction count nesting. If equal to zero, the viewer
  is in animation mode, otherwise the camera is still.

  \sa interactiveCountInc(), interactiveCountDec()
*/

int
SoGtkViewer::getInteractiveCount(void) const
{
  return this->interactionNesting;
} // getInteractiveCount()

// *************************************************************************

/*!
  Set the value used for calculating how close the camera and intersection
  hit point should be made at the end of a seek operation.

  The value can be interpreted as an absolute value in the given world
  unit (which typically is meters) or as a percentage value of the
  distance between the camera starting position and the intersection
  hit point. This can be controlled through the
  setSeekValueAsPercentage() method. It is as default used as an
  absolute value.

  Default value is 50 (absolute distance or percent).

  \sa getSeekDistance(), setSeekValueAsPercentage(), setSeekTime()
*/

void
SoGtkViewer::setSeekDistance(
  const float distance )
{
#if SOGTK_DEBUG
  if (distance <= 0.0f) {
    SoDebugError::postWarning("SoGtkViewer::setSeekDistance",
                              "invalid seek distance value: %f",
                              distance);
    return;
  }
#endif // SOGTK_DEBUG
  this->seekDistance = distance;
} // setSeekDistance()

// *************************************************************************

/*!
  Returns the current seek distance. Value given as an absolute scalar
  length or as a percentage value of the original distance between
  the hitpoint and the camera starting position.

  \sa setSeekDistance(), isSeekValueAsPercentage()
*/

float
SoGtkViewer::getSeekDistance(
  void ) const
{
  return this->seekDistance;
} // getSeekDistance()

// *************************************************************************

/*!
  Control whether or not the seek distance value should be interpreted as
  a percentage value or as an absolute distance. See documentation on
  setSeekDistance() for more information.

  \sa setSeekDistance(), isSeekValueAsPercentage()
*/
void
SoGtkViewer::setSeekValueAsPercentage(
  const SbBool on )
{
#if SOGTK_DEBUG
  if ((on && this->isSeekValuePercentage()) ||
      (!on && !this->isSeekValuePercentage())) {
    SoDebugError::postWarning("SoGtkViewer::setSeekDistanceAsPercentage",
                              "unnecessary called, value already %s",
                              on ? "on" : "off");
    return;
  }
#endif // SOGTK_DEBUG
  this->seekDistanceAbs = on ? FALSE : TRUE;
} // getSeekValueAsPercentage()

// *************************************************************************

/*!
  Returns an boolean which indicates if the seek distance value from
  getSeekDistance() should be interpreted as a percentage value or
  as an absolute value.

  \sa setSeekValuePercentage(), getSeekDistance()
*/

SbBool
SoGtkViewer::isSeekValuePercentage(
  void ) const
{
  return this->seekDistanceAbs ? FALSE : TRUE;
} // isSeekValuePercentage()

// *************************************************************************

/*!
  If the current camera is of perspective type, switch to orthographic,
  and vice versa.
*/

void
SoGtkViewer::toggleCameraType( // virtual
  void )
{
  assert(this->camera);

  SoType oldtype = this->camera->getTypeId();
  SoType perspectivetype = SoPerspectiveCamera::getClassTypeId();
  SoType orthotype = SoOrthographicCamera::getClassTypeId();
  SoType newtype = oldtype == perspectivetype ? orthotype : perspectivetype;

  SoCamera * newcamera = (SoCamera *)newtype.createInstance();
  newcamera->aspectRatio.setValue(camera->aspectRatio.getValue());
  newcamera->focalDistance.setValue(camera->focalDistance.getValue());
  newcamera->orientation.setValue(camera->orientation.getValue());
  newcamera->position.setValue(camera->position.getValue());
  newcamera->viewportMapping.setValue(camera->viewportMapping.getValue());

  if (oldtype.isDerivedFrom(perspectivetype)) {
    ((SoOrthographicCamera *)newcamera)->height =
      2 * this->camera->focalDistance.getValue() *
      tan(((SoPerspectiveCamera *)this->camera)->heightAngle.getValue()/2.0f);
  }
  else if (oldtype.isDerivedFrom(orthotype)) {
    ((SoPerspectiveCamera *)newcamera)->heightAngle = 2.0f *
      atan(((SoOrthographicCamera *)this->camera)->height.getValue() / 2.0f /
           this->camera->focalDistance.getValue());
  }
  else {
    assert(0);
  }

  SoGroup * cameraparent = getParentOfNode( this->viewerRoot, this->camera );
  cameraparent->insertChild(newcamera, cameraparent->findChild(this->camera));
  SoCamera * oldcamera = (! this->deleteCamera) ? this->camera : NULL;
  this->setCamera(newcamera);
  this->deleteCamera = TRUE;
  if (oldcamera) cameraparent->removeChild(oldcamera);
} // toggleCameraType()

// *************************************************************************

/*!
  \internal

  Returns a boolean to indicate if the dynamic drawstyle equals
  the static drawstyle.
*/

SbBool
SoGtkViewer::drawInteractiveAsStill(
  void ) const
{
  SbBool moveasstill = this->drawStyles[INTERACTIVE] == VIEW_SAME_AS_STILL;
  if (!moveasstill)
    moveasstill = this->drawStyles[INTERACTIVE] == this->drawStyles[STILL];
  if (!moveasstill)
    moveasstill =
      this->drawStyles[INTERACTIVE] == VIEW_NO_TEXTURE &&
      this->drawStyles[STILL] != VIEW_AS_IS;
  return moveasstill;
} // drawInteractiveAsStill()

// *************************************************************************

/*!
  \internal

  Returns the current drawing style.
*/

SoGtkViewer::DrawStyle
SoGtkViewer::currentDrawStyle(
  void ) const
{
  SbBool interactivemode = this->getInteractiveCount() > 0 ? TRUE : FALSE;

  if ( ! interactivemode || this->drawInteractiveAsStill())
    return this->drawStyles[STILL];
  else
    return this->drawStyles[INTERACTIVE];
} // currentDrawStyle()

// *************************************************************************

/*!
  \internal

  Returns a boolean to indicate if the current drawstyle settings implies
  hidden line rendering.
*/

SbBool
SoGtkViewer::drawAsHiddenLine(
  void ) const
{
  return ((this->currentDrawStyle() == VIEW_HIDDEN_LINE) ? TRUE : FALSE);
} // drawAsHiddenLine()

// *************************************************************************

/*!
  \internal

  Use the given style setting to set the correct states in the
  rendering control nodes. This will affect the way the scene is
  currently rendered.
*/

void
SoGtkViewer::changeDrawStyle(
  SoGtkViewer::DrawStyle style )
{
  // Turn on/off Z-buffering based on the style setting.
  gtk_gl_area_make_current( GTK_GL_AREA(this->getGtkGLArea()) );

  DrawStyle s = this->currentDrawStyle();
  if (s & (VIEW_LOW_RES_LINE|VIEW_LOW_RES_POINT|VIEW_BBOX))
    glDisable(GL_DEPTH_TEST);
  else
    glEnable(GL_DEPTH_TEST);


  this->drawStyleRoot->whichChild = SO_SWITCH_NONE;

  // Render everything as its supposed to be done, don't override
  // any of the settings in the ``real'' graph.
  if (style == VIEW_AS_IS) return;

  this->drawStyleRoot->whichChild = SO_SWITCH_ALL;

  // Further settings for rendering as hidden line will be done in the
  // overloaded actualRedraw().
  if (style == VIEW_HIDDEN_LINE) {
    this->complexity->type.setIgnored(TRUE);
    this->complexity->value.setIgnored(TRUE);
    this->hiddenLineRoot->whichChild = SO_SWITCH_ALL;
    return;
  }

  this->hiddenLineRoot->whichChild = SO_SWITCH_NONE;

  // Set ignore flags.
  SbBool ignored =
    ((VIEW_NO_TEXTURE|VIEW_LOW_COMPLEXITY) & style) ? TRUE : FALSE;
  this->drawStyle->style.setIgnored(ignored);
  this->lightModel->model.setIgnored(ignored);
  ignored = ((VIEW_NO_TEXTURE|VIEW_LOW_COMPLEXITY|VIEW_LINE|VIEW_POINT) &
             style) ? TRUE : FALSE;
  this->complexity->type.setIgnored(ignored);
  ignored = ((VIEW_NO_TEXTURE|VIEW_LINE|VIEW_POINT|VIEW_BBOX) & style) ?
    TRUE : FALSE;
  this->complexity->value.setIgnored(ignored);
  ignored = ((VIEW_NO_TEXTURE|VIEW_LOW_COMPLEXITY|VIEW_LINE|VIEW_LOW_RES_LINE|
              VIEW_POINT|VIEW_LOW_RES_POINT|VIEW_BBOX) & style) ? TRUE : FALSE;

  // Set draw style.
  if (!this->drawStyle->style.isIgnored()) {
    SoDrawStyle::Style ds;
    if (style & (VIEW_LINE|VIEW_LOW_RES_LINE|VIEW_BBOX))
      ds = SoDrawStyle::LINES;
    else if (style & (VIEW_POINT|VIEW_LOW_RES_POINT))
      ds = SoDrawStyle::POINTS;
    else
      assert(0);

    this->drawStyle->style = ds;
  }

  // Set rendering complexity type.
  if (!this->complexity->type.isIgnored()) {
    SoComplexity::Type t;
    if (style & (VIEW_LINE|VIEW_LOW_RES_LINE))
      t = SoComplexity::OBJECT_SPACE;
    else if (style & VIEW_BBOX)
      t = SoComplexity::BOUNDING_BOX;
    else
      assert(0);

    this->complexity->type = t;
  }

#if 0 // debug
  SoDebugError::postInfo("SoGtkViewer::changeDrawStyle",
                         "\n"
                         "\tdrawstyle style: 0x%02x (isIgnored() == %s)\n"
                         "\tlightmodel model: 0x%02x, (isIgnored() == %s)\n"
                         "\tcomplexity type: 0x%02x, (isIgnored() == %s)\n"
                         "\tcomplexity value: %f, (isIgnored() == %s)\n"
                         "",
                         this->drawStyle->style.getValue(),
                         this->drawStyle->style.isIgnored() ? "T" : "F",
                         this->lightModel->model.getValue(),
                         this->lightModel->model.isIgnored() ? "T" : "F",
                         this->complexity->type.getValue(),
                         this->complexity->type.isIgnored() ? "T" : "F",
                         this->complexity->value.getValue(),
                         this->complexity->value.isIgnored() ? "T" : "F");
#endif // debug
} // changeDrawStyle()

// *************************************************************************

/*!
  \internal

  Position the near and far clipping planes just in front of and behind
  the scene's bounding box. This will give us the optimal utilization of
  the z buffer resolution by shrinking it to its minimum depth.
*/

void
SoGtkViewer::setClippingPlanes(
  void )
{
  if (!this->autoClipBox)
    this->autoClipBox = new SoGetBoundingBoxAction(this->getViewportRegion());
  else
    this->autoClipBox->setViewportRegion(this->getViewportRegion());

  this->autoClipBox->apply( this->viewerRoot );

  SbXfBox3f xbox = this->autoClipBox->getXfBoundingBox();
  SbMatrix mat;
  mat.setTranslate(- this->camera->position.getValue());
  xbox.transform(mat);
  mat = this->camera->orientation.getValue().inverse();
  xbox.transform(mat);
  SbBox3f box = xbox.project();

  // Bounding box was calculated in camera space, so we need to "flip"
  // the box (because camera is pointing in the (0,0,-1) direction
  // from origo.
  float nearval = -box.getMax()[2];
  float farval = -box.getMin()[2];

  // This factor should really be made dependent on the underlying
  // hardware or software Z buffer resolution.
  const float SLACK = 1.0f/1000.0f;

  // Scene is completely behind us.
  if (farval < SLACK) return;

  // Disallow negative near clipping plane distance, and make sure the
  // z-buffer depth utilization is kept below a certain threshold.
  nearval = SbMax(SLACK * farval, nearval);

  // Give a bit of slack to avoid artifacts when scene fits exactly
  // inside bounding box.
  this->camera->nearDistance = nearval * (1.0f - SLACK);
  this->camera->farDistance = farval * (1.0f + SLACK);

#if 0 // debug
  SoDebugError::postInfo("SoGtkViewer::setClippingPlanes",
                         "near, far: %f (%f), %f (%f)",
                         nearval, this->camera->nearDistance.getValue(),
                         farval, this->camera->farDistance.getValue());
#endif // debug
} // setClippingPlanes()

// *************************************************************************

/*!
  \internal

  Translate camera a distance equal to the difference in projected,
  normalized screen coordinates given by the argument.
*/

void
SoGtkViewer::moveCameraScreen(
  const SbVec2f & screenpos )
{
  SoCamera * cam = this->getCamera();
  assert(cam);

#if 1 // debug
  SoDebugError::postInfo("SoGtkViewer::moveCameraScreen",
                         "screenpos: <%f, %f>, campos: <%f, %f, %f>",
                         screenpos[0], screenpos[1],
                         cam->position.getValue()[0],
                         cam->position.getValue()[1],
                         cam->position.getValue()[2]);
#endif // debug

  SbViewVolume vv = cam->getViewVolume(this->getGlxAspectRatio());
  SbPlane panplane = vv.getPlane(cam->focalDistance.getValue());

  SbLine line;
  vv.projectPointToLine(screenpos + SbVec2f(0.5, 0.5f), line);
  SbVec3f current_planept;
  panplane.intersect(line, current_planept);
  vv.projectPointToLine(SbVec2f(0.5f, 0.5f), line);
  SbVec3f old_planept;
  panplane.intersect(line, old_planept);

  // Reposition camera according to the vector difference between the
  // projected points.
  cam->position = cam->position.getValue() - (current_planept - old_planept);

#if 1 // debug
  SoDebugError::postInfo("SoGtkViewer::moveCameraScreen",
                         "newcampos: <%f, %f, %f>",
                         cam->position.getValue()[0],
                         cam->position.getValue()[1],
                         cam->position.getValue()[2]);
#endif // debug
} // moveCameraScreen()

// *************************************************************************

/*!
  \internal

  Called when viewer enters interactive mode (animation, drag, ...).
*/

void
SoGtkViewer::interactiveStartCB(
  void *,
  SoGtkViewer * thisp )
{
  if ( thisp->bufferType == BUFFER_INTERACTIVE )
    thisp->inherited::setDoubleBuffer(TRUE);

  if ( ! thisp->drawInteractiveAsStill() )
    thisp->changeDrawStyle( thisp->drawStyles[INTERACTIVE] );
} // interactivestartCB()

// *************************************************************************

/*!
  \internal

  Called when viewer goes out of interactive mode and into
  "frozen" mode.
*/

void
SoGtkViewer::interactiveEndCB(
  void *,
  SoGtkViewer * thisp )
{
  if ( thisp->bufferType == BUFFER_INTERACTIVE )
    thisp->inherited::setDoubleBuffer( FALSE );
  if ( ! thisp->drawInteractiveAsStill() )
    thisp->changeDrawStyle( thisp->drawStyles[STILL] );
} // interactiveendCB()

// *************************************************************************

/*!
  \internal

  Called repeatedly during the seek animation.
*/

// *************************************************************************

void
SoGtkViewer::seekSensorCB(
  void * data,
  SoSensor * s )
{
  SbTime currenttime = SbTime::getTimeOfDay();

  SoGtkViewer * thisp = (SoGtkViewer *)data;
  SoTimerSensor * sensor = (SoTimerSensor *)s;

  float t =
    (currenttime - sensor->getBaseTime()).getValue() / thisp->seekPeriod;
  if ((t > 1.0f) || (t + sensor->getInterval().getValue() > 1.0f)) t = 1.0f;
  SbBool end = (t == 1.0f);

  t = 0.5f - cos(M_PI * t) * 0.5f;
  thisp->camera->position = thisp->cameraStartPosition +
    (thisp->cameraEndPosition - thisp->cameraStartPosition) * t;

  if (end) thisp->setSeekMode(FALSE);
} // seeksensorCB()

// *************************************************************************
