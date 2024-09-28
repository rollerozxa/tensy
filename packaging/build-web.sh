#!/bin/bash -e

TARGET=web

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

build_sdl() {
	get_tar_archive SDL3 "https://github.com/libsdl-org/SDL/archive/${SDL3_tag}.tar.gz"

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
