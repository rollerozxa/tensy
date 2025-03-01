#!/bin/bash -eu

build() {
	echo "Building $1..."

	pushd "$BUILDDIR" || exit
	"build_$1"
	popd || exit
}

mk_build_dir() {
	mkdir -p build
	cd build || exit
}

SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

if [ $TARGET == 'lin' ]; then
	BUILDDIR="/tmp/tensy/lin64"
	BINDIR="${SRCDIR}/packaging/bin/lin64"
elif [ $TARGET == 'web' ]; then
	BUILDDIR="/tmp/tensy/web"
	BINDIR="${SRCDIR}/packaging/bin/web"
elif [ $TARGET == 'win' ]; then
	ARCH="$1"
	shift

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
elif [ $TARGET == 'macos' ]; then
	BUILDDIR="/tmp/tensy/macos"
	BINDIR="${SRCDIR}/packaging/bin/macos"
elif [ $TARGET == 'haiku' ]; then
	BUILDDIR="/tmp/tensy/haiku"
	BINDIR="${SRCDIR}/packaging/bin/haiku"
elif [ $TARGET == 'vita' ]; then
	BUILDDIR="/tmp/tensy/vita"
	BINDIR="${SRCDIR}/packaging/bin/vita"
fi

mkdir -p "$BUILDDIR"
rm -rf "$BINDIR"
mkdir -p "$BINDIR"

CMAKE_FLAGS=(
	-GNinja
	-DUSE_VENDORED_LIBS=1
)

source "${SRCDIR}/packaging/dl-deps.sh"
