#!/bin/bash -e

TARGET=vita

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

CMAKE_FLAGS+=(
	-DCMAKE_INSTALL_PREFIX=
	-DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
)

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${CMAKE_SDL_FLAGS[@]}" \
		-DSDL_{GPU,CAMERA,HAPTIC,POWER,SENSOR}=OFF
	ninja

	DESTDIR="$BUILDDIR/deps" ninja install
	cd ../..
}

build_game() {
	mk_build_dir
	cmake "$topdir/../" "${CMAKE_FLAGS[@]}" -DSDL3_DIR="$BUILDDIR/deps/lib/cmake/SDL3"
	ninja

	mv tensy.vpk "$BINDIR"
}

pushd "$BUILDDIR" || exit
build_sdl
build_game
popd || exit
