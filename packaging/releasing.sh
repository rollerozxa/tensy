#!/bin/bash -eu

if [ -z "${GAME_VERSION}" ]; then
	echo "pls set GAME_VERSION variable" >&2
	exit 1
fi

download_artifact() {
	local workflow="$1"
	local artifact_id="$2"
	local final_filename="${3:-}"

	if [[ -z "$final_filename" ]]; then
		final_filename="$artifact_id"
	fi

	local tmpdir=$(mktemp -d)

	# Extremely slow for some reason:
	#gh run download --repo rollerozxa/tensy --name "$artifact_id" --dir "$tmpdir"

	# Also extremely slow for some reason:
	#curl -L "https://nightly.link/rollerozxa/tensy/workflows/${workflow}/master/${artifact_id}.zip" -o "$tmpdir/artifact.zip"
	#bsdtar -xf "$tmpdir/artifact.zip" -C "$tmpdir"
	#rm "$tmpdir/artifact.zip"

	# This works:
	wget "https://github.com/rollerozxa/tensy/releases/download/rolling/${artifact_id}" -O "$tmpdir/${artifact_id}"

	local extracted_file=$(find "$tmpdir" -type f | head -n 1)

	mv "$extracted_file" "./$final_filename"
}

download_artifacts() {
	download_artifact android tensy-android-unsigned.apk

	apksigner sign --ks ~/key.jks --ks-pass pass:everybodyvoteschannel --key-pass pass:everybodyvoteschannel --out tensy-android.apk tensy-android-unsigned.apk

	download_artifact haiku tensy-haiku

	download_artifact linux tensy-linux-x86_64
	download_artifact linux tensy-linux-aarch64
	download_artifact linux tensy-x86_64.AppImage
	download_artifact linux tensy-aarch64.AppImage

	download_artifact macos tensy-macos.zip

	download_artifact psvita tensy-vita.vpk

	download_artifact web tensy-web.zip

	download_artifact windows tensy-win32.exe
	download_artifact windows tensy-win64.exe
	download_artifact windows tensy-winarm32.exe
	download_artifact windows tensy-winarm64.exe
}

butler_push() {
	butler push --userversion="${GAME_VERSION}" "$2" rollerozxa/tensy:"$1"
	sleep 2
}

upload_to_itch() {
	butler_push android tensy-android.apk

	butler_push haiku tensy-haiku haiku

	butler_push linux-64 tensy-linux-x86_64
	butler_push linux-arm64 tensy-linux-aarch64
	butler_push linux-appimage-64 tensy-x86_64.AppImage
	butler_push linux-appimage-arm64 tensy-aarch64.AppImage

	butler_push macos tensy-macos.zip

	butler_push psvita tensy-vita.vpk

	butler_push web tensy-web.zip

	butler_push windows-32 tensy-win32.exe
	butler_push windows-64 tensy-win64.exe
	butler_push windows-arm32 tensy-winarm32.exe
	butler_push windows-arm64 tensy-winarm64.exe

}

mkdir -p dist
cd dist

download_artifacts
upload_to_itch
