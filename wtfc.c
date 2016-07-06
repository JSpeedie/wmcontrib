/* See LICENSE file for license details. */
#define _XOPEN_SOURCE 500

#include <err.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

Display *dpy;
int focus = 0;
int compatible = 0;
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

int window_compare_dir(Window window1, Window window2, int *direction, int *dist) {

	/* Get center-of-window coordinates for first window */
	XWindowAttributes window1_attrib;
	XGetWindowAttributes(dpy, window1, &window1_attrib);
	int window1_center_x = window1_attrib.x + (window1_attrib.width / 2);
	int window1_center_y = window1_attrib.y + (window1_attrib.height / 2);

	/* Get center-of-window coordinates for first window */
	XWindowAttributes window2_attrib;
	XGetWindowAttributes(dpy, window2, &window2_attrib);
	int window2_center_x = window2_attrib.x + (window2_attrib.width / 2);
	int window2_center_y = window2_attrib.y + (window2_attrib.height / 2);

	int diff_x = window1_center_x - window2_center_x;
	int diff_y = window1_center_y - window2_center_y;


	/*
	 * If the difference in the y between the 2 windows is greater than the
	 * difference in the x then the second window is located in either the
	 * NORTH or SOUTH cardinal direction.
	 */
	if (abs(diff_y) > abs(diff_x)) {
		/* If the y diff is positive. The cardinal direction is north */
		if (diff_y > 0) {
			*direction = NORTH;
			*dist = (int) sqrt((double) (diff_y * diff_y) + (double) (diff_x * diff_x));
			return 0;
		}
		else if (diff_y < 0) {
			*direction = SOUTH;
			*dist = (int) sqrt((double) (diff_y * diff_y) + (double) (diff_x * diff_x));
			return 0;
		}
		/* Should never be triggered but is here for completion's sake */
		else {
			*direction = -1;
			*dist = (int) sqrt((double) (diff_y * diff_y) + (double) (diff_x * diff_x));
			return -1;
		}
	}
	/*
	 * If the difference in the x between the 2 windows is greater than or
	 * equal the difference in the y then the second window is located in either the
	 * EAST or WEST cardinal direction.
	 */
	else {
		/* If the x diff is positive. The cardinal direction is west */
		if (diff_x > 0) {
			*direction = WEST;
			*dist = (int) sqrt((double) (diff_y * diff_y) + (double) (diff_x * diff_x));
			return 0;
		}
		else if (diff_x < 0) {
			*direction = EAST;
			*dist = (int) sqrt((double) (diff_y * diff_y) + (double) (diff_x * diff_x));
			return 0;
		}
		/* Triggered if the 2 windows have centers of the same coordinates */
		else {
			*direction = AMBIGUOUS;
			*dist = (int) sqrt((double) (diff_y * diff_y) + (double) (diff_x * diff_x));
			return 0;
		}
	}
}

int get_closest_dir(Window *closest_return, int dir) {

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
		Window currently_focused;
		int focus_status;
		XGetInputFocus(dpy, &currently_focused, &focus_status);
		/* Get a list of all the windows in X session*/
		XQueryTree(dpy, w, &root_return, &parent_return, &children_return, \
			&number_children);
		int closest_dist = INT_MAX;
		int closest_window = -1;
	
		/* Go through list of windows */
		for (int i = 0; i < number_children; i++) {
			Window temp_window = *(children_return + i);
			XWindowAttributes attrib;
			XGetWindowAttributes(dpy, temp_window, &attrib);
	
			/* If the window is mapped (visible) */
			if (attrib.map_state == 2 && temp_window != currently_focused) {
				int distance;
				int compare_dir;
				// Get whether window is up, down, left or right
				int temp_window_dir;
				window_compare_dir(currently_focused, temp_window, \
					&temp_window_dir, &distance);
					// printf("%dx%x \n", temp_window_dir, temp_window);
				
				if (temp_window_dir == dir) {
					// printf("---%x temp_window dir vs dir %d %d\n", temp_window, temp_window_dir, dir);
					/* If this is the closest window it's come across so far */
					if (distance < closest_dist) {
						// printf("closer window in %d %x\n", temp_window_dir, temp_window);
						/* Remember the closest window in the given direction */
						closest_window = temp_window;
						closest_dist = distance;
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
		}
		else {
			get_closest_dir(&closest_window_ret, cardinal);
			/* If there is any window in the given cardinal direction */
			if (closest_window_ret != -1) {
				if (focus == 1) {
					int s = XSetInputFocus(dpy, closest_window_ret, RevertToParent, CurrentTime);
					XSync(dpy, False);
				}
				/* wmutils window id formatting */
				unsigned long int long_win_id = (unsigned long int) closest_window_ret;
				printf("0x%08x\n", long_win_id);
			}
			else {
				printf("no valid window found\n");
			}
		}
	}
	else {
		print_help();
	}
	
	return 0;
}
