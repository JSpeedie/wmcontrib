/* See LICENSE file for license details. */
#define _XOPEN_SOURCE 500

#include <err.h>
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

	int lx;
	int rx;
	int ty;
	int by;

	// TODO: add "flag" for using anchors or not
	int ret = get_monitor_dims_of_focused_screen(1, &lx, &rx, &ty, &by);

	if (ret == 0) {
		fprintf(stdout, "%d %d %d %d\n", \
			lx, ty, rx-lx, by-ty);
	} else {
		return ret;
	}

	return 0;
}
