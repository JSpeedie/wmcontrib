# wmcontrib

A collection of C programs created to make use of X with no window managers
(or window managers lacking certain features) more tolerable.


## Programs/Contents

wmcontrib consists (currently) of 3 programs: wtfc, wtsr, and wrsr. They stand for:  
**WTFC:** **W**indow **T**ransfer **F**ocus **C**ardinal  
**WTSR:** **W**indow **T**eleport **S**creen **R**elative  
**WRSR:** **W**indow **R**esize **S**creen **R**elative


### wtfc

wtfc was created to make up for the lack of (in a no-wm setup) what I deem
the core feature of tiling window managers; being able to use the keyboard
to focus windows to the left, right, above or below the currently focused
window. wtfc has designed to be a little more useful than that. You can pass
arguments to wtfc to focus or to not focus the window it finds to the left or
right, etc. This could be useful if you wanted to hack together a bash script
to swap a window with the window on its' left, right or etc. Obviously this
has more potential, but I will leave that to the user.

**Note** that in some
window managers, wtfc will not be able to focus the windows it finds due to
the way some wms control window focusing.


### wtsr

**Note:** This program has just under gone a name change. wtsr is what used to
be wmvc. Apologies if this breaks anyones' binds.

wtsr came about because I often would create a urxvt instance, set it to
floating and then I'd drag it to a corner with my mouse. For instance, having
a separate urxvt for svn, git or just for compiling the project. wtsr was
designed to replace using the mouse and dragging the window. Instead you can
use wtsr to move a given window to a given location, relative to the screen it
is on. For instance, running (and making use of `wmutils`' `pfw`)
`wtsr -x 1 -y 1 -w $(pfw)` will send the currently focused window to the
bottom right corner of the screen it is currently on. `-x` and `-y` take
`double`s. This means you could do `wtsr -x 0.7593 -y 1 -w $(pfw)` to send
the currently focused window to the bottom of the screen, 75.93% of the way
to being all the way in the bottom right corner. Some of the more useful
locations (combinations of `-x` = 0, 0.5, 1 and `-y` = 0, 0.5, 1) are shown
below.

![wtsr](https://github.com/JSpeedie/wmcontrib/blob/master/wtsr.gif?raw=true)


### wrsr

wrsr works similarly to wtsr. It takes 1 or 2 ratios representing a width,
height, or both and resizes a given window to those ratios relative to the
screen it is on. For instance, `wrsr -w 1 -h 1 -W $(pfw)` will resize the
currently focused window to the full size of the screen it was on when the call
was made. Note that this does not include borders so part of the window may
appear off screen. To take in account border sizes, use the `-b` flag.
`wrsr -w 1 -h 1 -W $(pfw) -b` will resize the window to the full size of the
screen, accounting for border width. Just like `wtsr`, `wrsr` accepts doubles.
A call of `wrsr -w 0.252525 -h 0.00001 =W $(pfw)` will resize the width to
25.2525% of the screen width and make the height 0.001% of the height. In cases
where either the new width or height would be 0, the program returns 1. This
is because you cannot have a window with a width or height or 0.


## To be added

* Make wtfc gif
* Make wrsr gif
* Possibly remake wtsr gif to show new possiblities
