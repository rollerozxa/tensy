#!/bin/bash -eu

TARGET=android

source $(dirname "${BASH_SOURCE[0]}")/_common.sh

export PATH="$ANDROID_NDK:$PATH"

CMAKE_FLAGS+=(
	-DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK}/build/cmake/android.toolchain.cmake"
	-DANDROID_NATIVE_API_LEVEL="$ANDROID_API"
)

iterate_abis() {
	for ABI in "${ANDROID_ABIS[@]}"; do
		mkdir -p "build_$ABI" && cd "build_$ABI"
		"$1" "$ABI"
		ninja

		DESTDIR="$BINDIR/$ABI" ninja install
		cd ..
	done
}

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	conf_sdl() {
		cmake .. "${CMAKE_FLAGS[@]}" "${SDL_FLAGS[@]}" \
			-DANDROID_ABI="$ABI"
	}
	iterate_abis conf_sdl

	JAVA_DIR="$BINDIR/../app/src/main/java"
	cp -r android-project/app/src/main/java/org "$JAVA_DIR"
}

build_sdl_mixer() {
	get_tar_archive SDL3_mixer "${SDL3_mixer_url}"

	conf_sdl_mixer() {
		cmake .. "${CMAKE_FLAGS[@]}" "${SDL_MIXER_FLAGS[@]}" \
			-DSDL3_DIR="$BINDIR/$ABI/lib/cmake/SDL3" \
			-DANDROID_ABI="$ABI"
	}
	iterate_abis conf_sdl_mixer
}

build sdl
build sdl_mixer
