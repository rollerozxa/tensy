#!/bin/bash -e

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
