/* See LICENSE file for license details. */
#define _XOPEN_SOURCE 500

#include <err.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

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
	printf("usage: wtfc [undslwrefN]\n");
	exit(0);
}

int determine_closest_win_in_dir(XWindowAttributes *other_attr,
	Window *other_win, XWindowAttributes *cur_attr, int *closest_win,
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
	/* function was not given a valid direction */
	} else {
		return 1;
	}

	return 0;
}

int get_closest_win_in_dir(Window *closest_return, int dir) {

	if (!(dpy = XOpenDisplay(0))) {
		exit(1);
	} else {
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
				if (determine_closest_win_in_dir(&temp_win_attrib,
					&temp_window, &cur_win_attrib, &closest_window,
					&closest_dist, dir) != 0) {
					return 1;
				}
			}
		}

		/* If there is any window in the given direction */
		if (closest_window != -1) {
			/* return the window id (in decimal) of the closest window */
			*closest_return = closest_window;
			return 0;
		}
	}

	*closest_return = -1;
	return 1;
}

/*
* argc is the number of WORDS in the call to the wtfc
* argv contains a bunch of info about the call. The shell, the terminal,
* all that jazz. This is what we want.
*/
int main(int argc, char **argv) {

	/* If the call to wtfc has 2 WORDS (wtfc and the direction) */
	if (argc == 2) {

		int cardinal = -1;
		Window closest_win_ret = -1;

		for (int j = 0; j < strlen(argv[1]); j++) {
			switch (argv[1][j]) {
				case 'u': cardinal = NORTH; break;
				case 'n': cardinal = NORTH; break;
				case 'r': cardinal = EAST; break;
				case 'e': cardinal = EAST; break;
				case 'd': cardinal = SOUTH; break;
				case 's': cardinal = SOUTH; break;
				case 'l': cardinal = WEST; break;
				case 'w': cardinal = WEST; break;
				case 'f': focus = 1; break;
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
			printf("No valid direction given\n");
			print_help();
			return 1;
		} else {
			/* If there is any window in the given cardinal direction */
			if (!get_closest_win_in_dir(&closest_win_ret, cardinal)) {
				if (focus == 1) {
					int s = XSetInputFocus(dpy, closest_win_ret,
						RevertToParent, CurrentTime);
					XSync(dpy, False);
				}
				printf("0x%08x\n", closest_win_ret);
			} else {
				/* If the 'c' option flag wasn't used, if a Window wasn't
				 * found, then wtfc prints out the Window currently focused
				 */
				if (stop == 1) {
					Window current_window;
					int focus_status;
					XGetInputFocus(dpy, &current_window, &focus_status);
					printf("0x%08x\n", current_window);
				} else {
					return 1;
				}
			}
		}
	} else {
		print_help();
		exit(1);
	}

	return 0;
}
