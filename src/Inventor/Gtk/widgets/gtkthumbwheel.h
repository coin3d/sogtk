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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __GTK_THUMBWHEEL_H__
#define __GTK_THUMBWHEEL_H__

#include <gdk/gdk.h>
#include <gtk/gtkadjustment.h>
#include <gtk/gtkwidget.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

#define GTK_TYPE_THUMBWHEEL \
  (gtk_thumbwheel_get_type())

#define GTK_THUMBWHEEL(obj) \
  (GTK_CHECK_CAST( (obj), GTK_TYPE_THUMBWHEEL, GtkThumbWheel ))

#define GTK_THUMBWHEEL_CLASS(cclass) \
  (GTK_CHECK_CLASS_CAST( (cclass), GTK_TYPE_THUMBWHEEL, GtkThumbwheelClass ))

#define GTK_IS_THUMBWHEEL(obj) \
  (GTK_CHECK_TYPE( (obj), GTK_TYPE_THUMBWHEEL ))

#define GTK_IS_THUMBWHEEL_CLASS(cclass) \
  (GTK_CHECK_CLASS_TYPE( (cclass), GTK_TYPE_THUMBWHEEL ))

/* ********************************************************************** */

typedef struct _GtkThumbWheel       GtkThumbWheel;
typedef struct _GtkThumbWheelClass  GtkThumbWheelClass;

struct _GtkThumbWheel {
  GtkWidget widget;

  guint8 policy : 2;
  guint8 vertical : 1;
  guint8 button;
  gint downpos;
  gfloat value, tempvalue;
  void * wheel;

};

struct _GtkThumbWheelClass {
  GtkWidgetClass parent_class;

  void (* thumbwheel_activated)  ( GtkThumbWheel * thumbwheel );
  void (* thumbwheel_changed)    ( GtkThumbWheel * thumbwheel, gfloat value );
  void (* thumbwheel_settled)    ( GtkThumbWheel * thumbwheel );
};

/* ********************************************************************** */

GtkWidget *  gtk_thumbwheel_new               (gint vertical);
guint        gtk_thumbwheel_get_type          (void);
void         gtk_thumbwheel_set_update_policy (GtkThumbWheel * thumbwheel,
                                               GtkUpdateType policy);

/* ********************************************************************** */

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus */

#endif /* ! __GTK_THUMBWHEEL_H__ */
