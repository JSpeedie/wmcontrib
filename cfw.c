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

#include <ctype.h>
#include <pwd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <X11/keysym.h>

Display *dpy;
Window win;
XEvent event;

void print_help(void) {
	printf("usage: cfw\n");
	exit(0);
}

/*
* argc is the number of WORDS in the call to the cfw
* argv contains a bunch of info about the call. The shell, the terminal,
* all that jazz. This is what we want.
*/
int main(int argc, char **argv) {

	if (!(dpy = XOpenDisplay(0))) {
		printf("Couldn't open X display\n");
		exit(1);
	} else {
		int thing;
		Window root;

		root = XDefaultRootWindow(dpy);
		XGetInputFocus(dpy, &win, &thing);
		XSelectInput(dpy, win, FocusChangeMask);

		// while (!XPeekEvent(dpy, &event)) {
		while (!XNextEvent(dpy, &event)) {
		// while (1) {
			// XCheckMaskEvent(dpy, FocusChangeMask, &event);

			// if (event.type == FocusIn || event.type == FocusOut) {
			if (event.type == FocusOut) {
				XGetInputFocus(dpy, &win, &thing);
				printf("%08x\n", win);
				XSelectInput(dpy, win, FocusChangeMask);
			}
		}
	}

	return 0;
}
