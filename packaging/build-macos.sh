#!/bin/bash -e

TARGET=macos

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

CMAKE_FLAGS+=(
	"-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"
	-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11
)

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
		-DSDL_{GPU,CAMERA,OPENGLES,POWER,SENSOR,VULKAN,TESTS}=OFF
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

	strip tensy
	mv tensy "$BINDIR"
}

build_create_bundle() {
	cd "$BINDIR"
	mkdir -p tensy.app/Contents/{MacOS,Resources}
	cd tensy.app/Contents

	cp "$topdir/Info.plist" .
	echo "APPLtnsy" > PkgInfo

	cp "$topdir/tensy.icns" Resources/
	mv "$BINDIR/tensy" MacOS/
}

build sdl
build sdl_mixer
build game
build create_bundle
