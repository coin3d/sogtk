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

#include <gdk/gdktypes.h>
#include <gdk/gdkkeysyms.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkKeyboard.h>
#include <Inventor/Gtk/devices/SoGuiKeyboardP.h>
#include <Inventor/Gtk/devices/SoGtkInputFocus.h>

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

class SoGtkKeyboardP : public SoGuiKeyboardP {
};

// *************************************************************************

SoGtkKeyboard::SoGtkKeyboard(int eventbits)
{
  PRIVATE(this) = new SoGtkKeyboardP;
  PRIVATE(this)->eventmask = eventbits & SoGtkKeyboard::ALL_EVENTS;
}

SoGtkKeyboard::~SoGtkKeyboard()
{
  delete PRIVATE(this);
}

// *************************************************************************

// A Gtk-event handler internally used to automatically grab the
// keyboard focus, when a mouse pointer enters a widget connected to a
// SoGtkKeyboard.
static gboolean
EnterHandler(GtkWidget *widget,
             GdkEventCrossing *event,
             gpointer user_data)
{
  // Compell CAN_FOCUS on the (gtkglarea) widget. G.Barrand.
  if (!GTK_WIDGET_CAN_FOCUS(widget)) 
    GTK_OBJECT_SET_FLAGS(widget,GTK_CAN_FOCUS);
  if (!GTK_WIDGET_HAS_FOCUS(widget))
    gtk_widget_grab_focus(widget);
  return FALSE;
}

void
SoGtkKeyboard::enable(GtkWidget * widget, SoGtkEventHandler * func,
                      gpointer closure)
{
  if (func) {
    if (PRIVATE(this)->eventmask & SoGtkKeyboard::KEY_PRESS) {
      gtk_signal_connect(GTK_OBJECT(widget), "key_press_event",
                         GTK_SIGNAL_FUNC(func), closure);
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_KEY_PRESS_MASK);
    }
    if (PRIVATE(this)->eventmask & SoGtkKeyboard::KEY_RELEASE) {
      gtk_signal_connect(GTK_OBJECT(widget), "key_release_event",
                         GTK_SIGNAL_FUNC(func), closure);
      gtk_widget_add_events(GTK_WIDGET(widget),GDK_KEY_RELEASE_MASK);
    }
  }

  // When entering the window, we want to have the keyboard
  // focus. G.Barrand.
  //
  // FIXME: this seems bogus, as we're checking our eventmask against
  // a flag in SoGtkInputFocus? Does it work? If so, is it by sheer
  // luck? 20020625 mortene.
  if (PRIVATE(this)->eventmask & SoGtkInputFocus::ENTER_WINDOW) {
    gtk_signal_connect(GTK_OBJECT(widget), "enter_notify_event",
                       GTK_SIGNAL_FUNC(EnterHandler), closure);
    gtk_widget_add_events(GTK_WIDGET(widget), GDK_ENTER_NOTIFY_MASK);
  }
}

void
SoGtkKeyboard::disable(GtkWidget * widget,
                       SoGtkEventHandler * func,
                       gpointer closure)
{
  if (func) {
    gtk_signal_disconnect_by_func(GTK_OBJECT(widget),
                                  GTK_SIGNAL_FUNC(func), closure);
  }
}

// *************************************************************************

static
SoKeyboardEvent::Key
getKeyForKeyCode(guint keycode, char & printable)
{
  switch (keycode) {
  case GDK_Shift_L:        return SoKeyboardEvent::LEFT_SHIFT;
  case GDK_Shift_R:        return SoKeyboardEvent::RIGHT_SHIFT;
  case GDK_Control_L:      return SoKeyboardEvent::LEFT_CONTROL;
  case GDK_Control_R:      return SoKeyboardEvent::RIGHT_CONTROL;
  case GDK_Alt_L:          return SoKeyboardEvent::LEFT_ALT;
  case GDK_Alt_R:          return SoKeyboardEvent::RIGHT_ALT;
  case GDK_0: 
    printable = '0';
    return SoKeyboardEvent::NUMBER_0;
  case GDK_parenright:
    printable = ')';
    return SoKeyboardEvent::NUMBER_0;
    
  case GDK_exclam:
    printable = '!';
  case GDK_1: 
    return SoKeyboardEvent::NUMBER_1;  
  case GDK_at: 
    printable = '@';
  case GDK_2: 
    return SoKeyboardEvent::NUMBER_2;
  case GDK_numbersign:
    printable = '#';
  case GDK_3: 
    return SoKeyboardEvent::NUMBER_3;
  case GDK_dollar: 
    printable = '$';
  case GDK_4:
    return SoKeyboardEvent::NUMBER_4;
  case GDK_percent:
    printable = '%';
  case GDK_5: 
    return SoKeyboardEvent::NUMBER_5;
  case GDK_asciicircum: // ?
    printable = '^';
  case GDK_6: 
    return SoKeyboardEvent::NUMBER_6;
  case GDK_ampersand:
    printable = '&';
  case GDK_7: 
    return SoKeyboardEvent::NUMBER_7;
  case GDK_asterisk:
    printable = '*';
  case GDK_8: 
    return SoKeyboardEvent::NUMBER_8;
  case GDK_parenleft:
    printable = '(';
  case GDK_9: 
    return SoKeyboardEvent::NUMBER_9;

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

  case GDK_space: return SoKeyboardEvent::SPACE;

  case GDK_apostrophe:
    printable = '\'';
    return SoKeyboardEvent::APOSTROPHE;
  case GDK_quotedbl: 
    printable = '\"';
    return SoKeyboardEvent::APOSTROPHE;

  case GDK_comma:
    printable = ',';
    return SoKeyboardEvent::COMMA;
  case GDK_less: 
    printable = '<';
    return SoKeyboardEvent::COMMA;

  case GDK_minus:
    printable = '-';
    return SoKeyboardEvent::MINUS;
  case GDK_underscore:
    printable = '_';
    return SoKeyboardEvent::MINUS;

  case GDK_period:
    printable = '.';
    return SoKeyboardEvent::PERIOD;
  case GDK_greater:
    printable = '>';
    return SoKeyboardEvent::PERIOD;

  case GDK_slash:
    printable = '/';
    return SoKeyboardEvent::SLASH;
  case GDK_question:
    printable = '?';
    return SoKeyboardEvent::SLASH;

  case GDK_semicolon:
    printable = ';';
    return SoKeyboardEvent::SEMICOLON;
  case GDK_colon:
    printable = ':';
    return SoKeyboardEvent::SEMICOLON;

  case GDK_equal:
    printable = '=';
    return SoKeyboardEvent::EQUAL;
  case GDK_plus:
    printable = '+';
    return SoKeyboardEvent::EQUAL;

  case GDK_bracketleft:
    printable = '[';
    return SoKeyboardEvent::BRACKETLEFT;
  case GDK_braceleft:
    printable = '{';
    return SoKeyboardEvent::BRACKETLEFT;

  case GDK_backslash:
    printable = '\\';
    return SoKeyboardEvent::BACKSLASH;
  case GDK_bar:
    printable = '|';
    return SoKeyboardEvent::BACKSLASH;

  case GDK_bracketright:
    printable = ']';
    return SoKeyboardEvent::BRACKETRIGHT;
  case GDK_braceright:
    printable = '}';
    return SoKeyboardEvent::BRACKETRIGHT;
    
  case GDK_grave:
    printable = '`';
    return SoKeyboardEvent::GRAVE;
  case GDK_asciitilde:
    printable = '~';
    return SoKeyboardEvent::GRAVE;

  default:
    break;
  }

  return SoKeyboardEvent::ANY;
}

// *************************************************************************

const SoEvent *
SoGtkKeyboard::translateEvent(GdkEvent * ev)
{
  switch (ev->type) {
  case GDK_KEY_PRESS:
    if (PRIVATE(this)->eventmask & KEY_PRESS) {
      const GdkEventKey * const event = (GdkEventKey *) ev;
      SbTime stamp;
      stamp.setMsecValue(event->time);
      PRIVATE(this)->kbdevent->setTime(stamp);
      const SbVec2s pos = SoGtkDevice::getLastEventPosition();
      SoGtkDevice::setEventPosition(PRIVATE(this)->kbdevent, pos[0], pos[1]);
      char printable = 0;
      PRIVATE(this)->kbdevent->setKey(getKeyForKeyCode(event->keyval, printable));
#if 0 // disabled. Breaks build when compiling against OIV
      if (printable) PRIVATE(this)->kbdevent->setPrintableCharacter(printable);
#endif // disabled
      PRIVATE(this)->kbdevent->setState(SoButtonEvent::DOWN);
      PRIVATE(this)->kbdevent->setShiftDown((event->state & GDK_SHIFT_MASK) ? TRUE : FALSE);
      PRIVATE(this)->kbdevent->setCtrlDown((event->state & GDK_CONTROL_MASK) ? TRUE : FALSE);
      PRIVATE(this)->kbdevent->setAltDown((event->state & GDK_MOD1_MASK) ? TRUE : FALSE);
      return PRIVATE(this)->kbdevent;
    }
    break;

  case GDK_KEY_RELEASE:
    if (PRIVATE(this)->eventmask & KEY_RELEASE) {
      const GdkEventKey * const event = (GdkEventKey *) ev;
      SbTime stamp;
      stamp.setMsecValue(event->time);
      PRIVATE(this)->kbdevent->setTime(stamp);
      const SbVec2s pos = SoGtkDevice::getLastEventPosition();
      SoGtkDevice::setEventPosition(PRIVATE(this)->kbdevent, pos[0], pos[1]);
      char printable = 0;
      PRIVATE(this)->kbdevent->setKey(getKeyForKeyCode(event->keyval, printable));
#if 0 // disabled. Breaks build when compiling against OIV
      if (printable) PRIVATE(this)->kbdevent->setPrintableCharacter(printable);
#endif // disabled
      PRIVATE(this)->kbdevent->setState(SoButtonEvent::UP);
      PRIVATE(this)->kbdevent->setShiftDown((event->state & GDK_SHIFT_MASK) ? TRUE : FALSE);
      PRIVATE(this)->kbdevent->setCtrlDown((event->state & GDK_CONTROL_MASK) ? TRUE : FALSE);
      PRIVATE(this)->kbdevent->setAltDown((event->state & GDK_MOD1_MASK) ? TRUE : FALSE);
      return PRIVATE(this)->kbdevent;
    }
    break;

  default:
    break;
  }

  return (const SoEvent *) NULL;
}

// *************************************************************************
