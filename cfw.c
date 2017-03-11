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
/* Local includes */
#include "common.h"

Display *dpy;

void print_help(void) {
	fprintf(stderr, "usage: cfw\n");
}

/*
* argc is the number of WORDS in the call to the cfw
* argv contains a bunch of info about the call. The shell, the terminal,
* all that jazz. This is what we want.
*/
int main(int argc, char **argv) {

	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "cfw: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}

	Window window_ret = -1;
	int focus_status;
	XGetInputFocus(dpy, &window_ret, &focus_status);
	if (window_ret != PointerRoot && window_ret != None) {
		printf("0x%08x\n", window_ret);
	} else {
		fprintf(stderr, "cfw: Couldn't locate currently focused window\n");
		exit(ERR_COULDNT_LOC_FOCUS_WIN);
	}

	return 0;
}
