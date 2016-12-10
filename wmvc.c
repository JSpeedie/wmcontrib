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
#include <X11/extensions/Xrandr.h>

Display *dpy;
int focus = 0;
int compatible = 0;

/* Constants to represent the corners of the screen */
static const int TOP_LEFT = 0;
static const int TOP_CENTER = 1;
static const int TOP_RIGHT = 2;
static const int MIDDLE_LEFT = 3;
static const int MIDDLE_CENTER = 4;
static const int MIDDLE_RIGHT = 5;
static const int BOTTOM_LEFT = 6;
static const int BOTTOM_CENTER = 7;
static const int BOTTOM_RIGHT = 8;


void print_help(void) {
	printf("usage: wmvc <corner> <wid>\n");
	exit(0);
}

int window_exists(Window win) {

	Display *dpy;
	dpy = XOpenDisplay(":0");
	int screen = DefaultScreen(dpy);
	Window root_win = RootWindow(dpy, screen);
	Window root_ret, parent_ret, *child_ret;
	unsigned int number_children;
	XQueryTree(dpy, root_win, &root_ret, &parent_ret, &child_ret, \
		&number_children);

	for (int i = 0; i < number_children; i++) {
		Window temp = *(child_ret + i);
		/* 'win' exists, return 0 */
		if (temp == win) {
			return 0;
		}
	}

	return 1;
}

int get_coord_for_corner(Window win, int corner, int *return_x,
	int *return_y) {

	XWindowAttributes win_attrib;
	Display *dpy;
	dpy = XOpenDisplay(":0");

	/* If the given window could not be found, return 1 */
	if (window_exists(win)) {
		printf("Could not find window of given id.\n");
		return 1;
	}

	XGetWindowAttributes(dpy, win, &win_attrib);
	XRRScreenResources *screen_res;
	XRRCrtcInfo *screen_info;

	screen_res = XRRGetScreenResources(dpy, DefaultRootWindow(dpy));
	/* 0 to get the first monitor */
	screen_info = XRRGetCrtcInfo(dpy, screen_res, screen_res->crtcs[0]);

	if (corner == TOP_LEFT) {
		*return_x = screen_info->x;
		*return_y = screen_info->y;
	} else if (corner == TOP_CENTER) {
		*return_x = (screen_info->width)/2 - (win_attrib.width)/2 + screen_info->x;
		*return_y = screen_info->y;
	} else if (corner == TOP_RIGHT) {
		*return_x = (screen_info->width) - (win_attrib.width) + screen_info->x;
		*return_y = screen_info->y;
	} else if (corner == MIDDLE_LEFT) {
		*return_x = screen_info->x;
		*return_y = (screen_info->height)/2 - (win_attrib.height)/2 + screen_info->y;
	} else if (corner == MIDDLE_CENTER) {
		*return_x = (screen_info->width)/2 - (win_attrib.width)/2 + screen_info->x;
		*return_y = (screen_info->height)/2 - (win_attrib.height)/2 + screen_info->y;
	} else if (corner == MIDDLE_RIGHT) {
		*return_x = (screen_info->width) - (win_attrib.width) + screen_info->x;
		*return_y = (screen_info->height)/2 - (win_attrib.height)/2 + screen_info->y;
	} else if (corner == BOTTOM_LEFT) {
		*return_x = screen_info->x;
		*return_y = (screen_info->height) - (win_attrib.height) + screen_info->y;
	} else if (corner == BOTTOM_CENTER) {
		*return_x = (screen_info->width)/2 - (win_attrib.width)/2 + screen_info->x;
		*return_y = (screen_info->height) - (win_attrib.height) + screen_info->y;
	} else if (corner == BOTTOM_RIGHT) {
		*return_x = (screen_info->width) - (win_attrib.width) + screen_info->x;
		*return_y = (screen_info->height) - (win_attrib.height) + screen_info->y;
	// No valid corner number was given, return 1
	} else {
		printf("No valid corner number given. Must match [1-9].\n");
		return 1;
	}

	return 0;

}

/*
* argc is the number of WORDS in the call to the wmvc
* argv contains a bunch of info about the call. The shell, the terminal,
* all that jazz. This is what we want.
*/
int main(int argc, char **argv) {

	int corner = -1;
	/* If the call to wmvc has 3 WORDS (wmvc, the corner number, and the
	 * window id)
	 * In regex form, wmvc [1-9] 0x[0-F]\{8\} */
	if (argc == 3) {

		if (!strcmp(argv[1], "1")) {
			corner = TOP_LEFT;
		} else if (!strcmp(argv[1], "2")) {
			corner = TOP_CENTER;
		} else if (!strcmp(argv[1], "3")) {
			corner = TOP_RIGHT;
		} else if (!strcmp(argv[1], "4")) {
			corner = MIDDLE_LEFT;
		} else if (!strcmp(argv[1], "5")) {
			corner = MIDDLE_CENTER;
		} else if (!strcmp(argv[1], "6")) {
			corner = MIDDLE_RIGHT;
		} else if (!strcmp(argv[1], "7")) {
			corner = BOTTOM_LEFT;
		} else if (!strcmp(argv[1], "8")) {
			corner = BOTTOM_CENTER;
		} else if (!strcmp(argv[1], "9")) {
			corner = BOTTOM_RIGHT;
		} else {
			printf("No valid corner number given. Must match [1-9].\n");
			print_help();
			return 1;
		}

		Display *dpy = XOpenDisplay(":0");
		unsigned long int win_id = strtoul(&argv[2][0], NULL, 0);

		int ret_x = 0;
		int ret_y = 0;

		int stat = get_coord_for_corner(win_id, corner, &ret_x, &ret_y);

		if (stat != 0) {
			return 1;
		}

		XWindowAttributes win_attrib;
		XGetWindowAttributes(dpy, win_id, &win_attrib);

		printf("%d %d %d %d 0x%08x\n", ret_x, ret_y, win_attrib.width, \
			win_attrib.height, win_id);
		return 0;

	}
	/* If the program wasn't called with 2 arguments */
	else {
		printf("Incorrect number of arguments provided.\n");
		print_help();
		return 1;
	}

	return 0;
}
