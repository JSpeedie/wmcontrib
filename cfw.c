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

		XGetInputFocus(dpy, &win, &thing);
		XSelectInput(dpy, win, FocusChangeMask);
		XSync(dpy, False);
		XFlush(dpy);

		while (1) {
			/* If there are more than 0 X events yet to be removed from the queue */
			if (XPending(dpy) > 0) {
				XNextEvent(dpy, &event);

				if (event.type == FocusIn || event.type == FocusOut) {
					XGetInputFocus(dpy, &win, &thing);
					printf("%08x\n", win);
					// XSync(dpy, True);
				}
			} else {
				/* remove 2 zeros for decent speed. Check old sflock */
				usleep(100000);
			}

			XSync(dpy, False);
			XFlush(dpy);
		}
	}

	return 0;
}
