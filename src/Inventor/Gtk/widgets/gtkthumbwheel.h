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

/*  $Id$ */

#ifndef SOGTK_THUMBWHEEL_H
#define SOGTK_THUMBWHEEL_H

#include <gdk/gdk.h>
/* #include <gtk/gtkadjustment.h> */
#include <gtk/gtkwidget.h>
#include <gtk/gtkmisc.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

#define GTK_TYPE_THUMBWHEEL \
  (gtk_thumbwheel_get_type())

#define GTK_THUMBWHEEL(obj) \
  (GTK_CHECK_CAST((obj), GTK_TYPE_THUMBWHEEL, GtkThumbWheel))

#define GTK_THUMBWHEEL_CLASS(cclass) \
  (GTK_CHECK_CLASS_CAST((cclass), GTK_TYPE_THUMBWHEEL, GtkThumbwheelClass))

#define GTK_IS_THUMBWHEEL(obj) \
  (GTK_CHECK_TYPE((obj), GTK_TYPE_THUMBWHEEL))

#define GTK_IS_THUMBWHEEL_CLASS(cclass) \
  (GTK_CHECK_CLASS_TYPE((cclass), GTK_TYPE_THUMBWHEEL))

/* ********************************************************************** */

typedef enum
{
  GTK_THUMBWHEEL_BOUNDARY_CLAMP,
  GTK_THUMBWHEEL_BOUNDARY_MODULATE, 
  GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE
} GtkThumbWheelBoundaryType;
 
typedef enum
{
  GTK_THUMBWHEEL_MOTION_UNIFORM,
  GTK_THUMBWHEEL_MOTION_AUTHENTIC
} GtkThumbWheelMotionType;
 
typedef struct _GtkThumbWheel       GtkThumbWheel;
typedef struct _GtkThumbWheelClass  GtkThumbWheelClass;

struct _GtkThumbWheel {
  GtkMisc misc;

  GdkRectangle wheel_area ;
  
  guint8 state : 2;
  guint8 policy : 2;
  guint8 vertical : 1;
  guint8 button;
  gint downpos;
  gfloat value, tempvalue;

  int img; 		/* index of the bitmap to be shown */
  guchar *bitmap ;
  void * wheel; 	/* SoAnyThumbWheel class instance */
}; /* GtkThumbWheel */

struct _GtkThumbWheelClass {
  GtkMiscClass parent_class;

  void (* attached)       (GtkThumbWheel * thumbwheel);
  void (* value_changed)  (GtkThumbWheel * thumbwheel, gfloat value);
  void (* released)       (GtkThumbWheel * thumbwheel);
}; /* GtkThumbWheelClass */

/* ********************************************************************** */

GtkWidget *  gtk_thumbwheel_new               (gint vertical);
guint        gtk_thumbwheel_get_type          (void);
void         gtk_thumbwheel_set_update_policy (GtkThumbWheel * thumbwheel,
                                               GtkUpdateType policy);

void         gtk_thumbwheel_set_value         (GtkThumbWheel * thumbwheel,
                                               gfloat value);
gfloat       gtk_thumbwheel_get_value         (GtkThumbWheel * thumbwheel);

void         gtk_thumbwheel_set_range_boundary_handling(
  GtkThumbWheel * thumbwheel,
  GtkThumbWheelBoundaryType handling);
GtkThumbWheelBoundaryType  gtk_thumbwheel_get_range_boundary_handling(
  GtkThumbWheel * thumbwheel);

void         gtk_thumbwheel_set_motion_method(
  GtkThumbWheel * thumbwheel,
  GtkThumbWheelMotionType     handling);
GtkThumbWheelMotionType  gtk_thumbwheel_get_motion_method(
  GtkThumbWheel * thumbwheel);

/* ********************************************************************** */

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOGTK_THUMBWHEEL_H */
