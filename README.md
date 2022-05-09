Cogmind Performance Mod Fork
==
This project is a fork of the **deprecated** [Simple Directmedia Layer 1.2 project](https://github.com/libsdl-org/SDL-1.2). It has changes and bugfixes specific to the delightful roguelike [Cogmind](https://gridsagegames.com/cogmind/).

It promises up to 100-500% speedups on modern systems over the retail build shipped with the game. The effect is most noticable when the engine is under stress, by panning the map or in combat with multiple enemies.

Compiling on Windows
--
Open the VisualC/SDL.sln file in at least VS2017. Tested on VS2017 and VS2022. You need the Windows SDK and C++ Desktop development components. You may need the Universal C Runtime SDK and UCRT as well. You may need the DirectX 9 SDK.

**WARNING:** There is a post-build step that will attempt to copy the outputted SDL.dll to `C:\Program Files (x86)\Steam\steamapps\common\Cogmind\SDL.dll`.

Compiling on Linux/macOS
--

Cogmind is a Win32 application. Install a mingw toolchain for i686. I suggest MSYS2. You wll need to use `make`, there is no `CMake` support at this time.
```
git clone git@github.com:aronson/SDL-1.2.git
cd SDL-1.2
./autogen.sh
CFLAGS="-msse4.1 -m32" LDFLAGS="-static-libgcc -lwinpthread" ./configure --disable-assembly --host=i686-w64-mingw32 --build=x86_64-w64-mingw32 --disable-directx
make
```
On ARM-based macOS there is a bug in the mingw compiler. One can also use clang with the mingw toolchain instead to compile with no bugs:
```
WINDRES="i686-w64-mingw32-windres" LDFLAGS="-shared -static -lpthread" CC="/opt/homebrew/Cellar/llvm/13.0.1_1/bin/clang -static-libgcc -static-libstdc++ -msse4.1 -target i686-w64-mingw32 --sysroot=/opt/homebrew/Cellar/mingw-w64/10.0.0_1/toolchain-i686/ -Ofast" ./configure --host=i686-w64-mingw32 --disable-directx --disable-assembly
...
```

Output is in `build/.libs/SDL.dll`.

Parallel builds are supported, as are optimizations with `-O3`. One can build with `-march=native` to enable further vectorization in some routines if possible, with the caveat the library is not portable to other machines.

Current Features
--
- SSE4.1 compatible 2-pixel-wide alpha blending software blitting routine -- [JellySquid](https://github.com/jellysquid3)
- AVX2 compatible 4-pixel-wide routine of the same kind -- JellySquid
- ARGB to AGRB SSE3 compatible color-shift function for proper rendering -- [Luigi](https://github.com/aronson/), Pablo, [SuperCoder79](https://github.com/SuperCoder7979)
- SDL_FreeWav patch to address undefined behavior in Cogmind at runtime for MSVC builds -- Luigi
