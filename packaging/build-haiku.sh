#!/bin/bash -eu

TARGET=haiku

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}"
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
	cmake "$SRCDIR" "${CMAKE_FLAGS[@]}" "${GAME_FLAGS[@]}"
	ninja

	strip -s tensy
	mv tensy "$BINDIR"
}

build sdl
build sdl_mixer
build game
