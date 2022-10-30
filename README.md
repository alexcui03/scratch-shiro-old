# scratch-shiro

Shiro is a Scratch project compiler to convert scratch projects into pure C++ programs.

## Build

### Requirement

CMake 3.15 or later
C++ Compiler (C++ 20 should be supported)

The following libraries are included in git submodules. You don't need to configure or build it by your own.
- jsoncpp
- libzip
- argparser

The following libraries should be configured manually. They should be configured and built before you build Shiro.
- zlib
- resvg
- glad
- glfw
- glm
- irrklang

### Configure & Build
```bash
mkdir build && cd build
cmake .. -DZLIB_INCLUDE_DIR=/path/to/zlib/dir -DZLIB_LIBRARY=/path/to/zlib/library -DRESVG_DIR=/path/to/resvg/dir
cmake --build .
```
