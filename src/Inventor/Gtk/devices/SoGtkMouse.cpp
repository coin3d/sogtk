/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

/*!
  \class SoGtkMouse SoGtkMouse.h Inventor/Gtk/devices/SoGtkMouse.h
  \brief The SoGtkMouse class ...
  \ingroup gtkdevices

  FIXME: write class doc
*/

#include <assert.h>

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
  \enum SoGtkMouse::SoGtkMouseEventMask
  FIXME: write documentation for enum
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::ButtonPressMask
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::ButtonReleaseMask
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::PointerMotionMask
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::ButtonMotionMask
  FIXME: write documentation for enum definition
*/
/*!
  \var SoGtkMouse::SoGtkMouseEventMask SoGtkMouse::SO_GTK_ALL_MOUSE_EVENTS
  FIXME: write documentation for enum definition
*/

// *************************************************************************

/*!
  Constructor.
*/

SoGtkMouse::SoGtkMouse(
  SoGtkMouseEventMask mask )
{
  this->eventmask = mask;

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
  SoEvent * super = NULL;
  switch ( ev->type ) {
  case GDK_BUTTON_PRESS:
    do {
      GdkEventButton * event = (GdkEventButton *) ev;
      this->buttonevent->setState( SoButtonEvent::DOWN );
      this->buttonevent->setShiftDown( event->state & GDK_SHIFT_MASK );
      this->buttonevent->setCtrlDown( event->state & GDK_CONTROL_MASK );
      this->buttonevent->setAltDown( event->state & GDK_MOD1_MASK );
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
    super = this->buttonevent;
    break;

  case GDK_BUTTON_RELEASE:
    do {
      GdkEventButton * event = (GdkEventButton *) ev;
      this->buttonevent->setState( SoButtonEvent::UP);
      this->buttonevent->setShiftDown( event->state & GDK_SHIFT_MASK );
      this->buttonevent->setCtrlDown( event->state & GDK_CONTROL_MASK );
      this->buttonevent->setAltDown( event->state & GDK_MOD1_MASK );
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
    super = this->buttonevent;
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
    super = this->locationevent;
    break;

  default:
    return (SoEvent *) NULL;

  } // switch ( ev->type )

  return super;
} // translateEvent()

// *************************************************************************
