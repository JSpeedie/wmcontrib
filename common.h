#ifndef WMCONTRIB_COMMON_FILE
#define WMCONTRIB_COMMON_FILE

extern int ERR_COULDNT_OPEN_X_DISPLAY;
extern int ERR_WIN_NOT_FOUND;
extern int ERR_SCREEN_OF_WIN_NOT_FOUND;
extern int ERR_NEITHER_RATIO_GIVEN;
extern int ERR_WINDOW_NOT_GIVEN;
extern int ERR_RATIO_NOT_IN_RANGE;
extern int ERR_NO_VALID_DIR;
extern int ERR_INVALID_DIRECTION;
extern int ERR_CARDINAL_NOT_GIVEN;

int window_exists(Window);
int get_screen_number_of_win(Window, Display *, int, int *);

#endif
