/**************************************************************************
 *
 *  This file is part of the Coin SoGtk GUI binding library.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/events/SoEvent.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/widgets/gtkthumbwheel.h>
#include <Inventor/Gtk/viewers/SoAnyWalkViewer.h>
#include <Inventor/Gtk/viewers/SoGtkWalkViewer.h>


/*!
  \class SoGtkWalkViewer Inventor/Gtk/viewers/SoGtkWalkViewer.h
  \brief The SoGtkWalkViewer class is yet to be documented...
  \ingroup components viewers
*/

// ************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkWalkViewer);

// ************************************************************************

/*!
  The public constructor.
*/

SoGtkWalkViewer::SoGtkWalkViewer(
  GtkWidget * parent,
  const char * name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag flag,
  SoGtkViewer::Type type )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyWalkViewer(this) )
{
  this->constructor( TRUE );
} // SoGtkWalkViewer()

// ************************************************************************

/*!
  A protected constructor.
*/

SoGtkWalkViewer::SoGtkWalkViewer( // protected
  GtkWidget * parent,
  const char * name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag flag,
  SoGtkViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyWalkViewer(this) )     
{
  this->constructor( build );
} // SoGtkWalkViewer()

// ************************************************************************

/*!
  \internal

  This contains the real constructor code (the two constructors are only
  entry points for this method).
*/

void
SoGtkWalkViewer::constructor( // private
  const SbBool build )
{
  // Cursors.
#if NOTYET
  GdkColormap *colormap = gtk_widget_get_colormap (this->getParentWidget());

  this->orthopixmap =
      gdk_pixmap_colormap_create_from_xpm_d( (GdkWindow *) 0, colormap,
        &this->orthomask, (GdkColor *) 0, ortho_xpm );
#endif

  this->setClassName("SoGtkWalkViewer");

//  this->addVisibilityChangeCallback( SoGtkWalkViewer::visibilityCB, this );

  this->setPopupMenuString( _( "Walk Viewer" ) );
  this->setPrefSheetString( _( "Walk Viewer Preference Sheet" ) );

  this->setLeftWheelString( _( "Tilt" ) );
  this->setBottomWheelString( _( "Rotate" ) );
  this->setRightWheelString( _( "Dolly" ) );

  if ( ! build ) return;
  GtkWidget * viewer = this->buildWidget( this->getParentWidget() );
  this->setBaseWidget( viewer );
} // constructor()

// ************************************************************************

/*!
  The destructor.
*/

SoGtkWalkViewer::~SoGtkWalkViewer(
  void )
{
  delete this->common ;
} // ~SoGtkWalkViewer()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setViewing( // virtual
  SbBool enable )
{
} // setViewing()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setCamera( // virtual
  SoCamera * camera )
{
} // setCamera()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setCursorEnabled( // virtual
  SbBool enable )
{
} // setCursorEnabled()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setCameraType( // virtual
  SoType type )
{
} // setCameraType()

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkWalkViewer::getDefaultWidgetName( // virtual
  void ) const
{
  static const char defaultWidgetName[] = N_( "Walk Viewer" );
  return _( defaultWidgetName );
} // getDefaultWidgetName()

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkWalkViewer::getDefaultTitle( // virtual
  void ) const
{
  static const char defaultTitle[] = N_( "Walk Viewer" );
  return _( defaultTitle );
} // getDefaultTitle()

// ************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkWalkViewer::getDefaultIconTitle( // virtual
  void ) const
{
  static const char defaultIconTitle[] = N_( "Walk Viewer" );
  return _( defaultIconTitle );
} // getDefaultIconTitle()

// ************************************************************************

/*!
  FIXME: write doc
*/

SbBool
SoGtkWalkViewer::processSoEvent( // virtual, protected
  const SoEvent * const event )
{
  if ( common->processSoEvent( event ) )
    return TRUE;
  return inherited::processSoEvent( event );
} // processSoEvent()

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::processEvent( // virtual, protected
  GdkEvent * event )
{
  // SoDebugError::postInfo( "SoGtkWalkViewer::processEvent", "[invoked]" );
  if ( SoGtkViewer::processCommonEvents( event ) )
    return;

  inherited::processEvent( event );
} // processEvent()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::setSeekMode( // virtual
  SbBool enable )
{
} // setSeekMode()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::actualRedraw( // virtual
  void )
{
} // actualRedraw()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::rightWheelMotion( // virtual
  float value )
{
} // rightWheelMotion()

// ************************************************************************

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkWalkViewer::buildLeftTrim( // virtual
  GtkWidget * parent )
{
  // get the examiner viewer's left vbox
  GtkWidget *trim = inherited::buildLeftTrim(parent);

  GtkWidget *uleftWheel = gtk_thumbwheel_new( 1 );   
  gtk_misc_set_padding( GTK_MISC(uleftWheel), 2, 2 );
  gtk_box_pack_end( GTK_BOX(trim), uleftWheel, FALSE, TRUE, FALSE );
  gtk_widget_show(uleftWheel);

#if NOTYET
  gtk_signal_connect( GTK_OBJECT(uleftWheel), "attached",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelPressedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(uleftWheel), "value_changed",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelMovedCB), (gpointer) this );
  gtk_signal_connect( GTK_OBJECT(uleftWheel), "released",
    GTK_SIGNAL_FUNC(SoGtkFullViewer::leftwheelReleasedCB), (gpointer) this );
#endif
  GtkWidget *uleftStr = gtk_label_new( _("H") );
  gtk_box_pack_end( GTK_BOX(trim), uleftStr, FALSE, TRUE, FALSE );
  gtk_widget_show(uleftStr);

  // HACK: reorder the leftWheel to the end of the vbox
  gtk_box_reorder_child(GTK_BOX(trim), this->leftWheel, INT_MAX );

  return trim ;
} // buildLeftTrim()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::createPrefSheet( // virtual
  void )
{
} // createPrefSheet()

// ************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkWalkViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoGtkWalkViewer.help" );
} // openViewerHelpCard()

// ************************************************************************

/*!
  Overloaded to add preferences settings for fly viewer
  specific stuff.
*/

GtkWidget *
SoGtkWalkViewer::makeSubPreferences(
  GtkWidget * parent )
{
  GtkWidget* hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (parent), hbox);

  GtkWidget *label1 = gtk_label_new( _( "Walking speed:" ) );
  gtk_box_pack_start (GTK_BOX (hbox), label1, FALSE, FALSE, 0);

  GtkWidget *incbutton = gtk_button_new_with_label( _( "increase" ) );
  gtk_box_pack_start (GTK_BOX (hbox), incbutton, FALSE, FALSE, 0);

  GtkWidget *decbutton = gtk_button_new_with_label( _( "decrease" ) );
  gtk_box_pack_start (GTK_BOX (hbox), decbutton, FALSE, FALSE, 0);

#if NOTYET
  gtk_signal_connect(GTK_OBJECT(incbutton), "activate",
    GTK_SIGNAL_FUNC(SoGtkWalkViewer::increaseCB), this );
  gtk_signal_connect(GTK_OBJECT(decbutton), "activate",
    GTK_SIGNAL_FUNC(SoGtkWalkViewer::decreaseCB), this );
#endif

  gtk_widget_show_all(hbox);

  return hbox;
}

