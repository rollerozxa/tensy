#!/bin/bash -eu

TARGET=macos

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

CMAKE_FLAGS+=(
	"-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"
	-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11
)

build_game() {
	mk_build_dir
	cmake "$SRCDIR" "${CMAKE_FLAGS[@]}"
	ninja

	strip tensy
	mv tensy "$BINDIR"
}

build_create_bundle() {
	cd "$BINDIR"
	mkdir -p tensy.app/Contents/{MacOS,Resources}
	cd tensy.app/Contents

	cp "${SRCDIR}/packaging/Info.plist" .
	echo "APPLtnsy" > PkgInfo

	cp "${SRCDIR}/packaging/tensy.icns" Resources/
	mv "${BINDIR}/tensy" MacOS/
}

build game
build create_bundle
