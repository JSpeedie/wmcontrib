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
int print_results = 1;
int focus_after = 0;

void print_help(void) {
	fprintf(stderr, "usage: wuc -[Nf]\n");
}

int main(int argc, char **argv) {

	int opt;
	struct option opt_table[] = {
		/* fine functionality options */
		{ "no-print",	no_argument,	NULL,	'N' },
		{ "focus",		no_argument,	NULL,	'f' },
		{ 0, 0, 0, 0 }
	};

	while ((opt = getopt_long(argc, argv, "Nf", opt_table, NULL)) != -1) {
		switch (opt) {
			/* fine functionality options */
			case 'N': print_results = 0; break;
			case 'f': focus_after = 1; break;
		}
	}

	Display *dpy;
	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "wuc: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}
	Window root, containing_win;
	int root_x, root_y, win_x, win_y;
	unsigned int mask;
	XQueryPointer(dpy, DefaultRootWindow(dpy), &root, &containing_win, \
		&root_x, &root_y, &win_x, &win_y, &mask);

	/* If the pointer was not in any window */
	if (containing_win == None) {
		return ERR_WIN_NOT_FOUND;
	/* If the cursor was contained in a window, focus that window */
	} else {
		/* option flags that take effect near the end of the process */
		if (print_results == 1) {
			printf("0x%08x\n", containing_win);
		}
		if (focus_after == 1) {
			XSetInputFocus(dpy, containing_win, RevertToParent, CurrentTime);
			XSync(dpy, False);
		}

		return 0;
	}

}
