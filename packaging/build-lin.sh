#!/bin/bash -eu

TARGET=lin

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

# Download appimagetool
if [ ! -f /tmp/appimagetool ]; then
	wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /tmp/appimagetool
	chmod +x /tmp/appimagetool
fi

build_game() {
	mk_build_dir
	cmake "$SRCDIR" "${CMAKE_FLAGS[@]}"
	ninja

	strip -s tensy
	mv tensy "$BINDIR"
}

build_appimage() {
	cd "${SRCDIR}/packaging"

	cp "$BINDIR/tensy" AppDir/AppRun

	ARCH=x86_64 /tmp/appimagetool --appimage-extract-and-run --comp xz AppDir/

	mv *.AppImage bin/
}

build game
build appimage
