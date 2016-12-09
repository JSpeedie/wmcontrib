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
int focus = 0;
int compatible = 0;

/* Constants to represent the cardinal directions */
int NORTH = 0;
int EAST = 1;
int SOUTH = 2;
int WEST = 3;
int AMBIGUOUS = 3;

void print_help(void) {
	printf("wtfc\n\tusage: " \
		"[ -a ]");

	printf("\n\n\t-a, --thiswillbeanoptionflag variable\n\t\tMight be a thing.");

	printf("\n");
	exit(0);
}

int get_closest_win_in_dir(Window *closest_return, int dir) {

	if (!(dpy = XOpenDisplay(0))) {
		exit(1);
	}
	else {
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
		int closest_window = -1;
		XWindowAttributes cur_win_attrib;
		XGetWindowAttributes(dpy, current_window, &cur_win_attrib);

		/* Go through list of windows */
		for (int i = 0; i < number_children; i++) {
			Window temp_window = *(children_return + i);
			XWindowAttributes temp_win_attrib;
			XGetWindowAttributes(dpy, temp_window, &temp_win_attrib);

			/* If the window is mapped (visible) */
			if (temp_win_attrib.map_state == 2 && temp_window != current_window) {

				if (dir == EAST) {
					/* If the temp window is more easternly */
					if (temp_win_attrib.x > cur_win_attrib.x) {
						/* If the temp window is closer to the cur window */
						/* than the current closest win in the given dir */
						if ((temp_win_attrib.x - cur_win_attrib.x) < closest_dist) {
							closest_window = temp_window;
							closest_dist = temp_win_attrib.x - cur_win_attrib.x;
						}
					}
				}
				if (dir == WEST) {
					/* If the temp window is more westernly */
					if (temp_win_attrib.x < cur_win_attrib.x) {
						/* If the temp window is closer to the cur window */
						/* than the current closest win in the given dir */
						if ((cur_win_attrib.x - temp_win_attrib.x) < closest_dist) {
							closest_window = temp_window;
							closest_dist = cur_win_attrib.x - temp_win_attrib.x;
						}
					}
				}
				if (dir == SOUTH) {
					/* If the temp window is more southernly */
					if (temp_win_attrib.y > cur_win_attrib.y) {
						/* If the temp window is closer to the cur window */
						/* than the current closest win in the given dir */
						if ((temp_win_attrib.y - cur_win_attrib.y) < closest_dist) {
							closest_window = temp_window;
							closest_dist = temp_win_attrib.y - cur_win_attrib.y;
						}
					}
				}
				if (dir == NORTH) {
					/* If the temp window is more northernly */
					if (temp_win_attrib.y < cur_win_attrib.y) {
						/* If the temp window is closer to the cur window */
						/* than the current closest win in the given dir */
						if ((cur_win_attrib.y - temp_win_attrib.y) < closest_dist) {
							closest_window = temp_window;
							closest_dist = cur_win_attrib.y - temp_win_attrib.y;
						}
					}
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
int
main(int argc, char **argv) {

	int cardinal = -1;
	/* If the call to wtfc has 2 WORDS (wtfc and the direction) */
	if (argc == 2) {
		Window closest_window_ret = -1;

		for (int j = 0; j < strlen(argv[1]); j++) {
			switch (argv[1][j]) {
				case 'u':
					cardinal = NORTH; break;
				case 'n':
					cardinal = NORTH; break;
				case 'r':
					cardinal = EAST; break;
				case 'e':
					cardinal = EAST; break;
				case 'd':
					cardinal = SOUTH; break;
				case 's':
					cardinal = SOUTH; break;
				case 'l':
					cardinal = WEST; break;
				case 'w':
					cardinal = WEST; break;
				case 'f':
					focus = 1; break;
				case 'N':
					XSetInputFocus(dpy, None, RevertToNone, CurrentTime);
					XSync(dpy, False);
					exit(0);
					break;
			}
		}


		/* If a direction was not specified, then print usage help */
		if (cardinal == -1) {
			printf("no valid direction given\n");
			print_help();
			return 1;
		}
		else {
			int win_in_dir_ret = get_closest_win_in_dir(&closest_window_ret, cardinal);
			/* If there is any window in the given cardinal direction */
			if (win_in_dir_ret == 0) {
				if (focus == 1) {
					int s = XSetInputFocus(dpy, closest_window_ret, RevertToParent, CurrentTime);
					XSync(dpy, False);
				}
				/* wmutils window id formatting */
				unsigned long int long_win_id = (unsigned long int) closest_window_ret;
				printf("0x%08x\n", long_win_id);
			}
			else {
				return 1;
			}
		}
	}
	else {
		print_help();
		return 1;
	}

	return 0;
}
