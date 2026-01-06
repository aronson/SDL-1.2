#!/bin/sh

export CC="i686-w64-mingw32-gcc -static-libgcc"
export CXX="i686-w64-mingw32-g++ -static-libgcc"
export WINDRES="i686-w64-mingw32-windres"
export AR="i686-w64-mingw32-gcc-ar"
export RANLIB="i686-w64-mingw32-gcc-ranlib"
export STRIP="i686-w64-mingw32-strip"

export CFLAGS="-O3 -flto -D_WIN32_WINNT=0x0501 -Wno-incompatible-pointer-types -static-libgcc -I/opt/llvm-mingw/i686-w64-mingw32/include"
export CXXFLAGS="-O3 -flto -D_WIN32_WINNT=0x0501 -Wno-incompatible-pointer-types -static-libgcc -I/opt/llvm-mingw/i686-w64-mingw32/include"
export LDFLAGS="-L/DirectX/Lib/x86 -shared -lmsvcrt -lmingw32 -static-libgcc -lwinpthread -ldxguid -lddraw -ldsound -ldinput -Wl,--subsystem,windows -luser32 -lgdi32 -lwinmm -flto=auto"

./configure --host=i686-w64-mingw32 \
            --target=i686-w64-mingw32 \
            --prefix=`pwd`/mingw-dist \
            --enable-shared \
            --disable-static \
            --enable-directx \
            --disable-assembly
