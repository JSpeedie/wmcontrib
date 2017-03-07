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
/* Local includes */
#include "common.h"

Display *dpy;
int compatible = 0;
int focus = 0;
int stop = 1;

/* Constants to represent the cardinal directions */
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int AMBIGUOUS = 3;

void print_help(void) {
	fprintf(stderr, "usage: wtfc -[undslwre] -[fNc]\n");
}

/** Returns an int of 1 if the direction given was not valid, otherwise
 * it returns an int of 0. Takes an information about an 'other' window,
 * a 'cur' window and a direction. Sets closest_win to other win if the
 * distance in a given cardinal direction between 'other' and 'cur'
 * is less than closest_dist. Sets closest_dist to the distance between
 * the two windows as well.
 * *other_attr pointer to an XWindowAttributes representing the window
 *     to run the check on.
 * *other_win pointer to the other window to run the check on. *other_attr
 *     should be the attributes of *other_win.
 * *cur_attr pointer to an XWindowAttributes representing the base window
 *     of the check.
 * *closest_win sets the value of this int pointer to *other_win if it is
 *     closer to 'cur' than closest_dist
 * *closest_dist. If the distance between 'cur' and 'other' in the given dir
 *     less than closest_dist, closest_dist will be set to that distance.
 * dir an int representing the direction to check the distance in.
 * Returns 0 if a valid 'dir' was given. Otherwise it returns 1.
 */
int determine_closest_win_in_dir(XWindowAttributes *other_attr,
	Window *other_win, XWindowAttributes *cur_attr, Window *closest_win,
	int *closest_dist, int dir) {

	if (dir == EAST) {
		/* If the temp window is more easternly */
		if (other_attr->x > cur_attr->x) {
			/* If the temp window is closer to the cur window */
			/* than the current closest win in the given dir */
			if ((other_attr->x - cur_attr->x) < *closest_dist) {
				*closest_win = *other_win;
				*closest_dist = other_attr->x - cur_attr->x;
			}
		}
	} else if (dir == WEST) {
		/* If the temp window is more westernly */
		if (other_attr->x < cur_attr->x) {
			/* If the temp window is closer to the cur window */
			/* than the current closest win in the given dir */
			if ((cur_attr->x - other_attr->x) < *closest_dist) {
				*closest_win = *other_win;
				*closest_dist = cur_attr->x - other_attr->x;
			}
		}
	} else if (dir == SOUTH) {
		/* If the temp window is more southernly */
		if (other_attr->y > cur_attr->y) {
			/* If the temp window is closer to the cur window */
			/* than the current closest win in the given dir */
			if ((other_attr->y - cur_attr->y) < *closest_dist) {
				*closest_win = *other_win;
				*closest_dist = other_attr->y - cur_attr->y;
			}
		}
	} else if (dir == NORTH) {
		/* If the temp window is more northernly */
		if (other_attr->y < cur_attr->y) {
			/* If the temp window is closer to the cur window */
			/* than the current closest win in the given dir */
			if ((cur_attr->y - other_attr->y) < *closest_dist) {
				*closest_win = *other_win;
				*closest_dist = cur_attr->y - other_attr->y;
			}
		}
	} else {
		return ERR_INVALID_DIRECTION;
	}

	return 0;
}

int get_closest_win_in_dir(Window *closest_return, int dir) {

	if (!(dpy = XOpenDisplay(0))) { return ERR_COULDNT_OPEN_X_DISPLAY; }

	int screen = DefaultScreen(dpy);
	Window w = RootWindow(dpy, screen);
	Window root_return;
	Window parent_return;
	Window *children_return;
	unsigned int number_children;
	/* Get currently focused window for comparison. */
	Window current_window;
	int focus_status;
	XGetInputFocus(dpy, &current_window, &focus_status);
	/* Get a list of all the windows in X session*/
	XQueryTree(dpy, w, &root_return, &parent_return, &children_return, \
		&number_children);
	int closest_dist = INT_MAX;
	int ret = 0;
	unsigned long closest_window = -1;
	XWindowAttributes cur_win_attrib;
	XGetWindowAttributes(dpy, current_window, &cur_win_attrib);

	/* Go through list of windows */
	for (int i = 0; i < number_children; i++) {
		Window temp_window = *(children_return + i);
		XWindowAttributes temp_win_attrib;
		XGetWindowAttributes(dpy, temp_window, &temp_win_attrib);

		/* If the window is mapped (visible) */
		if (temp_win_attrib.map_state == 2 && temp_window != current_window) {
			if ((ret = determine_closest_win_in_dir(&temp_win_attrib, &temp_window,
				&cur_win_attrib, &closest_window, &closest_dist, dir)) != 0) {
				return ret;
			}
		}
	}

	/* If there is any window in the given direction */
	if (closest_window != -1) {
		/* return the window id (in decimal) of the closest window */
		*closest_return = closest_window;
		return 0;
	}

	*closest_return = -1;
	return 23489;
}

/*
* argc is the number of WORDS in the call to the wtfc
* argv contains a bunch of info about the call. The shell, the terminal,
* all that jazz. This is what we want.
*/
int main(int argc, char **argv) {

	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "wtfc: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}
	Window closest_win_ret = -1;
	int cardinal = -1;

	int opt;
	struct option opt_table[] = {
		/* cardinal direction arguments */
		{ "up",			no_argument,	NULL,	'u' },
		{ "north",		no_argument,	NULL,	'n' },
		{ "down",		no_argument,	NULL,	'd' },
		{ "south",		no_argument,	NULL,	's' },
		{ "left",		no_argument,	NULL,	'l' },
		{ "west",		no_argument,	NULL,	'w' },
		{ "right",		no_argument,	NULL,	'r' },
		{ "east",		no_argument,	NULL,	'e' },
		/* fine functionality options */
		{ "focus",		no_argument,	NULL,	'f' },
		{ "focus-none",	no_argument,	NULL,	'N' },
		{ "continue",	no_argument,	NULL,	'c' },
		{ 0, 0, 0, 0 }
	};

	while ((opt = getopt_long(argc, argv, \
		"undslwrefNc", opt_table, NULL)) != -1) {
		switch (opt) {
			/* cardinal direction arguments */
			case 'u': cardinal = NORTH; break;
			case 'n': cardinal = NORTH; break;
			case 'r': cardinal = EAST; break;
			case 'e': cardinal = EAST; break;
			case 'd': cardinal = SOUTH; break;
			case 's': cardinal = SOUTH; break;
			case 'l': cardinal = WEST; break;
			case 'w': cardinal = WEST; break;
			/* fine functionality options */
			case 'f': focus = 1; break;
			/* "Works" but not necessarily as intended. Being considered
			 * for removal */
			case 'N':
				XSetInputFocus(dpy, None, RevertToNone, CurrentTime);
				XSync(dpy, False);
				exit(0);
				break;
			case 'c':  stop = 0; break;
		}
	}

	/* If a direction was not specified, then print usage help */
	if (cardinal == -1) {
		fprintf(stderr, "wtfc: Not given a cardinal direction\n");
		print_help();
		exit(ERR_CARDINAL_NOT_GIVEN);
	} else {
		int ret = 0;

		if ((ret = get_closest_win_in_dir(&closest_win_ret, cardinal)) != 0) {
			if (ret == ERR_COULDNT_OPEN_X_DISPLAY) {
				fprintf(stderr, "wtfc: Couldn't open X display\n");
				exit(ret);
			} else if (ret == ERR_NO_VALID_DIR) {
				fprintf(stderr, "wtfc: Given invalid direction.\n");
				exit(ret);
			}

			/* If the 'c' option flag wasn't used, if a Window wasn't
			 * found, then wtfc prints out the Window currently focused
			 */
			if (stop == 1) {
				Window current_window;
				int focus_status;
				XGetInputFocus(dpy, &current_window, &focus_status);
				printf("0x%08x\n", current_window);
			} else {
				exit(-1);
			}
		/* If there is a window in the given cardinal direction */
		} else {
			if (focus == 1) {
				XSetInputFocus(dpy, closest_win_ret, RevertToParent, \
					CurrentTime);
				XSync(dpy, False);
			}
			printf("0x%08x\n", closest_win_ret);
		}
	}

	return 0;
}
