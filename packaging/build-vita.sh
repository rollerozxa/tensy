#!/bin/bash -e

TARGET=vita

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

CMAKE_FLAGS+=(
	-DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
)

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
		-DSDL_{GPU,CAMERA,HAPTIC,POWER,SENSOR}=OFF
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

	mv tensy.vpk "$BINDIR"
}

build sdl
build sdl_mixer
build game
