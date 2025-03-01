#!/bin/bash -eu

TARGET=vita

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

CMAKE_FLAGS+=(
	-DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
)

build_game() {
	mk_build_dir
	cmake "$SRCDIR" "${CMAKE_FLAGS[@]}"
	ninja

	mv tensy.vpk "$BINDIR"
}

build game
