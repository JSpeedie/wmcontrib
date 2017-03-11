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

/* Error codes */
int ERR_COULDNT_OPEN_X_DISPLAY =     1;
int ERR_WIN_NOT_FOUND =              2;
int ERR_SCREEN_OF_WIN_NOT_FOUND =    3;
int ERR_NEITHER_RATIO_GIVEN =        4;
int ERR_WINDOW_NOT_GIVEN =           5;
int ERR_RATIO_NOT_IN_RANGE =         6;
int ERR_NO_VALID_DIR =               7;
int ERR_INVALID_DIRECTION =          8;
int ERR_CARDINAL_NOT_GIVEN =         9;
int ERR_COULDNT_LOC_FOCUS_WIN =      10;
int ERR_NO_WINDOW_IN_DIR =           11;

/* Constants to represent the cardinal directions */
int NORTH = 0;
int EAST = 1;
int SOUTH = 2;
int WEST = 3;
int AMBIGUOUS = 3;

/** Returns an int of 0 if the Window 'win' exists or an int representing
 * the error it ran into.
 * win the Window to be searched for.
 * If the function failed to open an X display or could not find a Window 'win',
 * then it will return the correct error code for window-not-found.
 * If 'win' was found, then the function will return 0.
 */
int window_exists(Window win) {

	Display *display;

	if (!(display = XOpenDisplay(0))) { return ERR_COULDNT_OPEN_X_DISPLAY; }

	int screen = DefaultScreen(display);
	Window root_win = RootWindow(display, screen);
	Window root_ret, parent_ret, *child_ret;
	unsigned int number_children;
	XQueryTree(display, root_win, &root_ret, &parent_ret, &child_ret, \
		&number_children);

	for (int i = 0; i < number_children; i++) {
		Window temp = *(child_ret + i);
		/* 'win' exists, return 0 */
		if (temp == win) {
			return 0;
		}
	}

	return ERR_WIN_NOT_FOUND;
}

/** Returns an int of 0 if it succeeds in finding the screen number of a
 * given Window or an int representing the error it ran into.
 * win the Window we are trying to find the screen of.
 * *dpy pointer to the Display we will be searching on.
 * *return_screen_num pointer to an int where the function will fill in the
 *     screen number that 'win' belongs to, if it can be found.
 * This function will return 0 if it was able to find the screen 'win' belongs
 * to. If it cannot, then it will return the correct
 * error code for window-not-found.
 */
int get_screen_number_of_win(Window win, Display *dpy, int use_anchors, \
	int *return_screen_num) {

	XRRScreenResources *screen_res =
		XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
	XWindowAttributes win_attr;
	XGetWindowAttributes(dpy, win, &win_attr);

	int det_x = 0;
	int det_y = 0;
	int nmonitors = 0;
	XRRGetMonitors(dpy, win, 1, &nmonitors);

	for (int i = 0; i < nmonitors; i++) {
		XRRCrtcInfo *screen_info =
			XRRGetCrtcInfo(dpy, screen_res, screen_res->crtcs[i]);

		/* option flag for using the "anchor" (top left corner)  of a window
		 * to determine what screen it belongs to */
		if (use_anchors == 1) {
			det_x = win_attr.x;
			det_y = win_attr.y;
		/* Use the center of the window to determine what screen it's on */
		} else {
			det_x = win_attr.x + ((win_attr.width)/2);
			det_y = win_attr.y + ((win_attr.height)/2);
		}

		/* If the window is on the ith screen in the x */
		if (det_x >= screen_info->x &&
			det_x < (screen_info->x + screen_info->width)) {
			/* If the window is on the ith screen in the y */
			if (det_y >= screen_info->y &&
				det_y < (screen_info->y + screen_info->height)) {
				*return_screen_num = i;
				return 0;
			}
		}
	}

	/* If the function has not returned yet, then it could not find a screen
	 * on which 'win' resides.
	 */
	return ERR_SCREEN_OF_WIN_NOT_FOUND;
}
