# karson777's dmenu 
This is a fork of Luke Smith's dmenu 

Some differences are 
- Jetbrains Mono Nerd Font 
- Apple TTF font for emojis

## From Luke

Extra stuff added to vanilla dmenu:

- reads Xresources (ergo pywal compatible)
- alpha patch, which importantly allows this build to be embedded in transparent st
- can view color characters like emoji (libxft-bgra is required for this reason)
- `-P` for password mode: hide user input
- `-r` to reject non-matching input
- dmenu options are mouse clickable

## Installation

You must have `libxft-bgra`, `nerd-fonts-jetbrains-mono` and `ttf-apple-emoji` installed 

After making any config changes that you want, but `make`, `sudo make install` it.
