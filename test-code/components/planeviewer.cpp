/**************************************************************************\
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>

#include "computils.h"

// *************************************************************************

int
main(
  int argc,
  char ** argv )
{
  GtkWidget * window = SoGtk::init( argv[0] );

  SoNode * root = get_scene_graph( argc, argv );
  if ( ! root ) return -1;

  SoGtkPlaneViewer * viewer = new SoGtkPlaneViewer();
  viewer->setSceneGraph( root );
  viewer->show();
  // SoGtk::show( window );

  SoGtk::mainLoop();

  delete viewer;
  return 0;
} // main()

// *************************************************************************
