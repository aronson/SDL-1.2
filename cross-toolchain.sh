#!/bin/sh

# Set the path to the correct MSVCRT-based MinGW toolchain
export PATH="$HOME/llvm-mingw-20251118-msvcrt-ubuntu-22.04-x86_64/bin:$PATH"

export CC="i686-w64-mingw32-gcc -static-libgcc"
export CXX="i686-w64-mingw32-g++ -static-libgcc"
export WINDRES="i686-w64-mingw32-windres"
export AR="i686-w64-mingw32-gcc-ar"
export RANLIB="i686-w64-mingw32-gcc-ranlib"
export STRIP="i686-w64-mingw32-strip"

export CFLAGS="-O3 -ffast-math -flto -D_WIN32_WINNT=0x0501 -Wno-incompatible-pointer-types -static-libgcc -I/usr/i686-w64-mingw32/include"
export CXXFLAGS="-O3 -ffast-math -flto -D_WIN32_WINNT=0x0501 -Wno-incompatible-pointer-types -static-libgcc -I/usr/i686-w64-mingw32/include"
export LDFLAGS="-L/home/heni/DirectX/Lib/x86 -shared -lmsvcrt -lmingw32 -static-libgcc -lwinpthread -ldxguid -lddraw -ldsound -ldinput -Wl,--subsystem,windows -luser32 -lgdi32 -lwinmm -flto=auto"

./configure --host=i686-w64-mingw32 \
            --target=i686-w64-mingw32 \
            --prefix=`pwd`/mingw-dist \
            --enable-shared \
            --disable-static \
            --enable-directx \
            --disable-assembly
