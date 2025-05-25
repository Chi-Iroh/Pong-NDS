# Pong-NDS
## Introduction

This is a test project to try NDS programming, using [devkitpro](https://devkitpro.org/)'s [libnds](https://github.com/devkitPro/libnds).  
To summarize :
- devkitpro is a toolchain to build games on many Nintendo consoles, such as GBA, Nintendo DS or Wii.
- libnds is a low-level C library which provides every needed feature to use graphics, sound etc..

## Requirements

In addition to <ins>make</ins>, the aforementioned <ins>libnds</ins> must be installed on your system to build the project, see [here](https://devkitpro.org/wiki/Getting_Started).  
This done, you only need to run the `make` command, it will generate a `.nds` ROM file.  

## Running

There are 2 ways to run the game, either on an enumator (like no$gba or DeSmuME), or on real hardware using a [R4](https://www.r4ds.com/) card and a micro SD with a copy of the game.  

## File tree

- `doc/` : assets used in README
- `include/` : header files
- `source/` : source files
- `NDS tutorial (sverx)/` : A tutorial on libnds made by sverx, more information [here](https://github.com/Chi-Iroh/NDS-Tutorial)
- `copy_to_sd.sh` : copies the `.nds` file to a SD mounted on `/mnt/NDS-SD`
- `make_with_log.sh` : same as `make`, except that each command is displayed
- `Makefile` : `make` to build, `make clean` to remove build files, `make re` to rebuild, `.nds` file is generated at the repository root

## Gameplay

Here's a screenshot on MelonDS emulator :  
<img src="doc/in-game screenshot.png" alt="Black background with a yellow ball and two paddles, blue at left and red at right. The bottom screen has a score counter for both players."></img>

This game is like a Pong, with a ball bouncing on edges and on 2 paddles, one controlled by a player and the other automatically moves towards the ball.  
The player controls the blue paddle on the left, using up and down arrows of the D-pad.  
At the beginning / after each point, the ball spawns on the center and goes on a random direction.  
If the ball manages to go behind a paddle, its opponent wins a point.  
Both paddles and the ball are displayed on the top screen, and the points count is displayed on the bottom screen.  

## libnds 2.0

There was recently a major update to libnds which impacted emulators (DesMuME, no$GBA and melonDS), so [here](https://github.com/melonDS-emu/melonDS/pull/2197) is a pending pull request to melonDS by a libnds contributor, you can compile that branch and use that build.  
