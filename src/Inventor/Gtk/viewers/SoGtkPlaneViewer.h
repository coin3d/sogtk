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

// $Id$

#ifndef SOGTK_PLANEVIEWER_H
#define SOGTK_PLANEVIEWER_H

#include <Inventor/Gtk/viewers/SoGtkViewer.h>

// *************************************************************************

class SOGTK_DLL_EXPORT SoGtkPlaneViewer : public SoGtkFullViewer {
  SOGTK_OBJECT_HEADER(SoGtkPlaneViewer, SoGtkFullViewer);
  typedef SoGtkFullViewer inherited;

public:
  SoGtkPlaneViewer(GtkWidget * parent, ...);
  ~SoGtkPlaneViewer(void);

protected:
  SoGtkPlaneViewer(GtkWidget * parent, ...);

private:
  void constructor( const SbBool build );

}; // class SoGtkFullViewer

// *************************************************************************

#endif // ! SOGTK_PLANEVIEWER_H
