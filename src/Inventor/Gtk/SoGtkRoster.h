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

#ifndef SOGTK_ROSTER_H
#define SOGTK_ROSTER_H

#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/SoGtkComponent.h>

// *************************************************************************

class SOGTK_DLL_API SoGtkRoster : public SoGtkComponent {
  SOGTK_OBJECT_HEADER(SoGtkRoster, SoGtkComponent);

public:
  enum BuildFlags {
    MENUBAR = 0x01,
    ROSTERLIST = 0x02,
    STATUSBAR = 0x04,
    BUILD_ALL = 0x07
  };

  SoGtkRoster(
    GtkWidget * parent = (GtkWidget *) NULL,
    const char * const name = (char *) NULL,
    const SbBool embed = TRUE,
    const int flags = BUILD_ALL);
  ~SoGtkRoster(void);

protected:
  SoGtkRoster(GtkWidget * parent, const char * const name,
    const SbBool embed, const int flags, const SbBool build);

  GtkWidget * buildWidget(GtkWidget * parent);
  virtual GtkWidget * buildMenuBarWidget(GtkWidget * parent);
  virtual GtkWidget * buildRosterListWidget(GtkWidget * parent);
  virtual GtkWidget * buildStatusBarWidget(GtkWidget * parent);

  virtual void sizeChanged(const SbVec2s & size);

  virtual void componentCreated(SoGtkComponent * component);
  virtual void componentDestroyed(SoGtkComponent * component);
  virtual void componentChanged(SoGtkComponent * component);

  void buildRosterList(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor(const SbBool build);

  int buildflags;
  GtkAdjustment * horizontal;
  GtkAdjustment * vertical;
  GtkWidget * rosterbase;
  GtkWidget * menubar;
  GtkWidget * rosterlist;
  GtkWidget * statusbar;
  GtkWidget * listwidget;

  static void componentActionCB(SoGtkComponent * component,
    SoGtk::SoGtkComponentAction action, void * closure);

}; // class SoGtkRoster

// *************************************************************************

#endif // ! SOGTK_ROSTER_H
