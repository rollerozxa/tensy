#!/bin/bash -e

TARGET=win

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

CMAKE_FLAGS+=(
	"-DCMAKE_TOOLCHAIN_FILE=$topdir/toolchain-${CCPREFIX}.cmake"
	"-DCMAKE_INSTALL_PREFIX="
)

build_sdl() {
	get_tar_archive SDL3 "https://github.com/libsdl-org/SDL/archive/${SDL3_tag}.tar.gz"

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
