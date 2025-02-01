#!/bin/bash -eu

TARGET=web

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	emcmake cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
		-DSDL_HIDAPI=OFF
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
	emcmake cmake "$SRCDIR" "${CMAKE_FLAGS[@]}" "${GAME_FLAGS[@]}"
	ninja

	cp *.js *.wasm "${SRCDIR}/packaging/index.html" "$BINDIR"
}

build sdl
build sdl_mixer
build game
