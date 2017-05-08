# wmcontrib

A collection of C programs created to make use of X with no window managers
(or window managers lacking certain features) more tolerable.


## Programs/Contents

wmcontrib consists (currently) of 9 programs. They are:  
**wtfc:** **w**indow **t**ransfer **f**ocus **c**ardinal  
**wtsr:** **w**indow **t**eleport **s**creen **r**elative  
**wrsr:** **w**indow **r**esize **s**creen **r**elative  
**cfw:** **c**urrently **f**ocused **w**indow  
**wsft:** **w**indow **s**hi**ft**  
**wsrz:** **w**indow **r**e**s**i**z**e  
**wuc:** **w**indow **u**nder **c**ursor  
**BETA**  
**wucm:** **w**indow **u**nder **c**ursor **m**ove  
**wucr:** **w**indow **u**nder **c**ursor **r**esize


### wtfc

wtfc was created to make up for the lack of (in a no-wm setup), what I deem
the core feature of tiling window managers; being able to focus neighbouring
windows using the keyboard. wtfc was designed to be a little more useful than
that. You can pass arguments to wtfc to focus or to not focus the window it
finds. This could be useful if you wanted to hack together a bash script
to swap a window with a neighbouring window. Obviously this has more
potential, but I will leave that to the user. Below you see an example of using
`wtfc` to focus the windows to the left (`-lf`) and to the right (`-rf`) of
the currently focused window. The gif also shows the window ids via `cfw`
to demonstrate the output of `wtfc`.

**Note** that in some window managers, `wtfc` will NOT be able to focus the
windows it finds due to the way some wms control window focusing.

![wtfc](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/wtfc.gif)


### wtsr

**Note:** This program has just under gone a name change. wtsr is what used to
be wmvc. Apologies if this breaks anyones' binds.

wtsr came about because I often would create a urxvt instance, set it to
floating and then I'd drag it to a corner with my mouse. For instance, having
a separate urxvt for svn, git or just for compiling the project. wtsr was
designed to speed up/replace this process. Instead you can
use wtsr to move a given window to a location, relative to the screen it
is on. For instance, running `wtsr -x 1 -y 1 -w $(cfw)` will send the
currently focused window to the bottom right corner of the screen it is
currently on. `-x` and `-y` take `double`s. This means you could do
`wtsr -x 0.7593 -y 0.5 -w $(cfw)` to send the currently focused window to
the middle of the screen in the y, 75.93% of the way to the right.
Examples where `-x` and `-y` = 0, 1, 0.5 and `-x` = 0.5 `-y` = 0.7593 are
shown below.

![wtsr](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/wtsr.gif)


### wrsr

wrsr works similarly to wtsr. It takes 1 or 2 ratios representing a width,
height, or both and resizes a given window to those ratios relative to the
screen it is on. For instance, `wrsr -w 1 -h 1 -W $(cfw)` will resize the
currently focused window to the full size of the screen it was on when the call
was made. Note that this does not include borders so part of the window may
appear off screen. To take in account border sizes, use the `-b` flag.
`wrsr -w 1 -h 1 -W $(cfw) -b` will resize the window to the full size of the
screen, accounting for border width. Just like `wtsr`, `wrsr` accepts doubles.
A call of `wrsr -w 0.252525 -h 0.00001 -W $(cfw)` will resize the width to
25.2525% of the screen width and make the height 0.001% of the height. In cases
where either the new width or height would be 0, the program sets it (or them)
to 1, because you cannot have a window with a width or height or 0. Examples
where `-w` and `-h` = 0.25, 0.5, 0.75, 1 are shown below.

![wrsr](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/wrsr.gif)


### cfw

Simply prints the currently focused window's id. Takes no arguments. Prints out
an error to stderr and returns a non-zero number if the currently focused
window cannot be found.

This was created out of necessity in case one needs programs in this
directory but doesn't want to install `wmutils`. I used `wmutil`'s `pfw` for
a long time, but I feel that this repo shouldn't "depend" (most of my binds
that use the programs in this repo use `pfw`) so heavily on something from
another repo.

![cfw](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/cfw.gif)


### wsft

Very basic program that just shifts a given window by a given amount in the
x or y direction. I didn't like how `wmutils` `wmv` didn't allow you to move
the window outside of the screen borders. Sometimes I like to have the window
just peeking onto the screen (if I'm running a MySql or testing a server program
of mine that I want to check on only once in a while). Similarly to `cfw`,
I also like not depending on yet another part of `wmutils`. The gif below
showcases a user moving a window in a loop pattern by moving it 50 pixels
at a time in a given direction. 2 of the commands used are:
`wsft -x -50 -y 0 -w $(cfw) -m` and `wsft -x 0 -y 50 -w $(cfw) -m`.

![wsft](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/wsft.gif)


### wrsz

Also a basic program. Modifies the size of the given window by the given amount
in the dimension (width or height). This came about as a complement to `wsft`
because if this repo was going to contain programs to shift the windows, it
should have ones to resize them too. The gif below showcases a user resizing
a window by adding or subtracting 50 pixels at a time in a given dimension.
2 of the commands used are:
`wrsz -w 50 -h 0 -W $(cfw) -r` and `wrsz -w 0 -h -50 -W $(cfw) -r`.

![wrsz](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/wrsz.gif)


### wuc

Fairly important program for pure X usage in my opinion. Simply outputs the
id of the window under the cursor, if there is one. Using the `-f` flag,
it will attempt to focus the window it finds as well. By binding `wuc -f` to
mouse 1, you can have click-to-focus functionality without a window manager.
In the gif below, a user hovers their mouse over 2 windows running the command
`wuc -f` once the mouse is over the window they wish to focus.

**Note** that in some window managers, `wuc` will NOT be able to focus the
windows it finds due to the way some wms control window focusing.

![wuc](https://raw.githubusercontent.com/wiki/JSpeedie/wmcontrib/images/wuc.gif)


## BETA

The two programs below are currently being renamed. Until suitable names are
found, and other kinks have been sorted out, they are not recommended for use.


### wucm

Coming soon...


### wucr

Coming soon...


## To be added

* Write wucm and wucr man pages, readme overviews.
* Make wucm and wucr gifs
* Maybe more zoomed in wsft and wrsz gifs?
