# wmcontrib

A collection of C programs created to make use of X with no window managers
(or window managers lacking certain features) more tolerable.


## Programs/Contents

wmcontrib consists (currently) of 2 programs: wtfc and wmvc. They stand for
**W**indow **T**ransfer **F**ocus **C**ardinal and **W**indow **M**o**V**e
**C**orner.


### wtfc

wtfc was created to make up for the lack of (in a no-wm setup) what I deem
the core feature of tiling window managers; being able to use the keyboard
to focus windows to the left, right, above or below the currently focused
window. wtfc has designed to be a little more useful than that. You can pass
arguments to wtfc to focus or to not focus the window it finds to the left or
right, etc. This could be useful if you wanted to hack together a bash script
to swap a window with the window on its' left, right or etc. Obviously this
has more potential, but I will leave that to the user. **Note** that in some
window managers, wtfc will not be able to focus the windows it finds due to
the way they control window focusing.


### wmvc

**Note:** This program is currently being considered for a name change. wmvc
is no longer a good name as unlike `wmv` from wmutils, this program does not
"shift" or move the window relative to itself. It's more similar to `wtp`
which "teleports" (just sets the location non-relatively) to given parameters.
Furthermore, this program no longer sends the window to set corners (which is
a bad name to begin with because 5/9 of the locations it could send the window
to were not actually corners), but rather anywhere on the screen.

wmvc came about because I often would create a urxvt instance, set it to
floating and then I'd drag it to a corner with my mouse. For instance, having
a separate urxvt for svn, git or just for compiling the project. wmvc was
designed to replace using the mouse and dragging the window. Instead you can
use wmvc to move a given window to a given location, relative to the screen it
is on. For instance, running (and making use of `wmutils`' `pfw`)
`wmvc -x 1 -y 1 -w $(pfw)` will send the currently focused window to the
bottom right corner of the screen it is currently on. `-x` and `-y` take
`double`s. This means you could do `wmvc -x 0.7593 -y 1 -w $(pfw)` to send
the currently focused window to the bottom of the screen, 75.93% of the way
to being all the way in the bottom right corner. Some of the more useful
locations (combinations of `-x` = 0, 0.5, 1 and `-y` = 0, 0.5, 1) are shown
below.

![wmvc](https://github.com/JSpeedie/wmcontrib/blob/master/wmvc.gif?raw=true)


## To be added

I plan on making some gifs/webms to show what these programs look like in
action (there's one for wmvc, but that will be replaced to show how it's
functionality has been changed to allow for more possibilities), but that
will have to wait for now.
