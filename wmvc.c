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
	printf("wmvc\n\tusage: " \
		"[ -a ]");

	printf("\n\n\t-a, --thiswillbeanoptionflag variable");

	printf("\n");
	exit(0);
}

int get_coord_for_corner(Window win, int corner, int *return_x,
	int *return_y) {

	Display *dpy;
	XRRScreenResources *screen;
	XRRCrtcInfo *crtc_info;

	dpy = XOpenDisplay(":0");
	screen = XRRGetScreenResources (dpy, DefaultRootWindow(dpy));
	/* 0 to get the first monitor */
	crtc_info = XRRGetCrtcInfo (dpy, screen, screen->crtcs[0]);
	int test = crtc_info->noutput;

	return 1;


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

		corner = 1;

		/*if (!strcmp(argv[1], '1')) {
			corner = TOP_LEFT;
		}*/
			/*case '1':
				corner = TOP_LEFT; break;
			case '2':
				corner = TOP_CENTER; break;
			case '3':
				corner = TOP_RIGHT; break;
			case '4':
				corner = MIDDLE_LEFT; break;
			case '5':
				corner = MIDDLE_CENTER; break;
			case '6':
				corner = MIDDLE_RIGHT; break;
			case '7':
				corner = BOTTOM_LEFT; break;
			case '8':
				corner = BOTTOM_CENTER; break;
			case '9':
				corner = BOTTOM_RIGHT; break;
			case '9':
				XSetInputFocus(dpy, None, RevertToNone, CurrentTime);
				XSync(dpy, False);
				exit(0);
				break;*/

		/* If a valid corner was not specified, then print usage help */
		if (corner == -1) {
			printf("no valid corner number given. Must match [1-9]\n");
			print_help();
			return 1;
		} else {
			const char *thirdarg = &argv[2][2];
			unsigned long int win_id = strtoul(argv[2][2], NULL, 16);
			XWindowAttributes win_attrib;
			XGetWindowAttributes(dpy, win_id, &win_attrib);

			int *ret_x;
			int *ret_y;
			// get_coord_for_corner(argv[2], corner, &ret_x, &ret_y);

			printf("valid corner given");
			return 1;
		}
	}
	/* If the program wasn't called with 2 arguments */
	else {
		printf("Incorrect number of arguments provided.\n\n");
		print_help();
		return 1;
	}

	return 0;
}
