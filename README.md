# Note To Any Current Cloners
I think a lot of the files need to be put into their own folder (probably called "nes-game-utils" or somthing)
So this may cause issues with anyone who are currently using the files. But I will soon settle on a specific folder structure that hopefully won't conflict with other folders.

Also, I want to make it clear to anyone reading that I don't have a "fully ironed out" knowledge of C++. I am very experienced with it, but I am eager to learn the nuances of it, because I know I don't know everything about C++'s nuances. So if anyone sees any part of the code that could be written better, like the operator overloads, constructors, or how any other parts of the code are structured, please let me know so those things can be improved/fixed.

# llvm-mos-Game-Utilities
A library of pre-made types and functions to help with general programming or game development. Designed for the new 6502 compiler, "LLVM-MOS".

Right now, there is a basic implementation for:
+ fixed16 struct
+ vec2fx16 struct

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
