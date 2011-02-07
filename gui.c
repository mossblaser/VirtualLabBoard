#include <gtk/gtk.h>
#include <cairo.h>


void
draw_lcd(GtkWidget *widget, gpointer data)
{
	cairo_t *lcd = gdk_cairo_create(widget->window);
	cairo_set_source_rgb(lcd, 0.0, 0.0, 0.0);
	cairo_move_to(lcd, 0.0, 0.0);
	cairo_line_to(lcd, 100.0, 100.0);
	cairo_set_line_width(lcd, 1.0);
	cairo_stroke(lcd);
	cairo_destroy(lcd);
}


int
main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *lcd;
	
	gtk_init(&argc, &argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(window, "delete-event",
	                 G_CALLBACK (gtk_main_quit), NULL);
	
	lcd = gtk_drawing_area_new();
	gtk_drawing_area_size(GTK_DRAWING_AREA(lcd), 100,100);
	g_signal_connect(lcd, "expose_event", G_CALLBACK(draw_lcd), NULL);
	
	gtk_container_add(GTK_CONTAINER(window), lcd);
	gtk_widget_show(lcd);
	gtk_widget_show(window);
	
	gtk_main();
	
	return 0;
}

