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
int use_w = 0;
int use_h = 0;
int window_given = 0;
int print_results = 1;
int resize_after = 0;

void print_help(void) {
	fprintf(stderr, "usage: wrsz -[wh] <pixels> -W <wid> [OPTIONS]\n");
}

int main(int argc, char **argv) {

	int given_w = 0;
	int given_h = 0;
	unsigned long int win_id = 0;
	int final_w = 0;
	int final_h = 0;

	int opt;
	struct option opt_table[] = {
		/* relative to screen arguments */
		{ "w",		required_argument,	NULL,	'w' },
		{ "h",		required_argument,	NULL,	'h' },
		/* window id argument */
		{ "window-id",		required_argument,	NULL,	'W' },
		/* fine functionality options */
		{ "no-print",		no_argument,	NULL,	'N' },
		{ "resize-window",	no_argument,	NULL,	'r' },
		{ 0, 0, 0, 0 }
	};

	while ((opt = getopt_long(argc, argv, \
		"w:h:W:Nr", opt_table, NULL)) != -1) {
		switch (opt) {
			/* relative to screen options */
			case 'w':
				use_w = 1;
				given_w = strtod(&optarg[0], NULL);
				break;
			case 'h':
				use_h = 1;
				given_h = strtod(&optarg[0], NULL);
				break;
			/* window id argument */
			case 'W':
				window_given = 1;
				win_id = strtoul(&optarg[0], NULL, 0);
				break;
			/* fine functionality options */
			case 'N': print_results = 0; break;
			case 'r': resize_after = 1; break;
		}
	}

	if (use_w == 0 && use_h == 0) {
		fprintf(stderr, "wrsz: Neither a width nor height were given.\n");
		print_help();
		exit(ERR_NEITHER_RATIO_GIVEN);
	}
	if (window_given == 0) {
		fprintf(stderr, "wrsz: Window ID was not given.\n");
		print_help();
		exit(ERR_WINDOW_NOT_GIVEN);
	}
	if (window_exists(win_id) != 0) {
		fprintf(stderr, "wrsz: Could not find window of given id.\n");
		exit(ERR_WIN_NOT_FOUND);
	}

	Display *dpy;
	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "wrsz: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}
	XWindowAttributes win_attrib;
	XGetWindowAttributes(dpy, win_id, &win_attrib);

	/* Only change the width if given a new width to set it to*/
	if (use_w) {
		final_w = win_attrib.width + given_w;
	} else {
		final_w = win_attrib.width;
	}
	/* Only change the height if given a new height to set it to*/
	if (use_h) {
		final_h = win_attrib.height + given_h;
	} else {
		final_h = win_attrib.height;
	}

	/* You cannot have a window of 0 width or height. */
	if (final_w <= 0) { final_w = 1; }
	if (final_h <= 0) { final_h = 1; }

	/* option flags that take effect near the end of the process */
	if (print_results == 1) {
		printf("%d %d %d %d 0x%08x\n", win_attrib.x, win_attrib.y, \
			final_w, final_h, win_id);
	}
	if (resize_after == 1) {
		XMoveResizeWindow(dpy, win_id, win_attrib.x, win_attrib.y, \
			final_w, final_h);
		XSync(dpy, False);
	}

	return 0;
}
