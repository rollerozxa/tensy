#!/bin/bash -e

TARGET=macos

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

CMAKE_FLAGS+=(
	"-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"
	-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11
	-DCMAKE_INSTALL_PREFIX=
)

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" "${CMAKE_SDL_FLAGS[@]}" \
		-DSDL_{GPU,CAMERA,OPENGLES,POWER,SENSOR,VULKAN,TESTS}=OFF
	ninja

	DESTDIR="$BUILDDIR/deps" ninja install
	cd ../..
}

build_game() {
	mk_build_dir
	cmake "$topdir/../" "${CMAKE_FLAGS[@]}" -DSDL3_DIR="$BUILDDIR/deps/lib/cmake/SDL3"
	ninja

	strip tensy
	mv tensy "$BINDIR"
}

create_bundle() {
	cd "$BINDIR"
	mkdir -p tensy.app/Contents/{MacOS,Resources}
	cd tensy.app/Contents

	cp "$topdir/Info.plist" .
	echo "APPLtnsy" > PkgInfo

	cp "$topdir/tensy.icns" Resources/
	mv "$BINDIR/tensy" MacOS/
}

pushd "$BUILDDIR" || exit
build_sdl
build_game
create_bundle
popd || exit
