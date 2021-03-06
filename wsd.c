/* See LICENSE file for license details. */
#define _XOPEN_SOURCE 500

#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
/* Local includes */
#include "common.h"

Display *dpy;
int use_x = 0;
int use_y = 0;
int use_width = 0;
int use_height = 0;
int window_given = 0;
int offset_borders = 0;
int print_results = 1;
int make_changes = 0;
int contain = 0;

void print_help(void) {
	fprintf(stderr, "usage: wsd -[xy] <new_location> -w <wid> [OPTIONS]\n");
}

int main(int argc, char **argv) {

	int given_x = 0;
	int given_y = 0;
	int given_width = 0;
	int given_height = 0;
	unsigned long int win_id = 0;
	int final_x = 0;
	int final_y = 0;
	int final_width = 0;
	int final_height = 0;

	int opt;
	struct option opt_table[] = {
		/* Location arguments */
		{ "x",		required_argument,	NULL,	'x' },
		{ "y",		required_argument,	NULL,	'y' },
		/* Dimensionarguments */
		{ "width",		required_argument,	NULL,	'w' },
		{ "height",		required_argument,	NULL,	'h' },
		/* window id argument */
		{ "window-id",		required_argument,	NULL,	'W' },
		/* fine functionality options */
		{ "no-print",		no_argument,	NULL,	'N' },
		{ "offset-border",	no_argument,	NULL,	'b' },
		{ "make-changes",	no_argument,	NULL,	'm' },
		{ "contain",		no_argument,	NULL,	'c' },
		{ 0, 0, 0, 0 }
	};

	while ((opt = getopt_long(argc, argv, \
		"x:y:w:h:W:Nbmc", opt_table, NULL)) != -1) {
		switch (opt) {
			/* location options */
			case 'x':
				use_x = 1;
				given_x = strtod(&optarg[0], NULL);
				break;
			case 'y':
				use_y = 1;
				given_y = strtod(&optarg[0], NULL);
				break;
			/* location options */
			case 'w':
				use_width = 1;
				given_width = strtod(&optarg[0], NULL);
				break;
			case 'h':
				use_height = 1;
				given_height = strtod(&optarg[0], NULL);
				break;
			/* window id argument */
			case 'W':
				window_given = 1;
				win_id = strtoul(&optarg[0], NULL, 0);
				break;
			/* fine functionality options */
			case 'b': offset_borders = 1; break;
			case 'N': print_results = 0; break;
			case 'm': make_changes = 1; break;
			case 'c': contain = 1; break;
		}
	}

	if (use_x == 0 && use_y == 0 && use_width == 0 && use_height == 0) {
		fprintf(stderr, "wsd: Neither an x nor y nor width nor height were given.\n");
		print_help();
		// TODO: create specific error type
		exit(ERR_NEITHER_RATIO_GIVEN);
	}
	if (window_given == 0) {
		fprintf(stderr, "wsd: Window ID was not given.\n");
		print_help();
		exit(ERR_WINDOW_NOT_GIVEN);
	}
	if (window_exists(win_id) != 0) {
		fprintf(stderr, "wsd: Could not find window of given id.\n");
		exit(ERR_WIN_NOT_FOUND);
	}

	Display *dpy;
	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "wsd: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}
	XWindowAttributes win_attrib;
	XGetWindowAttributes(dpy, win_id, &win_attrib);

	if (use_x == 1) final_x = given_x;
	else final_x = win_attrib.x;
	if (use_y == 1) final_y = given_y;
	else final_y = win_attrib.y;
	if (use_width == 1) final_width = given_width;
	else final_width = win_attrib.width;
	if (use_height == 1) final_height = given_height;
	else final_height = win_attrib.height;

	if (contain == 1) {
		int display_x, display_y, display_right, display_bottom;
		if (get_full_display_dims(&display_x, &display_right, &display_y, &display_bottom) == 0) {
			printf("%d %d %d %d \n", display_x, display_right, display_y, display_bottom);
			/* If the window is to be moved outside of the bounds of the display */
			if (final_x < display_x) {
				final_x = display_x;
			} else if ((final_x + win_attrib.width) > display_right) {
				final_x = display_right - win_attrib.width;
				if (offset_borders == 1) {
					final_x -= 2 * win_attrib.border_width;
				}
			}
			if (final_y < display_y) {
				final_y = display_y;
			} else if ((final_y + win_attrib.height) > display_right) {
				final_x = display_right - win_attrib.width;
				if (offset_borders == 1) {
					final_x -= 2 * win_attrib.border_width;
				}
			}
		} else {
			fprintf(stderr, "wsd: Couldn't open X display\n");
			exit(ERR_COULDNT_OPEN_X_DISPLAY);
		}
	}

	/* option flags that take effect near the end of the process */
	if (print_results == 1) {
		printf("%d %d %d %d 0x%08x\n", final_x, final_y, final_width, \
			final_height, win_id);
	}
	if (make_changes == 1) {
		XMoveResizeWindow(dpy, win_id, final_x, final_y, \
			final_width, final_height);
		XSync(dpy, False);
	}

	return 0;
}
