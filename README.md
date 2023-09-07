# Note To Any Current Cloners
I reccomend grabbing what you want from this repo, rather than trying to use it all and directly depend on this repo. But I would appreciate it if you starred and cloned the repo to get updates.
HPP files may move around a lot, names may change. In general, there will probably be breaking changes. But do come back here when I update the repo to see what all i added.

I think a lot of the files need to be put into their own folder (probably called "nes-game-utils" or somthing)
So this may cause issues with anyone who are currently using the files. But I will soon settle on a specific folder structure that hopefully won't conflict with other folders.

Also, I want to make it clear to anyone reading that I don't have a "fully ironed out" knowledge of C++. I am very experienced with it, but I am eager to learn the nuances of it, because I know I don't know everything about C++'s nuances. So if anyone sees any part of the code that could be written better, like the operator overloads, constructors, or how any other parts of the code are structured, please let me know so those things can be improved/fixed.

# llvm-mos-Game-Utilities
A library of pre-made types and functions to help with general programming or game development. Designed for the new 6502 compiler, "LLVM-MOS".

Right now, this repo contains some useful things like:
+ fixed16 struct
+ vec2fx16 struct
+ vec2u8 struct
+ vec2u16 struct
+ Special Nes abstractions for different parts of the Nes hardware
  * Currently, this is mostly the PPU and Joypad.
+ A debugging tool that can be used with fceux for logging to an external console, performance profiling, etc.
+ A "string pool" system I came up with that can be used for when you need to generate a string from other objects.
+ Various other general utility functions that could be applicable to whatever project you are working on.
+ Other general purpose types that can be used
More to come.


# Other planned additions

* Useful utilities for interfacing with different parts of the NES (palette data, tile data, sprite data, controller?)
* Rect structs for use as colliders for objects
* Other useful functions for performing certain opertaions on bits.
* Packed Binary Coded Decimal struct for storing numbers that are mostly going to be displayed on screen (Score/Points)
* Random number generator

# Some Ideas

* function to convert an angle (1 byte representing [0,360]) to either a vec2byte, or vec2fixed
* function that could then take an angle value, and pick a sprite for that specific angle (this could include sprite flipping)
* function to help with making a hud using the sprite 0 thing
