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

wmvc came about because I often would create a urxvt instance, set it to
floating and then I'd drag it to a corner with my mouse. For instance, having
a separate urxvt for svn, git or just for compiling the project. wmvc was
designed to replace using the mouse and dragging the window. Instead you can
use wmvc (in conjunction with wmutils wtp) to move a given window to a given
"corner". With wtp, wmvc can send a window to any of the 4 corners of
the screen as well as 5 other locations which may be useful. These locations
are (from 1 to 9 respectively), are shown in the gif below.

![wmvc](https://github.com/JSpeedie/wmcontrib/blob/master/wmvc.gif?raw=true)


## To be added

I plan on making some webms to show what these programs look like in action,
but that will have to wait for now.
