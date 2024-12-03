#!/bin/bash -e

SDL3_tag=preview-3.1.6
SDL3_url="https://github.com/libsdl-org/SDL/archive/${SDL3_tag}.tar.gz"

get_tar_archive() {
	# $1: folder to extract to, $2: URL
	local filename="${2##*/}"

	wget -nc -c "$2" -O "../$filename" || echo 'ok, reusing archive'
	mkdir -p "$1"
	tar -xaf "../$filename" -C "$1" --strip-components=1
	cd "$1" || exit
}

mk_build_dir() {
	mkdir -p build
	cd build || exit
}

if [ $TARGET == 'lin' ]; then
	BUILDDIR="/tmp/tensy/lin64"
	BINDIR="$topdir/bin/lin64"
elif [ $TARGET == 'web' ]; then
	BUILDDIR="/tmp/tensy/web"
	BINDIR="$topdir/bin/web"
elif [ $TARGET == 'win' ]; then
	ARCH="$1"
	shift

	if [ "$ARCH" == "32" ]; then
		BUILDDIR="/tmp/tensy/win32"
		BINDIR="$topdir/bin/win32"
		CCPREFIX=i686-w64-mingw32
	elif [ "$ARCH" == "arm64" ]; then
		BUILDDIR="/tmp/tensy/winarm64"
		BINDIR="$topdir/bin/winarm64"
		CCPREFIX=aarch64-w64-mingw32
	else
		BUILDDIR="/tmp/tensy/win64"
		BINDIR="$topdir/bin/win64"
		CCPREFIX=x86_64-w64-mingw32
	fi
fi

mkdir -p "$BUILDDIR"
rm -rf "$BINDIR"
mkdir -p "$BINDIR"

CMAKE_FLAGS=(
	-GNinja
	-DCMAKE_BUILD_TYPE=Release
)

# Common SDL flags for most platforms
CMAKE_SDL_FLAGS=(
	-DSDL_SHARED=OFF -DSDL_STATIC=ON
	-DCMAKE_C_FLAGS="-DSDL_LEAN_AND_MEAN=1"
)
