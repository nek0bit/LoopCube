# LoopCube

![LoopCube Logo (Wii U Meta Image)](meta/icon.png)

LoopCube is an open source Survival/Sandbox game written in C++ and OpenGL.

It has infinite horizontal/vertical world generation, a day/night cycle, caves, and more.

Currently it can be compiled with SDL2 support.

# Screenshots

- *Version 0.3(beta) alpha screenshots*

![v0.3(beta) alpha screenshot 1](meta/screenshot-1.png)

![v0.3(beta) alpha screenshot 2](meta/screenshot-2.png)

# Building
## Linux / macOS
Ensure you are in the current directory, if not, textures may not load properly

Requires: `sdl2 sdl2_image sdl2_ttf glm`

###### If you want to simply run the application in the current directory

Run `make debug` (If you don't supply "debug" then it defaults to debug)

If you would like to build for release, run `make release`

###### If you plan on installing

Run `make DATA_LOCATION=/usr/local/share/loopcube release`

# Installing
## Linux

Copy the data folder into wherever you set the DATA_LOCATION value to be

`sudo make DATA_LOCATION=/usr/local/share/loopcube PREFIX=/usr/local/bin install`

Replace `install` with `uninstall` if you want to remove it.

*Note:* I haven't tested this on macOS yet, be careful

# TODO

Please note that I do this project for fun, as a result, it's very slowly developed and might have some bugs I'm already aware of.

- [ ] Clean up `src/` directory. I didn't expect it to get so big
- [ ] Switch to CMake
- [ ] Finish tweaking the UI system
- [ ] Add old backgrounds back

# License

Licensed under the MIT license, visit `LICENSE` to view more info.
