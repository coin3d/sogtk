/* ********************************************************************** */

#include <stdio.h>

#include <gtk/gtk.h>

#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

/* ********************************************************************** */

int
main(
  int argc,
  char ** argv )
{
  GtkWidget * window;
  GtkWidget * wheel;

  gtk_init( &argc, &argv );

  window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_signal_connect( GTK_OBJECT(window), "destroy",
                      GTK_SIGNAL_FUNC(gtk_exit), NULL );

  gtk_container_set_border_width( GTK_CONTAINER(window), 10 );

  wheel = gtk_thumbwheel_new( TRUE );
  gtk_container_add( GTK_CONTAINER(window), wheel );
  gtk_widget_show( wheel );
  gtk_widget_show( window );

  gtk_main();
  return 0;
} /* main() */

/* ********************************************************************** */
