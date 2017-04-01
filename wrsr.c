/* See LICENSE file for license details. */
#define _XOPEN_SOURCE 500

#include <err.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
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
int use_width = 0;
int use_height = 0;
int window_given = 0;
int use_anchors = 0;
int offset_borders = 0;
int print_results = 1;
int resize_after = 0;

void print_help(void) {
	fprintf(stderr, "usage: wrsr -[xy] <ratio> -w <wid> [OPTIONS]\n");
}

/** Returns an int of 0 if it succeeds in finding the new size for 'win'
 * or an int representing the error it ran into.
 * win the Window to be moved.
 * rel_x the ratio of the screen's width to return as '*return_w'.
 * rel_y the ratio of the screen's height to return as '*return_h'.
 * *return_w pointer to an int to be filled with  the new width value of 'win'
 *     after it has been resized to the given x ratio of the screen.
 * *return_h pointer to an int to be filled with  the new height value of 'win'
 *     after it has been resized to the given y ratio of the screen.
 * Returns 0 if it could find the screen 'win' belongs, if 'win' exists, and
 * if an X display could be opened.
 * Returns the correct error code for the error it encountered.
 */
int get_size_for_relative_size(Window win, double rel_x, double rel_y,
	int *return_w, int *return_h) {

	XWindowAttributes win_attrib;
	int win_belong_to;
	int ret = 0;

	/* Error checking */
	if (!(dpy = XOpenDisplay(0))) { return ERR_COULDNT_OPEN_X_DISPLAY; }
	if ((ret = window_exists(win)) != 0) { return ret; }
	if ((ret = get_screen_number_of_win(win, dpy, use_anchors, &win_belong_to)) != 0) {
		return ret;
	}

	XGetWindowAttributes(dpy, win, &win_attrib);
	XRRScreenResources *screen_res;
	XRRCrtcInfo *screen_info;

	screen_res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
	/* 0 to get the first monitor */
	screen_info = \
		XRRGetCrtcInfo(dpy, screen_res, screen_res->crtcs[win_belong_to]);

	/* option flag for changing output to compensate for border sizes */
	if (offset_borders == 1) {
		*return_w = (rel_x)*(screen_info->width)
			- (rel_x)*2*(win_attrib.border_width);
		*return_h = (rel_y)*(screen_info->height)
			- (rel_y)*2*(win_attrib.border_width);
	/* If not set to 1, then (if the window has borders), it can result
	 * in 1 border-width of the window off the screen + the outer border. */
	} else {
		*return_w = (rel_x)*(screen_info->width);
		*return_h = (rel_y)*(screen_info->height);
	}

	return 0;
}

int main(int argc, char **argv) {

	double relative_width = 0;
	double relative_height = 0;
	unsigned long int win_id = 0;
	int ret_w = 0;
	int ret_h = 0;
	int final_w = 0;
	int final_h = 0;

	int opt;
	struct option opt_table[] = {
		/* relative to screen arguments */
		{ "width-ratio",		required_argument,	NULL,	'w' },
		{ "height-ratio",		required_argument,	NULL,	'h' },
		/* window id argument */
		{ "window-id",		required_argument,	NULL,	'W' },
		/* fine functionality options */
		{ "use-anchor",		no_argument,	NULL,	'a' },
		{ "offset-border",	no_argument,	NULL,	'b' },
		{ "no-print",		no_argument,	NULL,	'N' },
		{ "resize-window",	no_argument,	NULL,	'r' },
		{ 0, 0, 0, 0 }
	};

	while ((opt = getopt_long(argc, argv, \
		"w:h:W:abNr", opt_table, NULL)) != -1) {
		switch (opt) {
			/* relative to screen options */
			case 'w':
				use_width = 1;
				/* check if optarg is 0-1 */
				relative_width = strtod(&optarg[0], NULL);
				break;
			case 'h':
				use_height = 1;
				relative_height = strtod(&optarg[0], NULL);
				break;
			/* window id argument */
			case 'W':
				window_given = 1;
				win_id = strtoul(&optarg[0], NULL, 0);
				break;
			/* fine functionality options */
			case 'a': use_anchors = 1; break;
			case 'b': offset_borders = 1; break;
			case 'N': print_results = 0; break;
			case 'r': resize_after = 1; break;
		}
	}

	if (use_width == 0 && use_height == 0) {
		fprintf(stderr, "wrsr: Neither an x nor y ratio were given.\n");
		print_help();
		exit(ERR_NEITHER_RATIO_GIVEN);
	}
	if (window_given == 0) {
		fprintf(stderr, "wrsr: Window ID was not given.\n");
		print_help();
		exit(ERR_WINDOW_NOT_GIVEN);
	}
	if (window_exists(win_id) != 0) {
		fprintf(stderr, "wrsr: Could not find window of given id.\n");
		exit(ERR_WIN_NOT_FOUND);
	}

	Display *dpy;
	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "wrsr: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}
	XWindowAttributes win_attrib;
	XGetWindowAttributes(dpy, win_id, &win_attrib);

	/* Check if the program was given valid relative x and relative y values */
	if (relative_width > 1 || relative_width < 0) {
		fprintf(stderr, "wrsr: Relative width value provided was not within 0 <= value <= 1\n");
		exit(ERR_RATIO_NOT_IN_RANGE);
	} else if (relative_height > 1 || relative_height < 0) {
		fprintf(stderr, "wrsr: Relative height value provided was not within 0 <= value <= 1\n");
		exit(ERR_RATIO_NOT_IN_RANGE);
	/* If the program was given valid parameters for those variables */
	} else {
		int ret = 0;

		if ((ret = get_size_for_relative_size(win_id, relative_width, relative_height,
			&ret_w, &ret_h)) != 0) {
			if (ret == ERR_COULDNT_OPEN_X_DISPLAY) {
				fprintf(stderr, "wrsr: Couldn't open X display\n");
			} else if (ret == ERR_WIN_NOT_FOUND) {
				fprintf(stderr, "wrsr: Could not find window of given id.\n");
			} else if (ret == ERR_SCREEN_OF_WIN_NOT_FOUND) {
				fprintf(stderr, "wrsr: Could not find the screen the given window resides in.\n");
			}
			exit(ret);
		}

		/* Only change the width if given a width ratio to resize it to */
		if (use_width) {
			final_w = ret_w;
		}
		else {
			final_w = win_attrib.width;
		}
		/* Only change the height if given a height ratio to resize it to */
		if (use_height) {
			final_h = ret_h;
		}
		else {
			final_h = win_attrib.height;
		}

		/* You cannot have a window of 0 width or height. */
		if (final_w == 0) { final_w = 1; }
		if (final_h == 0) { final_h = 1; }

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
}
