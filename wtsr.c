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
int focus = 0;
int compatible = 0;
int use_x = 0;
int use_y = 0;
int window_given = 0;
int use_anchors = 0;
int offset_borders = 0;
int print_results = 1;
int move_after = 0;

void print_help(void) {
	fprintf(stderr, "usage: wtsr -[xy] <ratio> -w <wid> [OPTIONS]\n");
}

/** Returns an int of 0 if it succeeds in finding the new location for 'win'
 * or an int representing the error it ran into.
 * win the Window to be moved.
 * rel_x the ratio of the screen's width to return as '*return_x'.
 * rel_y the ratio of the screen's height to return as '*return_y'.
 * *return_x pointer to an int to be filled with  the new x value of 'win'
 *     after it has been moved to the given x ratio of the screen.
 * *return_y pointer to an int to be filled with  the new y value of 'win'
 *     after it has been moved to the given y ratio of the screen.
 * Returns 0 if it could find the screen 'win' belongs, if 'win' exists, and
 * if an X display could be opened.
 * Returns the correct error code for the error it encountered.
 */
int get_coord_for_relative_location(Window win, double rel_x, double rel_y,
	int *return_x, int *return_y) {

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
		*return_x = (rel_x)*(screen_info->width)
			- (rel_x)*2*(win_attrib.border_width)
			- (rel_x)*(win_attrib.width)
			+ screen_info->x;
		*return_y = (rel_y)*(screen_info->height)
			- (rel_y)*2*(win_attrib.border_width)
			- (rel_y)*(win_attrib.height)
			+ screen_info->y;
	/* If not set to 1, then (if the window has borders), it can result
	 * in 1 border width of the window off the screen + the outer border. */
	} else {
		*return_x = (rel_x)*(screen_info->width)
			- (rel_x)*(win_attrib.width)
			+ screen_info->x;
		*return_y = (rel_y)*(screen_info->height)
			- (rel_y)*(win_attrib.height)
			+ screen_info->y;
	}

	return 0;
}

int main(int argc, char **argv) {

	double relative_x = 0;
	double relative_y = 0;
	unsigned long int win_id = 0;
	int ret_x = 0;
	int ret_y = 0;

	int opt;
	struct option opt_table[] = {
		/* relative to screen arguments */
		{ "x-ratio",		required_argument,	NULL,	'x' },
		{ "y-ratio",		required_argument,	NULL,	'y' },
		/* window id argument */
		{ "window-id",		required_argument,	NULL,	'w' },
		/* fine functionality options */
		{ "use-anchor",		no_argument,	NULL,	'a' },
		{ "offset-border",	no_argument,	NULL,	'b' },
		{ "no-print",		no_argument,	NULL,	'N' },
		{ "move-window",	no_argument,	NULL,	'm' },
		{ 0, 0, 0, 0 }
	};

	while ((opt = getopt_long(argc, argv, \
		"x:y:w:abNm", opt_table, NULL)) != -1) {
		switch (opt) {
			/* relative to screen options */
			case 'x':
				use_x = 1;
				/* check if optarg is 0-1 */
				relative_x = strtod(&optarg[0], NULL);
				break;
			case 'y':
				use_y = 1;
				relative_y = strtod(&optarg[0], NULL);
				break;
			/* window id argument */
			case 'w':
				window_given = 1;
				win_id = strtoul(&optarg[0], NULL, 0);
				break;
			/* fine functionality options */
			case 'a': use_anchors = 1; break;
			case 'b': offset_borders = 1; break;
			case 'N': print_results = 0; break;
			case 'm': move_after = 1; break;
		}
	}

	if (use_x == 0 && use_y == 0) {
		fprintf(stderr, "wtsr: Neither an x nor y ratio were given.\n");
		exit(ERR_NEITHER_RATIO_GIVEN);
	}
	if (window_given == 0) {
		fprintf(stderr, "wtsr: Window ID was not given.\n");
		exit(ERR_WINDOW_NOT_GIVEN);
	}
	if (window_exists(win_id) != 0) {
		fprintf(stderr, "wtsr: Could not find window of given id.\n");
		exit(ERR_WIN_NOT_FOUND);
	}

	Display *dpy = XOpenDisplay(0);
	XWindowAttributes win_attrib;
	XGetWindowAttributes(dpy, win_id, &win_attrib);

	/* Check if the program was given valid relative x and relative y values */
	if (relative_x > 1 || relative_x < 0) {
		fprintf(stderr, "wtsr: Relative x value provided was not within 0 <= value <= 1\n");
		print_help();
		exit(ERR_RATIO_NOT_IN_RANGE);
	} else if (relative_y > 1 || relative_y < 0) {
		fprintf(stderr, "wtsr: Relative y value provided was not within 0 <= value <= 1\n");
		print_help();
		exit(ERR_RATIO_NOT_IN_RANGE);
	/* If the program was given valid parameters for those variables */
	} else {
		int ret = 0;

		if ((ret = get_coord_for_relative_location(win_id, relative_x, relative_y,
			&ret_x, &ret_y)) != 0) {
			if (ret == ERR_COULDNT_OPEN_X_DISPLAY) {
				fprintf(stderr, "wtsr: Couldn't open X display\n");
			} else if (ret == ERR_WIN_NOT_FOUND) {
				fprintf(stderr, "wtsr: Could not find window of given id.\n");
			} else if (ret == ERR_SCREEN_OF_WIN_NOT_FOUND) {
				fprintf(stderr, "wtsr: Could not find the screen the given window resides in.\n");
			}
			exit(ret);
		}

		/* option flags that take effect near the end of the process */
		if (print_results == 1) {
			printf("%d %d %d %d 0x%08x\n", ret_x, ret_y, win_attrib.width, \
				win_attrib.height, win_id);
		}
		if (move_after == 1) {
			XMoveWindow(dpy, win_id, ret_x, ret_y);
			XSync(dpy, False);
		}

		return 0;
	}
}
