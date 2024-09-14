#!/bin/bash -e

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

ARCH="$1"
shift

if [ "$ARCH" == "32" ]; then
	BUILDDIR="/tmp/ffdeps/win32"
	BINDIR="$topdir/bin/win32"
	CCPREFIX=i686-w64-mingw32
else
	BUILDDIR="/tmp/ffdeps/win64"
	BINDIR="$topdir/bin/win64"
	CCPREFIX=x86_64-w64-mingw32
fi

mkdir -p "$BUILDDIR"
rm -rf "$BINDIR"
mkdir -p "$BINDIR"

CMAKE_FLAGS=(
	"-DCMAKE_TOOLCHAIN_FILE=$topdir/toolchain-${CCPREFIX}.cmake"
	"-G Ninja"
	"-DCMAKE_BUILD_TYPE=Release"
	"-DCMAKE_INSTALL_PREFIX="
)

build_sdl() {
	get_tar_archive SDL3 "https://github.com/libsdl-org/SDL/archive/c797ae161929070a3e4087aa4ad4c6185b776954.tar.gz"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" \
		-DSDL_SHARED=OFF -DSDL_STATIC=ON -DCMAKE_C_FLAGS="-DSDL_LEAN_AND_MEAN=1" \
		-DSDL_{GPU,CAMERA,JOYSTICK,HAPTIC,HIDAPI,OPENGLES,POWER,SENSOR,VULKAN,TESTS}=OFF
	ninja

	DESTDIR="$BUILDDIR/deps" ninja install
	cd ../..
}

build_game() {
	mk_build_dir
	cmake "$topdir/../" "${CMAKE_FLAGS[@]}" -DSDL3_DIR="$BUILDDIR/deps/lib/cmake/SDL3"
	ninja

	x86_64-w64-mingw32-strip -s *.exe
	mv *.exe "$BINDIR"
}

pushd "$BUILDDIR" || exit
build_sdl
build_game
popd || exit
