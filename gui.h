#ifndef GUI_H
#define GUI_H

#include <math.h>
#include <gtk/gtk.h>
#include <cairo.h>

#include "vlb.h"

#define VLB_UPDATE_FREQUENCY 10

#define VLB_LED_SIZE 6.0

/* Colours of the lights on each column of lights */
static const int led_r[] = {1.0, 1.0, 0.0, 0.0};
static const int led_g[] = {0.0, 1.0, 1.0, 0.0};
static const int led_b[] = {0.0, 0.0, 0.0, 1.0};

/* Light Column X-Positions */
static const double led_x[] = {10.0, 30.0};
static const double led_y[] = {10.0, 30.0, 50.0, 70.0};

/* LED Bits */
static const uint8_t led_bits[] = {0x04, 0x02, 0x01, 0x08,
                                   0x40, 0x20, 0x10, 0x80};

int main(int argc, char *argv[]);

#endif

