#!/bin/bash -eu

TARGET="$1"

SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

CMAKE_FLAGS=(
	-GNinja
	-DUSE_VENDORED_LIBS=1
	-DCMAKE_INSTALL_PREFIX=
)

if [ $TARGET == 'linux' ]; then
	BUILDDIR="/tmp/tensy/lin64"
	BINDIR="${SRCDIR}/packaging/bin/lin64"

	if [ ! -f /tmp/appimagetool ]; then
		wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /tmp/appimagetool
		chmod +x /tmp/appimagetool
	fi
elif [ $TARGET == 'web' ]; then
	BUILDDIR="/tmp/tensy/web"
	BINDIR="${SRCDIR}/packaging/bin/web"
elif [ $TARGET == 'win' ]; then
	ARCH="$2"

	if [ "$ARCH" == "32" ]; then
		BUILDDIR="/tmp/tensy/win32"
		BINDIR="${SRCDIR}/packaging/bin/win32"
		CCPREFIX=i686-w64-mingw32
	elif [ "$ARCH" == "arm64" ]; then
		BUILDDIR="/tmp/tensy/winarm64"
		BINDIR="${SRCDIR}/packaging/bin/winarm64"
		CCPREFIX=aarch64-w64-mingw32
	else
		BUILDDIR="/tmp/tensy/win64"
		BINDIR="${SRCDIR}/packaging/bin/win64"
		CCPREFIX=x86_64-w64-mingw32
	fi

	CMAKE_FLAGS+=(
		-DCMAKE_TOOLCHAIN_FILE="${SRCDIR}/packaging/toolchain-${CCPREFIX}.cmake"
	)
elif [ $TARGET == 'macos' ]; then
	BUILDDIR="/tmp/tensy/macos"
	BINDIR="${SRCDIR}/packaging/bin/macos"

	CMAKE_FLAGS+=(
		"-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"
		-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11
	)
elif [ $TARGET == 'haiku' ]; then
	BUILDDIR="/tmp/tensy/haiku"
	BINDIR="${SRCDIR}/packaging/bin/haiku"

	CMAKE_FLAGS+=(
		-DCMAKE_TOOLCHAIN_FILE="${SRCDIR}/packaging/toolchain-haiku.cmake"
	)
elif [ $TARGET == 'vita' ]; then
	BUILDDIR="/tmp/tensy/vita"
	BINDIR="${SRCDIR}/packaging/bin/vita"

	CMAKE_FLAGS+=(
		-DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
	)
fi

mkdir -p "$BUILDDIR"
rm -rf "$BINDIR"
mkdir -p "$BINDIR"

source "${SRCDIR}/packaging/dl-deps.sh"

pushd "$BUILDDIR"

cmd=(cmake "$SRCDIR" "${CMAKE_FLAGS[@]}")
[ "$TARGET" == "web" ] && cmd=(emcmake "${cmd[@]}")

"${cmd[@]}"

ninja

DESTDIR="$BINDIR" ninja install

if [ $TARGET == 'linux' ]; then
	cd "${SRCDIR}/packaging"

	cp "$BINDIR/tensy" AppDir/AppRun

	ARCH=x86_64 /tmp/appimagetool --appimage-extract-and-run --comp xz AppDir/
	mv *.AppImage bin/
fi
