#include "gui.h"


/******************************************************************************
 * SHM Management Code                                                        *
 ******************************************************************************/

shm_t shm;

static boolean
create_shm(void)
{
	// Initialise the struct
	shm.mem = NULL;
	shm.connected = FALSE;
	
	// Create the shared memory area
	shm.id = shmget(VLB_SHM_ID, VLB_SHM_SIZE, VLB_SHM_FLG);
	if (shm.id == -1)
		return FALSE;
	
	// Attatch the shared memory to a pointer
	shm.mem = shmat(shm.id, NULL, 0);
	
	// This peutrid cast is specified by SHM
	if (shm.mem == (void *)-1)
		return FALSE;
	
	shm.connected = TRUE;
	return TRUE;
}


static boolean
free_shm(void)
{
	// Detatch the shared memory if attatched
	if (shm.connected == TRUE)
		return shmdt(shm.mem) == 0;
	else
		return TRUE;
}



/******************************************************************************
 * GUI Code                                                                   *
 ******************************************************************************/

GtkWidget *window;
GtkWidget *board;

static void
draw_led(cairo_t *board, gboolean state,
         double r, double g, double b,
         double x, double y)
{
	if (state)
		cairo_set_source_rgb(board, r, g, b);
	else
		cairo_set_source_rgb(board, r*0.2, g*0.2, b*0.2);
	
	cairo_arc(board, x, y, VLB_LED_SIZE, 0.0, 2*M_PI);
	cairo_fill(board);
	
	cairo_set_source_rgb(board, r*0.2, g*0.2, b*0.2);
	cairo_arc(board, x, y, VLB_LED_SIZE, 0.0, 2*M_PI);
	cairo_set_line_width(board, 1.0);
	cairo_stroke(board);
}


static void
draw_leds(GtkWidget *widget, gpointer data)
{
	cairo_t *board = gdk_cairo_create(widget->window);
	
	uint8_t port_a = 0x00;
	if (shm.connected) {
		port_a = shm.mem->memory[0];
	}
	
	int col, row;
	for (col = 0; col < 2; col++)
		for (row = 0; row < 4; row++)
			draw_led(board, (port_a & led_bits[(col*4) + row]) != 0,
			         led_r[row], led_g[row], led_b[row],
			         led_x[col], led_y[row]);
	
	cairo_destroy(board);
}

static gboolean
refresh_leds(GtkWidget *widget)
{
	static uint8_t last_port_a = 0x00;
	uint8_t port_a = 0x00;
	if (shm.connected) {
		port_a = shm.mem->memory[0];
	}
	
	if (port_a != last_port_a)
		draw_leds(board, NULL);
	
	last_port_a = port_a;
	
	return TRUE;
}


void
setup_gui(void)
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(window, "delete-event",
	                 G_CALLBACK (gtk_main_quit), NULL);
	
	board = gtk_drawing_area_new();
	gtk_drawing_area_size(GTK_DRAWING_AREA(board), 100,100);
	g_signal_connect(board, "expose_event", G_CALLBACK(draw_leds), NULL);
	
	g_timeout_add(VLB_UPDATE_FREQUENCY, (GSourceFunc)refresh_leds, NULL);
	
	gtk_container_add(GTK_CONTAINER(window), board);
	gtk_widget_show(board);
	gtk_widget_show(window);
}


/******************************************************************************
 * Startup Code & Mainloop                                                    *
 ******************************************************************************/

int
main(int argc, char *argv[])
{
	if (!create_shm()) {
		g_debug("Could not connect to SHM!\n");
	}
	
	gtk_init(&argc, &argv);
	setup_gui();
	gtk_main();
	
	if (!free_shm()) {
		g_debug("Could not free to SHM!\n");
		return 1;
	}
	
	return 0;
}

