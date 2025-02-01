#!/bin/bash -eu

SDL3_tag=release-3.2.0
SDL3_url="https://github.com/libsdl-org/SDL/archive/${SDL3_tag}.tar.gz"
SDL3_mixer_tag=af6a29df4e14c6ce72608b3ccd49cf35e1014255
SDL3_mixer_url="https://github.com/libsdl-org/SDL_mixer/archive/${SDL3_mixer_tag}.tar.gz"

build() {
	echo "Building $1..."

	pushd "$BUILDDIR" || exit
	"build_$1"
	popd || exit
}

get_tar_archive() {
	# $1: folder to extract to, $2: URL
	local filename="${2##*/}"

	wget -nc -c "$2" -O "../$filename" || echo 'ok, reusing archive'
	mkdir -p "$1"
	tar -xf "../$filename" -C "$1" --strip-components=1
	cd "$1" || exit
}

mk_build_dir() {
	mkdir -p build
	cd build || exit
}

dep_ninja_install() {
	ninja
	DESTDIR="$BUILDDIR/deps" ninja install
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
elif [ $TARGET == 'android' ]; then
	BUILDDIR="/tmp/tensy/android"
	BINDIR="${SRCDIR}/android/deps"

	ANDROID_ABIS=("arm64-v8a" "armeabi-v7a" "x86_64")
	ANDROID_API=21
elif [ $TARGET == 'macos' ]; then
	BUILDDIR="/tmp/tensy/macos"
	BINDIR="${SRCDIR}/packaging/bin/macos"
elif [ $TARGET == 'vita' ]; then
	BUILDDIR="/tmp/tensy/vita"
	BINDIR="${SRCDIR}/packaging/bin/vita"
fi

mkdir -p "$BUILDDIR"
rm -rf "$BINDIR"
mkdir -p "$BINDIR"

CMAKE_FLAGS=(
	-GNinja
	-DCMAKE_BUILD_TYPE=Release
	-DCMAKE_INSTALL_PREFIX=
)

SDL3_DIR=(-DSDL3_DIR="${BUILDDIR}/deps/lib/cmake/SDL3")
SDL3_MIXER_DIR=(-DSDL3_mixer_DIR="${BUILDDIR}/deps/lib/cmake/SDL3_mixer")

# Common SDL flags for most platforms
SDL_FLAGS=(
	-DSDL_SHARED=OFF -DSDL_STATIC=ON
	-DCMAKE_C_FLAGS="-DSDL_LEAN_AND_MEAN=1"
	-DSDL_{CAMERA,GPU,HAPTIC,JOYSTICK,POWER,SENSOR,TESTS,VULKAN}=OFF
)

SDL_MIXER_FLAGS=(
	"${SDL3_DIR}"
	-DBUILD_SHARED_LIBS=OFF
	-DSDLMIXER_{GME,MOD,MIDI,OPUS,FLAC,MP3,WAVE,WAVPACK,SAMPLES}=OFF
)

GAME_FLAGS=(
	"${SDL3_DIR}"
	"${SDL3_MIXER_DIR}"
)
