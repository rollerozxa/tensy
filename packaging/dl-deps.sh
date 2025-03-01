#!/bin/bash -eu

SDL3_tag=release-3.2.4
SDL3_url="https://github.com/libsdl-org/SDL/archive/${SDL3_tag}.tar.gz"
SDL3_mixer_tag=e97edec1902efe19ed95a50e9b25150408837004
SDL3_mixer_url="https://github.com/libsdl-org/SDL_mixer/archive/${SDL3_mixer_tag}.tar.gz"

SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

get_tar_archive() {
	# $1: folder to extract to, $2: version, $3: URL
	local filename="$1-$2.tar.gz"

	wget -nc -c "$3" -O "$filename" || echo 'ok, reusing archive'
	mkdir -p "$1"
	tar -xf "$filename" -C "$1" --strip-components=1
}

cd "$SRCDIR/lib"

get_tar_archive SDL "${SDL3_tag}" "${SDL3_url}"
get_tar_archive SDL_mixer "${SDL3_mixer_tag}" "${SDL3_mixer_url}"

JAVA_DIR="$SRCDIR/android/app/src/main/java"
cp -r SDL/android-project/app/src/main/java/org "$JAVA_DIR"
