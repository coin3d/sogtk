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
GtkType      gtk_thumbwheel_get_type          (void);
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
