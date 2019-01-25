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
	fprintf(stderr, "usage: scinf\n");
}

/*
* argc is the number of WORDS in the call to the scinf
* argv contains a bunch of info about the call. The shell, the terminal,
* all that jazz. This is what we want.
*/
int main(int argc, char **argv) {

	if (!(dpy = XOpenDisplay(0))) {
		fprintf(stderr, "scinf: Couldn't open X display\n");
		exit(ERR_COULDNT_OPEN_X_DISPLAY);
	}

	int dim_lx;
	int dim_rx;
	int dim_ty;
	int dim_by;

	if (0 == get_full_display_dims(&dim_lx, &dim_rx, &dim_ty, &dim_by)) {

		fprintf(stdout, "%d %d %d %d\n", dim_lx, dim_ty, dim_rx, dim_by);
	} else {
		// TODO: standardize this error
		fprintf(stderr, "There was an error with getting display dims\n");
		return -1;
	}

	return 0;
}
