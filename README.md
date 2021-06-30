# dsblocks

Modular status monitor for dwm, written and meant to be configured in C, with
features including signaling, clickability, cursor hinting and color.

> The project is only meant for people comfortable with the C language. See
> [dwmblocks](https://github.com/ashish-yadav11/dwmblocks) if you are not one
> of those.

# Usage

`dsblocks`

# Modifying blocks

Each block has two functions associated with it, one responsible for updating
what is shown in status and the other responsible for handling clicks. Take a
look at [config.h](config.h) and files in [blocks](blocks) folder. Functions
defined in [util.c](util.c) might prove helpful when adding your own blocks.

> The provided blocks and helpers are what I personally use. They may or may
> not work for others. They are mainly provided for suggestion purposes. (They
> use siji font for icons.)

If you want to contribute to the project and have written a block which might
prove helpful to others, add a pull request putting your block in contrib
folder.

# Colored output and Clickability

[patches](patches) folder contains two patches for dwm, one for dwm already
patched with systray patch and the other for vanilla dwm. One of the patches,
whichever appropriate, is essential for dsblocks to function properly. It will
add support for colored text (inspired by statuscolors patch for dwm),
clickability (inspired by statuscmd patch for dwm) and cursor hinting when
hovering on clickable blocks (inspired by polybar).

To add colors to status, have your functions output raw characters from `\x0b`
to `\x1f`. `\x0b` in status text switches active colorscheme to the first one in
the colors array defined in dwm's config.h and so on. See [util.h](util.h) and
[statuscolors patch for dwm](https://dwm.suckless.org/patches/statuscolors/)
for more info. Keep in mind that you have to start from `\x0b` instead of `\x01`
as instructed on the patch's page.

# Signaling changes

To signal a specific block to update, run `sigdsblocks <signal> [<sigval>]`.
`<sigval>` is optional and must be an integer.

# xgetrootname

A tiny program to get the current root name. May prove helpful in debugging.

# Acknowledgements

Some ideas and code was taken from other projects. Credits for those go to -

* torrinfail ([dwmblocks](https://github.com/torrinfail/dwmblocks))
* Daniel Bylinka ([statuscmd patch for dwm](https://dwm.suckless.org/patches/statuscmd/))
* Jeremy Jay ([statuscolors patch for dwm](https://dwm.suckless.org/patches/statuscolors/))
