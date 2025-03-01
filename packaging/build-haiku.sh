#!/bin/bash -eu

TARGET=haiku

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

CMAKE_FLAGS+=(
	-DCMAKE_TOOLCHAIN_FILE="${SRCDIR}/packaging/toolchain-haiku.cmake"
)

build_game() {
	mk_build_dir
	cmake "$SRCDIR" "${CMAKE_FLAGS[@]}"
	ninja

	strip -s tensy
	mv tensy "$BINDIR"
}

build game
