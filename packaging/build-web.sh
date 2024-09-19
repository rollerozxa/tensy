#!/bin/bash -e

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

BUILDDIR="/tmp/ffdeps/web"
BINDIR="$topdir/bin/web"

mkdir -p "$BUILDDIR"
rm -rf "$BINDIR"
mkdir -p "$BINDIR"

CMAKE_FLAGS=(
	"-G Ninja"
	"-DCMAKE_BUILD_TYPE=Release"
)

build_sdl() {
	get_tar_archive SDL3 "https://github.com/libsdl-org/SDL/archive/c797ae161929070a3e4087aa4ad4c6185b776954.tar.gz"

	mk_build_dir
	emcmake cmake .. "${CMAKE_FLAGS[@]}" \
		-DCMAKE_C_FLAGS="-DSDL_LEAN_AND_MEAN=1" \
		-DSDL_{GPU,CAMERA,JOYSTICK,HAPTIC,HIDAPI,POWER,SENSOR,VULKAN,TESTS}=OFF
	ninja

	ninja install
	cd ../..
}

build_game() {
	mk_build_dir
	emcmake cmake "$topdir/../" "${CMAKE_FLAGS[@]}"
	ninja

	cp *.js *.wasm "$topdir/index.html" "$BINDIR"
}

pushd "$BUILDDIR" || exit
build_sdl
build_game
popd || exit
