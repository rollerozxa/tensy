#!/bin/bash -eu

TARGET="$1"

SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

CMAKE_FLAGS=(
	-GNinja
	-DUSE_VENDORED_LIBS=1
	-DCMAKE_INSTALL_PREFIX=
)

if [ $TARGET == 'haiku' ]; then
	FOLDER="haiku"

	CMAKE_FLAGS+=(
		-DCMAKE_TOOLCHAIN_FILE="${SRCDIR}/packaging/toolchain-haiku.cmake"
	)
elif [ $TARGET == 'linux' ]; then
	export ARCH="$2"

	if [ "$ARCH" == "x86_64" ]; then
		FOLDER="lin_x86_64"
	elif [ "$ARCH" == "aarch64" ]; then
		FOLDER="lin_aarch64"
	fi

	if [ ! -f /tmp/appimagetool ]; then
		wget https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-${ARCH}.AppImage -O /tmp/appimagetool
		chmod +x /tmp/appimagetool
	fi
elif [ $TARGET == 'macos' ]; then
	FOLDER="macos"

	CMAKE_FLAGS+=(
		"-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"
		-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11
	)
elif [ $TARGET == 'vita' ]; then
	FOLDER="vita"

	CMAKE_FLAGS+=(
		-DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
	)
elif [ $TARGET == 'web' ]; then
	FOLDER="web"
elif [ $TARGET == 'win' ]; then
	ARCH="$2"

	if [ "$ARCH" == "32" ]; then
		FOLDER="win32"
		CCPREFIX=i686-w64-mingw32
	elif [ "$ARCH" == "arm64" ]; then
		FOLDER="winarm64"
		CCPREFIX=aarch64-w64-mingw32
	else
		FOLDER="win64"
		CCPREFIX=x86_64-w64-mingw32
	fi

	CMAKE_FLAGS+=(
		-DCMAKE_TOOLCHAIN_FILE="${SRCDIR}/packaging/toolchain-${CCPREFIX}.cmake"
	)
fi

BUILDDIR="/tmp/tensy/${FOLDER}"
BINDIR="${SRCDIR}/packaging/bin/${FOLDER}"

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

	/tmp/appimagetool --appimage-extract-and-run AppDir/
	mv *.AppImage bin/
fi
