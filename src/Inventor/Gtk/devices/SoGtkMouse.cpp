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

#include <gdk/gdktypes.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkMouse.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// *************************************************************************

/*!
  \class SoGtkMouse SoGtkMouse.h Inventor/Gtk/devices/SoGtkMouse.h
  \brief The SoGtkMouse class ...
  \ingroup sogtkdevices

  FIXME: write class doc
*/

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkMouse);

// *************************************************************************

/*!
  \enum SoGtkMouse::SoGtkMouseEventMask
  FIXME: write documentation for enum
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::BUTTON_PRESS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::BUTTON_RELEASE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::POINTER_MOTION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::BUTTON_MOTION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::ALL_EVENTS
  FIXME: write documentation for enum definition
*/

// *************************************************************************

/*!
  Constructor.
*/

SoGtkMouse::SoGtkMouse(
  const Events events )
{
  this->events = events;
  this->buttonevent = new SoMouseButtonEvent;
  this->locationevent = new SoLocation2Event;
} // SoGtkMouse()

/*!
  Destructor.
*/

SoGtkMouse::~SoGtkMouse(
  void )
{
  delete this->buttonevent;
  delete this->locationevent;
} // ~SoGtkMouse()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

void
SoGtkMouse::enable(
  GtkWidget *, // w,
  SoGtkEventHandler, // f,
  void * ) // data )
{
  // TODO: implement
} // enable()

/*!
  FIXME: write function documentation
*/

void
SoGtkMouse::disable(
  GtkWidget *, // w,
  SoGtkEventHandler, // f,
  void * ) // data )
{
  // TODO: implement
} // disable()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SoEvent *
SoGtkMouse::translateEvent(
  GdkEvent * ev )
{
  switch ( ev->type ) {
  case GDK_BUTTON_PRESS:
    do {
      GdkEventButton * event = (GdkEventButton *) ev;
      this->buttonevent->setState( SoButtonEvent::DOWN );
      this->buttonevent->setShiftDown( (event->state & GDK_SHIFT_MASK) ? TRUE : FALSE );
      this->buttonevent->setCtrlDown( (event->state & GDK_CONTROL_MASK) ? TRUE : FALSE );
      this->buttonevent->setAltDown( (event->state & GDK_MOD1_MASK) ? TRUE : FALSE );
      SbTime stamp;
      stamp.setMsecValue( event->time );
      this->buttonevent->setTime( stamp );
      SoGtkDevice::setEventPosition( this->buttonevent,
        (int) event->x, (int) event->y );
      switch ( event->button ) {
      case 1:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON1 );
        break;
      case 2:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON3 );
        break;
      case 3:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON2 );
        break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTONS
      case 4:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON4 );
        break;
      case 5:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON5 );
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTONS
      default:
        this->buttonevent->setButton( SoMouseButtonEvent::ANY );
        break;
      }
    } while ( FALSE );
    return this->buttonevent;
    break;

  case GDK_BUTTON_RELEASE:
    do {
      GdkEventButton * event = (GdkEventButton *) ev;
      this->buttonevent->setState( SoButtonEvent::UP );
      this->buttonevent->setShiftDown( (event->state & GDK_SHIFT_MASK) ? TRUE : FALSE );
      this->buttonevent->setCtrlDown( (event->state & GDK_CONTROL_MASK) ? TRUE : FALSE );
      this->buttonevent->setAltDown( (event->state & GDK_MOD1_MASK) ? TRUE : FALSE );
      SbTime stamp;
      stamp.setMsecValue( event->time );
      this->buttonevent->setTime( stamp );
      SoGtkDevice::setEventPosition( this->buttonevent,
        (int) event->x, (int) event->y );
      switch ( event->button ) {
      case 1:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON1 );
        break;
      case 2:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON3 );
        break;
      case 3:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON2 );
        break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTONS
      case 4:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON4 );
        break;
      case 5:
        this->buttonevent->setButton( SoMouseButtonEvent::BUTTON5 );
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTONS
      default:
        this->buttonevent->setButton( SoMouseButtonEvent::ANY );
        break;
      }
    } while ( FALSE );
    return this->buttonevent;
    break;

  case GDK_MOTION_NOTIFY:
    do {
      GdkEventMotion * event = (GdkEventMotion *) ev;
      this->locationevent->setShiftDown( event->state & GDK_SHIFT_MASK );
      this->locationevent->setCtrlDown( event->state & GDK_CONTROL_MASK );
      this->locationevent->setAltDown( event->state & GDK_MOD1_MASK );
      SbTime stamp;
      stamp.setMsecValue( event->time );
      this->locationevent->setTime( stamp );
      SoGtkDevice::setEventPosition( this->locationevent,
        (int) event->x, (int) event->y );
    } while ( 0 );
    return this->locationevent;
    break;

  default:
    return (SoEvent *) NULL;

  } // switch ( ev->type )

  return (const SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkMouseRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

