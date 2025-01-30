#!/bin/bash -eu

TARGET=win

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

CMAKE_FLAGS+=(
	-DCMAKE_TOOLCHAIN_FILE="$topdir/toolchain-${CCPREFIX}.cmake"
)

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
		-DSDL_{GPU,CAMERA,JOYSTICK,HAPTIC,HIDAPI,OPENGLES,POWER,SENSOR,VULKAN,TESTS}=OFF
	dep_ninja_install
}

build_sdl_mixer() {
	get_tar_archive SDL3_mixer "${SDL3_mixer_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${SDL_MIXER_FLAGS[@]}"
	dep_ninja_install
}

build_game() {
	mk_build_dir
	cmake "$topdir/../" "${CMAKE_FLAGS[@]}" "${GAME_FLAGS[@]}"
	ninja

	x86_64-w64-mingw32-strip -s *.exe
	mv *.exe "$BINDIR"
}

build sdl
build sdl_mixer
build game
