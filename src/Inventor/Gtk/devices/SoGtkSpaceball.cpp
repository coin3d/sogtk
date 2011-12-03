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
#endif

#include <sogtkdefs.h>
#include <Inventor/Gtk/devices/SoGtkSpaceball.h>
#include <Inventor/Gtk/devices/SoGtkSpaceballP.h>

// *************************************************************************

// FIXME:
//  - not implemented yet! no spaceball available
//  - idea: maybe we could make device classes for other available cheap
//    PC equipment? wheel mice, joysticks, ....
//  - implement XInput support

// *************************************************************************

SoGtkSpaceball::SoGtkSpaceball(int eventbits)
{
  PRIVATE(this) = new SoGtkSpaceballP(this);
  PRIVATE(this)->events = eventbits;
  PRIVATE(this)->rotationscale = 0.006f;
  PRIVATE(this)->translationscale = 0.006f;
  PRIVATE(this)->focustowindow = FALSE;
}

SoGtkSpaceball::~SoGtkSpaceball()
{
  delete PRIVATE(this);
}

// *************************************************************************

void
SoGtkSpaceball::enable(GtkWidget * widget,
                       SoGtkEventHandler * func,
                       gpointer closure)
{
  if (func)
    gtk_signal_connect(GTK_OBJECT(widget), "event",
                       GTK_SIGNAL_FUNC(func), closure);
}

void
SoGtkSpaceball::disable(GtkWidget * widget,
                        SoGtkEventHandler * func,
                        gpointer closure)
{
  if (func)
    gtk_signal_disconnect_by_func(GTK_OBJECT(widget),
                                  GTK_SIGNAL_FUNC(func), closure);
}

// *************************************************************************

const SoEvent *
SoGtkSpaceball::translateEvent(GdkEvent * /*event*/)
{
  // FIXME: implement
  SOGTK_STUB();
  return NULL;
}

// *************************************************************************

SbBool
SoGtkSpaceball::exists(void)
{
  // FIXME: implement
  SOGTK_STUB();
  return FALSE;
}

void
SoGtkSpaceball::setFocusToWindow(SbBool flag)
{
  PRIVATE(this)->focustowindow = flag;
}

SbBool
SoGtkSpaceball::isFocusToWindow(void) const
{
  return PRIVATE(this)->focustowindow;
}

// *************************************************************************
