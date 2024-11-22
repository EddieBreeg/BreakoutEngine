# Breakout Engine

Breakout is a small C++ game engine. It is by no means meant to be an alternative
to the other engines out there, I just happen to be very interested in these things
and wanted to take a shot at creating my own 🙂.

## How To Build

First order of business is to correctly clone the repo **recursively**:
```bash
git clone git@github.com:EddieBreeg/BreakoutEngine.git --recursive
```

If you cloned the repo without its submodules, no worries, they can be initialized as follows:
```bash
git submodule update --init
```

Right now, only Windows is officially supported (sorry Linux folks). Although the code is mostly cross platform, I haven't tested it on any other platform.

~~Breakout uses [CMake](https://cmake.org) as its build system, so in theory building it should be as simple as:~~
~~- configuring the project, either via command line using the `cmake` command or via CMake GUI~~
~~- building it either with `cmake --build -B build`, or with the generated project files and the IDE of your choice~~

Building the project using CMake *should* be a piece of cake (famous last words): use the configure/build presets included in the [CMakePresets file](CMakePresets.json).

```
cmake --preset win64
cmake --build --preset win64_dev
```