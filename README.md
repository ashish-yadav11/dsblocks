# dsblocks

Modular status monitor for dwm, written and meant to be extended in C, with
features including signaling, clickability, cursor hinting and color.

> The project is only meant for people comfortable with the C language. See
> [dwmblocks](https://github.com/ashish-yadav11/dwmblocks) if you are not one
> of those.

# Usage

`dsblocks [-d <delimiter>]`

# Modifying blocks

Blocks are composed of two functions, one responsible for updating what is
shown on the status and the other responsible for handling clicks. Take a look
at [blocks.h](blocks.h) file and files in [blocks folder](blocks). Functions
defined in [util.c](util.c) might prove helpful when adding your own blocks.

> The provided blocks and helpers are what I personally use. They may or may
> not work for others. They are mainly provided for suggestion purposes.

If you want to contribute to the project and have written a block which might
prove helpful to others, add a pull request putting your block in contrib
folder.

# Colored output and Clickability

The patches folder contains two patches for dwm, one for dwm already patched
with systray patch and the other for dwm without systray. One of the patches,
whichever appropriate, is essential for dsblocks to function properly. It will
add support for colored text (inspired by statuscolors patch for dwm),
clickability (inspired by statuscmd patch for dwm) and cursor hinting when
hovering on clickable blocks (inspired by polybar).

To add colors to the status have your functions output raw characters from
'\x0b' to '\x31'. '\x0b' in status text switches the active colorscheme to the
first one in the scheme array in dwm and so on. See [util.h](util.h) file and
[statuscolors patch](https://dwm.suckless.org/patches/statuscolors/)
for more info. Keep in mind that you have to start from '\x0b' instead of '\x01'
as instructed on statuscolors patch's page.

# Signaling changes

To signal a specific block to update, run `sigdsblocks <signal> [<sigval>]`.
`<sigval>` is optional and must be an integer.

# xgetrootname

It is a tiny program to get the current root window name. May prove helpful in
debugging.

# Acknowledgements

Some ideas and code was taken from other projects. Credits for those go to -

* torrinfail ([dwmblocks](https://github.com/torrinfail/dwmblocks))
* Daniel Bylinka ([statuscmd patch for dwm](https://dwm.suckless.org/patches/statuscmd/))
* Jeremy Jay ([statuscolors patch for dwm](https://dwm.suckless.org/patches/statuscolors/))
