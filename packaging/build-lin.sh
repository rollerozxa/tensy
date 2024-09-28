#!/bin/bash -e

TARGET=lin

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

# Download appimagetool
if [ ! -f /tmp/appimagetool ]; then
	wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /tmp/appimagetool
	chmod +x /tmp/appimagetool
fi

CMAKE_FLAGS+=(
	"-DCMAKE_INSTALL_PREFIX="
)

build_sdl() {
	get_tar_archive SDL3 "https://github.com/libsdl-org/SDL/archive/${SDL3_tag}.tar.gz"

	mk_build_dir
	cmake .. "${CMAKE_FLAGS[@]}" \
		-DSDL_SHARED=OFF -DSDL_STATIC=ON -DCMAKE_C_FLAGS="-DSDL_LEAN_AND_MEAN=1" \
		-DSDL_{GPU,CAMERA,JOYSTICK,HAPTIC,HIDAPI,OPENGLES,POWER,SENSOR,VULKAN,TESTS,WAYLAND}=OFF
	ninja

	DESTDIR="$BUILDDIR/deps" ninja install
	cd ../..
}

build_game() {
	mk_build_dir
	cmake "$topdir/../" "${CMAKE_FLAGS[@]}" -DSDL3_DIR="$BUILDDIR/deps/lib/cmake/SDL3"
	ninja

	strip -s tensy
	mv tensy "$BINDIR"
}

build_appimage() {
	cd "$topdir"

	cp "$BINDIR/tensy" AppDir/AppRun

	ARCH=x86_64 /tmp/appimagetool --appimage-extract-and-run --comp xz AppDir/

	mv *.AppImage bin/
}

pushd "$BUILDDIR" || exit
build_sdl
build_game
build_appimage
popd || exit
