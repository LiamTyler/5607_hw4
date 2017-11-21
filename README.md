# Real Time OpenGL
More info at http://www.liamtyler.com/5607/HW4/index.html

## Installation / Usage
- Naviagate the main directory, and just type 'make'. Note: you may have to change the include's (probably just in 'include/utils.h') for OpenGL and SDL2 to make it compile on your machine. I developed and tested this on Ubuntu 17.04
- Once it compiles, just type 'make run', or './build/bin/proj'

## Map format
- The maps are text files located in maps/
- 'S' and 'G' represent the 'Start' and 'Goal' for the player
- 'L' is for a light
- 'W' for a wall
- 'A' through 'E' are different kinds of doors
- 'a' through 'e' are the matching keys for the doors
- '0' is for just floor
