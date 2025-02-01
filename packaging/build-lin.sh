#!/bin/bash -eu

TARGET=lin

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

# Download appimagetool
if [ ! -f /tmp/appimagetool ]; then
	wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /tmp/appimagetool
	chmod +x /tmp/appimagetool
fi

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
		-DSDL_{HIDAPI,OPENGLES,WAYLAND}=OFF
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

build_appimage() {
	cd "${SRCDIR}/packaging"

	cp "$BINDIR/tensy" AppDir/AppRun

	ARCH=x86_64 /tmp/appimagetool --appimage-extract-and-run --comp xz AppDir/

	mv *.AppImage bin/
}

build sdl
build sdl_mixer
build game
build appimage
