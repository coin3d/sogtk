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

#ifndef SOGTKCOMPONENTP_H
#define SOGTKCOMPONENTP_H

#include <Inventor/Gtk/SoGuiComponentP.h>
#include <Inventor/Gtk/SoGtkCursor.h>

// ************************************************************************

// "Hidden" class for storing private data (to avoid cluttering up the
// header file, and to make it easier to keep ABI compatibility upon
// changes.)

class SoGtkComponentP : public SoGuiComponentP
{
public:
  SoGtkComponentP(SoGtkComponent * publ);
  ~SoGtkComponentP();

  static gint realizeHandlerCB(GtkObject * object, gpointer closure);
  static GdkCursor * getNativeCursor(GtkWidget * w,
                                     const SoGtkCursor::CustomCursor * cc);

  GtkWidget * widget;
  GtkWidget * parent;
  SbBool embedded;
  SbBool fullscreen;
  SbBool shelled;
  char * className;
  char * widgetName;
  char * captionText;
  char * iconText;
  SoGtkComponentCB * closeCB;
  void * closeCBdata;
  SbPList * visibilityChangeCBs;
  SbVec2s storeSize;

  static GdkCursor * arrowcursor;
  static GdkCursor * crosscursor;
  static GdkCursor * uparrowcursor;

  struct NonFull {
    gint x, y, w, h;
  };
  NonFull nonfull;

private:
  static SbDict * cursordict;
};

// ************************************************************************

#endif // !SOGTKCOMPONENTP_H
