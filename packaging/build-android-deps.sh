#!/bin/bash -e

TARGET=android

topdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $topdir/_common.sh

export PATH="$toolchain/bin:$ANDROID_NDK:$PATH"

CMAKE_FLAGS+=(
	-DCMAKE_INSTALL_PREFIX=
	-DCMAKE_TOOLCHAIN_FILE="${ANDROID_NDK}/build/cmake/android.toolchain.cmake"
	-DANDROID_NATIVE_API_LEVEL="$ANDROID_API"
)

build_sdl() {
	get_tar_archive SDL3 "${SDL3_url}"

	for ABI in "${ANDROID_ABIS[@]}"; do
		mkdir -p "build_$ABI" && cd "build_$ABI"
		cmake .. "${CMAKE_FLAGS[@]}" "${CMAKE_SDL_FLAGS[@]}" \
			-DSDL_{GPU,CAMERA,JOYSTICK,HAPTIC,POWER,SENSOR,VULKAN,TESTS}=OFF \
			-DANDROID_ABI="$ABI"
		ninja

		DESTDIR="$BINDIR/$ABI" ninja install
		cd ..
	done

	JAVA_DIR="$BINDIR/../app/src/main/java"
	cp -r android-project/app/src/main/java/org "$JAVA_DIR"
}

pushd "$BUILDDIR" || exit
build_sdl
popd || exit

exit 0
