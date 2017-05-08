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

void print_help(void) {
	fprintf(stderr, "usage: wrc\n");
}

int main(int argc, char **argv) {

	Display *dpy;
	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "wrc: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}
	Window root, orig_win, containing_win;
	int root_x, root_y, offset_x, offset_y, new_x, new_y;
	unsigned int mask;
	XQueryPointer(dpy, DefaultRootWindow(dpy), &root, &orig_win, \
		&root_x, &root_y, &offset_x, &offset_y, &mask);

	/* If the pointer was not in any window */
	if (orig_win == None) {
		exit(ERR_WIN_NOT_FOUND);
	/* If the cursor was contained in a window, focus that window */
	} else {
		int final_w, final_h;
		XEvent event;
		XWindowAttributes win_attr;
		XGetWindowAttributes(dpy, orig_win, &win_attr);
		XSelectInput(dpy, root, PointerMotionMask);

		while(!XNextEvent(dpy, &event)) {
			XQueryPointer(dpy, DefaultRootWindow(dpy), &root, &containing_win, \
				&root_x, &root_y, &new_x, &new_y, &mask);
			/* Check what quadrant of the window the user clicked on */
			/* If they clicked in the top left */
			if (offset_x <= win_attr.x + (win_attr.width / 2) &&
				offset_y <= win_attr.y + (win_attr.height / 2)) {

				final_w = win_attr.width - (new_x - offset_x);
				final_h = win_attr.height - (new_y - offset_y);
				/* You cannot have a window of 0 width or height. */
				if (final_w < 1) { final_w = 1; }
				if (final_h < 1) { final_h = 1; }

				XMoveResizeWindow(dpy, orig_win, \
					win_attr.x + (new_x - offset_x), \
					win_attr.y + (new_y - offset_y), \
					final_w, final_h);
			/* If they clicked in the top right */
			} else if (offset_x > win_attr.x + (win_attr.width / 2) &&
				offset_y <= win_attr.y + (win_attr.height / 2)) {

				final_w = win_attr.width + (new_x - offset_x);
				final_h = win_attr.height - (new_y - offset_y);
				/* You cannot have a window of 0 width or height. */
				if (final_w < 1) { final_w = 1; }
				if (final_h < 1) { final_h = 1; }

				XMoveResizeWindow(dpy, orig_win, \
					win_attr.x, \
					win_attr.y + (new_y - offset_y), \
					final_w, final_h);
			/* If they clicked in the bottom left */
			} else if (offset_x <= win_attr.x + (win_attr.width / 2) &&
				offset_y > win_attr.y + (win_attr.height / 2)) {

				final_w = win_attr.width - (new_x - offset_x);
				final_h = win_attr.height + (new_y - offset_y);
				/* You cannot have a window of 0 width or height. */
				if (final_w < 1) { final_w = 1; }
				if (final_h < 1) { final_h = 1; }

				XMoveResizeWindow(dpy, orig_win, \
					win_attr.x + (new_x - offset_x), \
					win_attr.y, \
					final_w, final_h);
			/* If they clicked in the bottom right */
			} else if (offset_x > win_attr.x + (win_attr.width / 2) &&
				offset_y > win_attr.y + (win_attr.height / 2)) {

				final_w = win_attr.width + (new_x - offset_x);
				final_h = win_attr.height + (new_y - offset_y);
				/* You cannot have a window of 0 width or height. */
				if (final_w < 1) { final_w = 1; }
				if (final_h < 1) { final_h = 1; }

				XMoveResizeWindow(dpy, orig_win, \
					win_attr.x, \
					win_attr.y, \
					final_w, final_h);
			} else {
				exit(ERR_WIN_NOT_FOUND);
			}
			XSync(dpy, False);
		}

		return 0;
	}
}
