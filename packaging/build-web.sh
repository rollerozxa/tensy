#!/bin/bash -eu

TARGET=web

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

build_game() {
	mk_build_dir
	emcmake cmake "$SRCDIR" "${CMAKE_FLAGS[@]}"
	ninja

	cp *.js *.wasm "${SRCDIR}/packaging/index.html" "$BINDIR"
}

build game
