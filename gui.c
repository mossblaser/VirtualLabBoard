#include "gui.h"

static void
draw_pixel(cairo_t *lcd, gboolean state, int x, int y)
{
	if (state)
		cairo_set_source_rgb(lcd, 1.0, 1.0, 1.0);
	else
		cairo_set_source_rgb(lcd, 0.0, 0.0, 0.0);
	
	double px = ((double)x) * 3.0;
	double py = ((double)y) * 3.0;
	
	cairo_rectangle(lcd, px, py, px + 2.0, py + 2.0);
	cairo_fill(lcd);
}


static void
draw_lcd(GtkWidget *widget, gpointer data)
{
	cairo_t *lcd = gdk_cairo_create(widget->window);
	
	cairo_set_source_rgb(lcd, 0.0, 0.0, 0.0);
	cairo_rectangle(lcd, 0.0, 0.0, 6.0 * 3.0, 8.0 * 3.0);
	cairo_fill(lcd);
	
	int x, y;
	for (x = 0; x < 5; x++)
		for (y = 0; y < 7; y++)
			draw_pixel(lcd, TRUE, x, y);
	
	cairo_destroy(lcd);
}




int shmid;
shm_data_t *shm = NULL;

int
main(int argc, char *argv[])
{
	shmid = shmget(VLB_SHM_ID, VLB_SHM_SIZE, VLB_SHM_FLG);
	if (shmid == -1) {
		fprintf(stderr, "Failed to create SHM!");
		return 1;
	}
	
	shm = shmat(shmid, NULL, 0);
	// This peutrid cast is specified by SHM
	if (shm == (void *)-1) {
		fprintf(stderr, "Failed to attatch SHM!");
		return 2;
	}
	
	shmdt(shm);
	
	
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

