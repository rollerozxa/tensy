#!/bin/bash -eu

TARGET=win

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

CMAKE_FLAGS+=(
	-DCMAKE_TOOLCHAIN_FILE="${SRCDIR}/packaging/toolchain-${CCPREFIX}.cmake"
)

build_game() {
	mk_build_dir
	cmake "$SRCDIR" "${CMAKE_FLAGS[@]}"
	ninja

	x86_64-w64-mingw32-strip -s *.exe
	mv *.exe "$BINDIR"
}

build game
