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

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <gdk/gdktypes.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkMouse.h>
#include <Inventor/Gtk/devices/SoGuiMouseP.h>

// *************************************************************************

class SoGtkMouseP : public SoGuiMouseP {
public:
  SoGtkMouseP(SoGtkMouse * p) : SoGuiMouseP(p) { }
};

// *************************************************************************

SoGtkMouse::SoGtkMouse(int eventbits)
{
  PRIVATE(this) = new SoGtkMouseP(this);
  PRIVATE(this)->eventmask = eventbits & SoGtkMouse::ALL_EVENTS;
}

SoGtkMouse::~SoGtkMouse()
{
  delete PRIVATE(this);
}

// *************************************************************************

void
SoGtkMouse::enable(GtkWidget * widget,
                   SoGtkEventHandler * func,
                   gpointer closure)
{
  if (func) {
    if (PRIVATE(this)->eventmask & SoGtkMouse::BUTTON_PRESS) {
      gtk_signal_connect(GTK_OBJECT(widget), "button_press_event",
                         GTK_SIGNAL_FUNC(func), closure);
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_BUTTON_PRESS_MASK);
    }
    if (PRIVATE(this)->eventmask & SoGtkMouse::BUTTON_RELEASE) {
      gtk_signal_connect(GTK_OBJECT(widget), "button_release_event",
                         GTK_SIGNAL_FUNC(func), closure);
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_BUTTON_RELEASE_MASK);
    }
    if (PRIVATE(this)->eventmask & SoGtkMouse::POINTER_MOTION) {
      gtk_signal_connect(GTK_OBJECT(widget), "motion_notify_event",
                         GTK_SIGNAL_FUNC(func), closure);
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_POINTER_MOTION_MASK);
    }
    if (PRIVATE(this)->eventmask & SoGtkMouse::BUTTON_MOTION) {
      if (!(PRIVATE(this)->eventmask & SoGtkMouse::POINTER_MOTION)) {
        gtk_signal_connect(GTK_OBJECT(widget), "motion_notify_event",
                           GTK_SIGNAL_FUNC(func), closure);
        gtk_widget_add_events(GTK_WIDGET(widget),GDK_BUTTON_MOTION_MASK);
      }
    }
  }
}

void
SoGtkMouse::disable(GtkWidget * widget,
                    SoGtkEventHandler * func,
                    gpointer closure)
{
  if (func) {
    gtk_signal_disconnect_by_func(GTK_OBJECT(widget),
                                  GTK_SIGNAL_FUNC(func), closure);
  }
}

// *************************************************************************

const SoEvent *
SoGtkMouse::translateEvent(GdkEvent * ev)
{
  switch (ev->type) {
  case GDK_BUTTON_PRESS:
    do {
      GdkEventButton * event = (GdkEventButton *) ev;
      PRIVATE(this)->buttonevent->setState(SoButtonEvent::DOWN);
      PRIVATE(this)->buttonevent->setShiftDown((event->state & GDK_SHIFT_MASK) ? TRUE : FALSE);
      PRIVATE(this)->buttonevent->setCtrlDown((event->state & GDK_CONTROL_MASK) ? TRUE : FALSE);
      PRIVATE(this)->buttonevent->setAltDown((event->state & GDK_MOD1_MASK) ? TRUE : FALSE);
      SbTime stamp;
      stamp.setMsecValue(event->time);
      PRIVATE(this)->buttonevent->setTime(stamp);
      this->setEventPosition(PRIVATE(this)->buttonevent,
                                    (int) event->x, (int) event->y);
      switch (event->button) {
      case 1:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON1);
        break;
      case 2:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON3);
        break;
      case 3:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON2);
        break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      case 4:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON4);
        break;
      case 5:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON5);
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      default:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::ANY);
        break;
      }
    } while (FALSE);
    return PRIVATE(this)->buttonevent;
    break;

  case GDK_BUTTON_RELEASE:
    do {
      GdkEventButton * event = (GdkEventButton *) ev;
      PRIVATE(this)->buttonevent->setState(SoButtonEvent::UP);
      PRIVATE(this)->buttonevent->setShiftDown((event->state & GDK_SHIFT_MASK) ? TRUE : FALSE);
      PRIVATE(this)->buttonevent->setCtrlDown((event->state & GDK_CONTROL_MASK) ? TRUE : FALSE);
      PRIVATE(this)->buttonevent->setAltDown((event->state & GDK_MOD1_MASK) ? TRUE : FALSE);
      SbTime stamp;
      stamp.setMsecValue(event->time);
      PRIVATE(this)->buttonevent->setTime(stamp);
      this->setEventPosition(PRIVATE(this)->buttonevent,
                             (int) event->x, (int) event->y);
      switch (event->button) {
      case 1:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON1);
        break;
      case 2:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON3);
        break;
      case 3:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON2);
        break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      case 4:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON4);
        break;
      case 5:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::BUTTON5);
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      default:
        PRIVATE(this)->buttonevent->setButton(SoMouseButtonEvent::ANY);
        break;
      }
    } while (FALSE);
    return PRIVATE(this)->buttonevent;
    break;

  case GDK_MOTION_NOTIFY:
    do {
      GdkEventMotion * event = (GdkEventMotion *) ev;
      PRIVATE(this)->locationevent->setShiftDown(event->state & GDK_SHIFT_MASK);
      PRIVATE(this)->locationevent->setCtrlDown(event->state & GDK_CONTROL_MASK);
      PRIVATE(this)->locationevent->setAltDown(event->state & GDK_MOD1_MASK);
      SbTime stamp;
      stamp.setMsecValue(event->time);
      PRIVATE(this)->locationevent->setTime(stamp);
      this->setEventPosition(PRIVATE(this)->locationevent,
                             (int) event->x, (int) event->y);
    } while (0);
    return PRIVATE(this)->locationevent;
    break;

  default:
    return (SoEvent *) NULL;

  }

  return (const SoEvent *) NULL;
}

// *************************************************************************
