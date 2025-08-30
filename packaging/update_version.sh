#!/bin/bash -eu

SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"
cd "$SRCDIR"

read_version_data() {
	VERSION_FILE="${SRCDIR}/packaging/version.json"
	VER_MAJOR=$(jq -r '.version.major' "$VERSION_FILE")
	VER_MINOR=$(jq -r '.version.minor' "$VERSION_FILE")
	VER_PATCH=$(jq -r '.version.patch' "$VERSION_FILE")
	VER_STRING=$(jq -r '.string' "$VERSION_FILE")
	VER_CODE=$(jq -r '.code' "$VERSION_FILE")
	VER_DEV=$(jq -r '.is_dev' "$VERSION_FILE")
}

update_android_version() {
	GRADLE_FILE="android/app/build.gradle"

	# Update version code and version name in gradle file
	sed -i -E "s/versionCode [0-9]+/versionCode $VER_CODE/" "$GRADLE_FILE"
	sed -i -E "s/versionName \"[^\"]+\"/versionName \"$VER_STRING\"/" "$GRADLE_FILE"
}

write_version_header() {
	HEADER_FILE="src/version_info.h"

	cat > "$HEADER_FILE" <<EOF
#pragma once

#define VER_MAJOR $VER_MAJOR
#define VER_MINOR $VER_MINOR
#define VER_PATCH $VER_PATCH
#define VER_DEV $VER_DEV

#define VER_STRING "$VER_STRING"
EOF

}

read_version_data
update_android_version
write_version_header
