#!/bin/bash
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

SDK="${IOS_SDK:-iphoneos}"
ARCH="${IOS_ARCH:-arm64}"
DEPLOYMENT_TARGET="${IOS_DEPLOYMENT_TARGET:-15.0}"

mkdir -p build_ios; cd build_ios
cmake .. \
	-DCMAKE_SYSTEM_NAME=iOS \
	-DCMAKE_SYSTEM_PROCESSOR="${ARCH}" \
	-DCMAKE_OSX_SYSROOT="${SDK}" \
	-DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
	-DCMAKE_OSX_DEPLOYMENT_TARGET="$DEPLOYMENT_TARGET" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_FLAGS="-Wno-deprecated-declarations" \
	-DUSE_VENDORED_LIBS=1 \
	-G Ninja
ninja

APP="$PWD/tensy.app"
IOS="$ROOT/packaging/ios"
ASSETS="$PWD/Assets.xcassets"
ASSET_BUILD="$PWD/asset-build"
PARTIAL_PLIST="$PWD/assetcatalog-partial.plist"

rm -rf "$PWD/data"
cp -r ../data "$APP"

cp "$IOS/Info.plist" "$APP"

rm -rf "$ASSETS"; mkdir -p "$ASSETS"

cp -R "$IOS/AppIcon.appiconset" "$ASSETS/AppIcon.appiconset"

rm -rf "$ASSET_BUILD"
rm -rf "$PARTIAL_PLIST"

mkdir -p "$ASSET_BUILD"

xcrun actool \
	--compile "$ASSET_BUILD" \
	--platform iphoneos \
	--minimum-deployment-target $DEPLOYMENT_TARGET \
	--target-device iphone \
	--target-device ipad \
	--app-icon AppIcon \
	--output-partial-info-plist "$PARTIAL_PLIST" \
	"$ASSETS"

cp "$ASSET_BUILD/"* "$APP"

rm -rf package
mkdir -p package/Payload
ditto "$APP" package/Payload/tensy.app
cd package
zip -qry "$ROOT/Tensy-$SDK-$ARCH-$DEPLOYMENT_TARGET.ipa" Payload
