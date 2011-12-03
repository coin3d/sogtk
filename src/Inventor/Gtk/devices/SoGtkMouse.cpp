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
