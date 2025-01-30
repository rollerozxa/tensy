#!/bin/bash -e

TARGET=web

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	emcmake cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
		-DSDL_{GPU,CAMERA,JOYSTICK,HAPTIC,HIDAPI,POWER,SENSOR,VULKAN,TESTS}=OFF
	dep_ninja_install
}

build_sdl_mixer() {
	get_tar_archive SDL3_mixer "${SDL3_mixer_url}"

	mk_build_dir
	emcmake cmake .. "${CMAKE_FLAGS[@]}" "${SDL_MIXER_FLAGS[@]}"
	dep_ninja_install
}

build_game() {
	mk_build_dir
	emcmake cmake "$topdir/../" "${CMAKE_FLAGS[@]}" "${GAME_FLAGS[@]}"
	ninja

	cp *.js *.wasm "$topdir/index.html" "$BINDIR"
}

build sdl
build sdl_mixer
build game
