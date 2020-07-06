# dsblocks

Modular status monitor for dwm written in C.

# Usage

`dsblocks [-d delimiter]`

# Modifying blocks

Take a look at `blocks.h` file.

## Output from external programs

Use `getcmdout` function defined in `dsblocks.c`. See `volumeu` function defined
in `blocks/volume.c` for an example. Note that `getcmdout` doesn't guarantee
null termination.

# Colored output and Clickability

The patches folder contains a patch for dwm which is required for dsblocks to
function properly. It adds support for colored text (inspired by statuscolors
patch for dwm), clickability (inspired by statuscmd patch for dwm) and cursor
hinting when hovering on clickable blocks (inspired by polybar).

To add colors to the status have your functions output raw characters from
'\x11' to '\x31'. '\x11' in status text switches the active colorscheme to the
first one in the scheme array in dwm and so on. See
[statuscolors patch](https://dwm.suckless.org/patches/statuscolors/)
for more info. Keep in mind that you have to start from '\x11' instead of '\x01'
as instructed on the page.

# Signaling changes

To signal a specific block to update, run `sigdsblocks signal [sigval]`.
`sigval` is optional and must be an integer. When not provided it defaults to 0.

# xgetrootname

It is a tiny program to get the current root window name. May prove helpful in
debugging.

# Acknowledgements

Some ideas and code was taken from other projects. Credits for those go to -

* torrinfail ([dwmblocks](https://github.com/torrinfail/dwmblocks))
* Daniel Bylinka ([statuscmd patch for dwm](https://dwm.suckless.org/patches/statuscmd/))
* Jeremy Jay ([statuscolors patch for dwm](https://dwm.suckless.org/patches/statuscolors/))
