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
#include <gdk/gdkkeysyms.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if SOGTK_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOGTK_DEBUG
#include <Inventor/events/SoKeyboardEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkKeyboard.h>

// *************************************************************************

/*!
  \class SoGtkKeyboard Inventor/Gtk/devices/SoGtkKeyboard.h
  \brief The SoGtkKeyboard class translates GdkEventKey events into
  SoKeyboardEvent events.
  \ingroup devices
*/

// *************************************************************************

/*!
  \enum SoGtkKeyboard::Events
  FIXME: write doc
*/

/*!
  \var SoGtkKeyboard::Events SoGtkKeyboard::KEY_PRESS
  FIXME: write doc
*/

/*!
  \var SoGtkKeyboard::Events SoGtkKeyboard::KEY_RELEASE
  FIXME: write doc
*/

/*!
  \var SoGtkKeyboard::Events SoGtkKeyboard::ALL_EVENTS
  FIXME: write doc
*/

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkKeyboard);

// *************************************************************************

/*!
  Constructor.

  The \a eventbits argument decides which events are handled.
  It defaults to SoGtkKeyboard::ALL_EVENTS.
*/

SoGtkKeyboard::SoGtkKeyboard(
  const int eventbits )
{
  this->events = eventbits & SoGtkKeyboard::ALL_EVENTS;
  this->kbdevent = new SoKeyboardEvent;
} // SoGtkKeyboard()

/*!
  Destructor.
*/

SoGtkKeyboard::~SoGtkKeyboard(
  void )
{
  delete this->kbdevent;
} // ~SoGtkKeyboard()

// *************************************************************************

/*!
  \internal
  A Gtk-event handler internally used to automatically grab the keyboard
  focus, when a mouse pointer enters a widget connected to a SoGtkKeyboard.
*/

gboolean SoGtkKeyboard::EnterHandler(
  GtkWidget *widget,
  GdkEventCrossing *event,
  gpointer user_data)
{
  if ( !GTK_WIDGET_HAS_FOCUS(widget) )
    gtk_widget_grab_focus(widget);
  return FALSE;
}

/*!
  FIXME: write function documentation
*/

void
SoGtkKeyboard::enable(
  GtkWidget * widget,
  SoGtkEventHandler *func,
  gpointer closure )
{
  if ( func )
  {
    if ( this->events & SoGtkKeyboard::KEY_PRESS )
    {
      gtk_signal_connect(GTK_OBJECT(widget), "key_press_event",
        GTK_SIGNAL_FUNC(func), closure );
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_KEY_PRESS_MASK );
    }
    if ( this->events & SoGtkKeyboard::KEY_RELEASE )
    {
      gtk_signal_connect(GTK_OBJECT(widget), "key_release_event",
        GTK_SIGNAL_FUNC(func), closure );
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_KEY_RELEASE_MASK );
    }
    if ( this->events & SoGtkKeyboard::ALL_EVENTS )
    {
       GTK_WIDGET_SET_FLAGS(GTK_WIDGET(widget),GTK_CAN_FOCUS);
       gtk_signal_connect(GTK_OBJECT(widget), "enter-notify-event",
         GTK_SIGNAL_FUNC(&SoGtkKeyboard::EnterHandler), 0 );
       gtk_widget_add_events(GTK_WIDGET(widget),GDK_ENTER_NOTIFY_MASK );
    }
  }
} // enable()

/*!
  FIXME: write function documentation
*/

void
SoGtkKeyboard::disable(
  GtkWidget * widget,
  SoGtkEventHandler * func,
  gpointer closure)
{
  if ( func )
    gtk_signal_disconnect_by_func(GTK_OBJECT(widget),
      GTK_SIGNAL_FUNC(func), closure );
} // disable()

// *************************************************************************

static
SoKeyboardEvent::Key
getKeyForKeyCode(
  guint keycode )
{
  switch ( keycode ) {
  case GDK_Shift_L:        return SoKeyboardEvent::LEFT_SHIFT;
  case GDK_Shift_R:        return SoKeyboardEvent::RIGHT_SHIFT;
  case GDK_Control_L:      return SoKeyboardEvent::LEFT_CONTROL;
  case GDK_Control_R:      return SoKeyboardEvent::RIGHT_CONTROL;
  case GDK_Alt_L:          return SoKeyboardEvent::LEFT_ALT;
  case GDK_Alt_R:          return SoKeyboardEvent::RIGHT_ALT;
  case GDK_0:              return SoKeyboardEvent::NUMBER_0;
  case GDK_1:              return SoKeyboardEvent::NUMBER_1;
  case GDK_2:              return SoKeyboardEvent::NUMBER_2;
  case GDK_3:              return SoKeyboardEvent::NUMBER_3;
  case GDK_4:              return SoKeyboardEvent::NUMBER_4;
  case GDK_5:              return SoKeyboardEvent::NUMBER_5;
  case GDK_6:              return SoKeyboardEvent::NUMBER_6;
  case GDK_7:              return SoKeyboardEvent::NUMBER_7;
  case GDK_8:              return SoKeyboardEvent::NUMBER_8;
  case GDK_9:              return SoKeyboardEvent::NUMBER_9;

  case GDK_a: case GDK_A:  return SoKeyboardEvent::A;
  case GDK_b: case GDK_B:  return SoKeyboardEvent::B;
  case GDK_c: case GDK_C:  return SoKeyboardEvent::C;
  case GDK_d: case GDK_D:  return SoKeyboardEvent::D;
  case GDK_e: case GDK_E:  return SoKeyboardEvent::E;
  case GDK_f: case GDK_F:  return SoKeyboardEvent::F;
  case GDK_g: case GDK_G:  return SoKeyboardEvent::G;
  case GDK_h: case GDK_H:  return SoKeyboardEvent::H;
  case GDK_i: case GDK_I:  return SoKeyboardEvent::I;
  case GDK_j: case GDK_J:  return SoKeyboardEvent::J;
  case GDK_k: case GDK_K:  return SoKeyboardEvent::K;
  case GDK_l: case GDK_L:  return SoKeyboardEvent::L;
  case GDK_m: case GDK_M:  return SoKeyboardEvent::M;
  case GDK_n: case GDK_N:  return SoKeyboardEvent::N;
  case GDK_o: case GDK_O:  return SoKeyboardEvent::O;
  case GDK_p: case GDK_P:  return SoKeyboardEvent::P;
  case GDK_q: case GDK_Q:  return SoKeyboardEvent::Q;
  case GDK_r: case GDK_R:  return SoKeyboardEvent::R;
  case GDK_s: case GDK_S:  return SoKeyboardEvent::S;
  case GDK_t: case GDK_T:  return SoKeyboardEvent::T;
  case GDK_u: case GDK_U:  return SoKeyboardEvent::U;
  case GDK_v: case GDK_V:  return SoKeyboardEvent::V;
  case GDK_w: case GDK_W:  return SoKeyboardEvent::W;
  case GDK_x: case GDK_X:  return SoKeyboardEvent::X;
  case GDK_y: case GDK_Y:  return SoKeyboardEvent::Y;
  case GDK_z: case GDK_Z:  return SoKeyboardEvent::Z;

  case GDK_Home:           return SoKeyboardEvent::HOME;
  case GDK_Left:           return SoKeyboardEvent::LEFT_ARROW;
  case GDK_Up:             return SoKeyboardEvent::UP_ARROW;
  case GDK_Right:          return SoKeyboardEvent::RIGHT_ARROW;
  case GDK_Down:           return SoKeyboardEvent::DOWN_ARROW;
  case GDK_Page_Up:        return SoKeyboardEvent::PAGE_UP;
  case GDK_Page_Down:      return SoKeyboardEvent::PAGE_DOWN;
  case GDK_End:            return SoKeyboardEvent::END;
#if 0
  case GDK_Prior:          return SoKeyboardEvent::PRIOR;
  case GDK_Next:           return SoKeyboardEvent::NEXT;
#endif

  case GDK_KP_Enter:       return SoKeyboardEvent::PAD_ENTER;
  case GDK_KP_F1:          return SoKeyboardEvent::PAD_F1;
  case GDK_KP_F2:          return SoKeyboardEvent::PAD_F2;
  case GDK_KP_F3:          return SoKeyboardEvent::PAD_F3;
  case GDK_KP_F4:          return SoKeyboardEvent::PAD_F4;
  case GDK_KP_0:           return SoKeyboardEvent::PAD_0;
  case GDK_KP_1:           return SoKeyboardEvent::PAD_1;
  case GDK_KP_2:           return SoKeyboardEvent::PAD_2;
  case GDK_KP_3:           return SoKeyboardEvent::PAD_3;
  case GDK_KP_4:           return SoKeyboardEvent::PAD_4;
  case GDK_KP_5:           return SoKeyboardEvent::PAD_5;
  case GDK_KP_6:           return SoKeyboardEvent::PAD_6;
  case GDK_KP_7:           return SoKeyboardEvent::PAD_7;
  case GDK_KP_8:           return SoKeyboardEvent::PAD_8;
  case GDK_KP_9:           return SoKeyboardEvent::PAD_9;
  case GDK_KP_Add:         return SoKeyboardEvent::PAD_ADD;
  case GDK_KP_Subtract:    return SoKeyboardEvent::PAD_SUBTRACT;
  case GDK_KP_Multiply:    return SoKeyboardEvent::PAD_MULTIPLY;
  case GDK_KP_Divide:      return SoKeyboardEvent::PAD_DIVIDE;
  case GDK_KP_Space:       return SoKeyboardEvent::PAD_SPACE;
  case GDK_KP_Tab:         return SoKeyboardEvent::PAD_TAB;
  case GDK_KP_Insert:      return SoKeyboardEvent::PAD_INSERT;
  case GDK_KP_Delete:      return SoKeyboardEvent::PAD_DELETE;
  case GDK_KP_Decimal:     return SoKeyboardEvent::PAD_PERIOD;

  case GDK_F1:             return SoKeyboardEvent::F1;
  case GDK_F2:             return SoKeyboardEvent::F2;
  case GDK_F3:             return SoKeyboardEvent::F3;
  case GDK_F4:             return SoKeyboardEvent::F4;
  case GDK_F5:             return SoKeyboardEvent::F5;
  case GDK_F6:             return SoKeyboardEvent::F6;
  case GDK_F7:             return SoKeyboardEvent::F7;
  case GDK_F8:             return SoKeyboardEvent::F8;
  case GDK_F9:             return SoKeyboardEvent::F9;
  case GDK_F10:            return SoKeyboardEvent::F10;
  case GDK_F11:            return SoKeyboardEvent::F11;
  case GDK_F12:            return SoKeyboardEvent::F12;

  case GDK_BackSpace:      return SoKeyboardEvent::BACKSPACE;
  case GDK_Tab:            return SoKeyboardEvent::TAB;
  case GDK_Return:         return SoKeyboardEvent::RETURN;
#if 0
  case GDK_Return:         return SoKeyboardEvent::ENTER;
#endif
  case GDK_Pause:          return SoKeyboardEvent::PAUSE;
  case GDK_Scroll_Lock:    return SoKeyboardEvent::SCROLL_LOCK;
  case GDK_Escape:         return SoKeyboardEvent::ESCAPE;
#ifdef HAVE_SOKEYBOARDEVENT_DELETE
  case GDK_Delete:         return SoKeyboardEvent::DELETE;
#else
  case GDK_Delete:         return SoKeyboardEvent::KEY_DELETE;
#endif
  case GDK_Print:          return SoKeyboardEvent::PRINT;
  case GDK_Insert:         return SoKeyboardEvent::INSERT;
  case GDK_Num_Lock:       return SoKeyboardEvent::NUM_LOCK;
  case GDK_Caps_Lock:      return SoKeyboardEvent::CAPS_LOCK;
  case GDK_Shift_Lock:     return SoKeyboardEvent::SHIFT_LOCK;

  case GDK_space:          return SoKeyboardEvent::SPACE;
  case GDK_apostrophe:     return SoKeyboardEvent::APOSTROPHE;
  case GDK_comma:          return SoKeyboardEvent::COMMA;
  case GDK_minus:          return SoKeyboardEvent::MINUS;
  case GDK_period:         return SoKeyboardEvent::PERIOD;
  case GDK_slash:          return SoKeyboardEvent::SLASH;
  case GDK_semicolon:      return SoKeyboardEvent::SEMICOLON;
  case GDK_equal:          return SoKeyboardEvent::EQUAL;
  case GDK_bracketleft:    return SoKeyboardEvent::BRACKETLEFT;
  case GDK_backslash:      return SoKeyboardEvent::BACKSLASH;
  case GDK_bracketright:   return SoKeyboardEvent::BRACKETRIGHT;
  case GDK_grave:          return SoKeyboardEvent::GRAVE;

  default:
    break;
  } // switch ( keycode )

  return SoKeyboardEvent::ANY;
} // getKeyForKeyCode()

// *************************************************************************

/*!
  FIXME: write function documentation
*/

const SoEvent *
SoGtkKeyboard::translateEvent( // virtual
  GdkEvent * ev )
{
  switch ( ev->type ) {
  case GDK_KEY_PRESS:
    if ( this->events & KEY_PRESS ) {
      const GdkEventKey * const event = (GdkEventKey *) ev;
      SbTime stamp;
      stamp.setMsecValue( event->time );
      this->kbdevent->setTime( stamp );
      const SbVec2s pos = SoGtkDevice::getLastEventPosition();
      SoGtkDevice::setEventPosition( this->kbdevent, pos[0], pos[1] );
      this->kbdevent->setKey( getKeyForKeyCode( event->keyval ) );
      this->kbdevent->setState( SoButtonEvent::DOWN );
      this->kbdevent->setShiftDown( (event->state & GDK_SHIFT_MASK) ? TRUE : FALSE );
      this->kbdevent->setCtrlDown( (event->state & GDK_CONTROL_MASK) ? TRUE : FALSE );
      this->kbdevent->setAltDown( (event->state & GDK_MOD1_MASK) ? TRUE : FALSE );
      return this->kbdevent;
    }
    break;

  case GDK_KEY_RELEASE:
    if ( this->events & KEY_RELEASE ) {
      const GdkEventKey * const event = (GdkEventKey *) ev;
      SbTime stamp;
      stamp.setMsecValue( event->time );
      this->kbdevent->setTime( stamp );
      const SbVec2s pos = SoGtkDevice::getLastEventPosition();
      SoGtkDevice::setEventPosition( this->kbdevent, pos[0], pos[1] );
      this->kbdevent->setKey( getKeyForKeyCode( event->keyval ) );
      this->kbdevent->setState( SoButtonEvent::UP );
      this->kbdevent->setShiftDown( (event->state & GDK_SHIFT_MASK) ? TRUE : FALSE );
      this->kbdevent->setCtrlDown( (event->state & GDK_CONTROL_MASK) ? TRUE : FALSE );
      this->kbdevent->setAltDown( (event->state & GDK_MOD1_MASK) ? TRUE : FALSE );
      return this->kbdevent;
    }
    break;

  default:
    break;
  } // switch ( ev->type )

  return (const SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkKeyboardRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

