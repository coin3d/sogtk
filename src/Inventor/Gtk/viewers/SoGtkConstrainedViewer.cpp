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

#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/viewers/SoGtkConstrainedViewer.h>
#include <Inventor/Gtk/viewers/SoAnyConstrainedViewer.h>

// ************************************************************************

/*!
  \class SoGtkConstrainedViewer Inventor/Gtk/viewers/SoGtkConstrainedViewer.h
  \brief The SoGtkConstrainedViewer class is yet to be documented...
  \ingroup components viewers

  - abstract component class
  - focus on camera constraint
*/

// ************************************************************************

/*!
  \var SbVec3f SoGtkConstrainedViewer::upVector
  FIXME: write doc
*/

// ************************************************************************

class SoGtkConstrainedViewerP {
public:
  SoGtkConstrainedViewerP(SoGtkConstrainedViewer * publ);
  ~SoGtkConstrainedViewerP(void);

private:
  SoGtkConstrainedViewer * pub;

}; // class SoGtkConstrainedViewerP

// ************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkConstrainedViewer);

// ************************************************************************

/*!
  Constructor.
*/

SoGtkConstrainedViewer::SoGtkConstrainedViewer(// protected
  GtkWidget * parent,
  const char * const name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag flag, 
  SoGtkViewer::Type type, 
  SbBool build)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->pimpl = new SoGtkConstrainedViewerP(this);
  this->common = new SoAnyConstrainedViewer(this);

  this->setClassName("SoGtkConstrainedViewer");
  this->upVector = SbVec3f(0.0f, 1.0f, 0.0f);

  if (! build) return;
  GtkWidget * viewer = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(viewer);
} // SoGtkConstainedViewer()

/*!
  The destructor.
*/

SoGtkConstrainedViewer::~SoGtkConstrainedViewer(// virtual
  void)
{
  delete this->common;
  delete this->pimpl;
} // ~SoGtkConstarinedViewer()
  
// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::setUpDirection(
  const SbVec3f & upDirection)
{
  this->upVector = upDirection;
} // setUpDirection()

// ************************************************************************

/*!
  FIXME: write doc
*/

const SbVec3f &
SoGtkConstrainedViewer::getUpDirection(
  void)
{
  return upVector;
} // getUpDirection()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::setCamera(// virtual
  SoCamera * newCamera)
{
  inherited::setCamera(newCamera);
} // setCamera()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::saveHomePosition(// virtual
  void)
{
} // saveHomePosition()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::resetToHomePosition(// virtual
  void)
{
} // resetToHomePosition()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::recomputeSceneSize(// virtual
  void)
{
  SoDebugError::postInfo("SoGtkConstrainedViewer::recomputeSceneSize",
    "stub called");
} // recomputeSceneGraph()

// ************************************************************************

/*!
  aka upperLeftWheelMotion() ;)
*/

void
SoGtkConstrainedViewer::tiltCamera(// virtual, protected
  float delta)
{
} // tiltCamera()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::bottomWheelMotion(// virtual, protected
  float value)
{
} // bottomWheelMotion()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::leftWheelMotion(// virtual, protected
  float value)
{
//  SoAnyConstrainedViewer::elevation();
} // leftWheelMotion()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::changeCameraValues(// virtual, protected
  SoCamera * camera)
{
  SoDebugError::postInfo("SoGtkConstrainedViewer::changeCameraValues",
    "stub called");
} // changeCameraValues()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::findUpDirection(
  SbVec2s mouseLocation)
{
  SoDebugError::postInfo("SoGtkConstrainedViewer::findUpDirection",
    "stub called");
  // pick on point, set upVector to normal of picked polygon
} // findUpDirection()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::checkForCameraUpConstrain(
  void)
{
  SoDebugError::postInfo("SoGtkConstrainedViewer::checkForCameraUpConstrain",
    "stub called");
} // checkForCameraUpConstrain()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkConstrainedViewer::computeSeekFinalOrientation(// virtual, protected
  void)
{
  SoDebugError::postInfo("SoGtkConstrainedViewer::computeSeekFinalOrientation",
    "stub called");
} // computeSeekFinalOrientation()

// ************************************************************************
//
//  Private implementation
//

/*
*/

SoGtkConstrainedViewerP::SoGtkConstrainedViewerP(
  SoGtkConstrainedViewer * publ)
{
  this->pub = publ;
} // SoGtkConstrainedViewerP()

/*
*/

SoGtkConstrainedViewerP::~SoGtkConstrainedViewerP(
  void)
{
} // ~SoGtkConstrainedViewerP()

// ************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkConstrainedViewerRCSId(void) { return rcsid; }
#endif

