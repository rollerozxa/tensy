cmake_minimum_required(VERSION 3.14)
project(tensy C)
set(CMAKE_C_STANDARD 11)
set(PROJ ${PROJECT_NAME})

include(cmake/DownloadLib.cmake)

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

option(USE_VENDORED_LIBS "Build with vendored libraries for SDL and SDL_mixer" TRUE)

option(FORCE_LOOSE_ASSET_LOADING "Force loose asset loading (no embedded assets)" FALSE)
mark_as_advanced(FORCE_LOOSE_ASSET_LOADING)

if(USE_VENDORED_LIBS)
	include(cmake/SDL.cmake)
	include(cmake/libxmp.cmake)
	include(cmake/SDL_mixer.cmake)
else()
	find_package(SDL3 REQUIRED)
	find_package(SDL3_mixer REQUIRED)
endif()

include_directories(
	src/
	lib/
	${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SRCS CONFIGURE_DEPENDS
	src/*.c
	src/gui/*.c
	src/media/*.c
	src/modes/*.c
	src/overlays/*.c
	src/scenes/*.c
	lib/perlin/*.c
	lib/SDL_QOI/*.c)

if(WIN32)
	set(SRCS ${SRCS} packaging/tensy.rc)
endif()

# Add executable and linked libraries

if(ANDROID)
	add_library(${PROJ} SHARED ${SRCS})
else()
	add_executable(${PROJ} ${SRCS})
endif()

target_link_libraries(${PROJ} m SDL3_mixer::SDL3_mixer SDL3::SDL3)
if(ANDROID)
	target_link_libraries(${PROJ} -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid)
endif()

# Compiler flags

if(NOT VITA AND NOT NINTENDO_3DS)
	target_link_options(${PROJ} PRIVATE $<$<CONFIG:Release>:-Wl,-s>)
endif()

if(WIN32)
	target_link_options(${PROJ} PRIVATE $<$<CONFIG:Release>:-mwindows>)

	if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
		target_link_options(${PROJ} PRIVATE -Wl,--major-os-version=5,--major-subsystem-version=5)
		add_compile_definitions(WINVER=0x0501 _WIN32_WINNT=0x0501)
	endif()
endif()

target_compile_definitions(${PROJ} PRIVATE $<$<CONFIG:Debug>:DEBUG=1>)

set(GENERATE_DATA_FILES TRUE)
if(ANDROID OR VITA OR APPLE OR FORCE_LOOSE_ASSET_LOADING)
	add_compile_definitions(ASSETLOADER_LOOSE=1)
	set(GENERATE_DATA_FILES FALSE)
endif()

# Nice set of warning flags to build with
target_compile_options(${PROJ} PRIVATE -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-missing-field-initializers)

# Packaging

if(VITA)
	include("${VITASDK}/share/vita.cmake" REQUIRED)
	vita_create_self(${PROJ}.self ${PROJ})
	vita_create_vpk(${PROJ}.vpk OZXA00010 ${PROJ}.self
		VERSION "00.00"
		NAME Tensy
		FILE data/ data/
		FILE packaging/vita/icon.png sce_sys/icon0.png
		FILE packaging/vita/bg.png sce_sys/livearea/contents/bg.png
		FILE packaging/vita/startup.png sce_sys/livearea/contents/startup.png
		FILE packaging/vita/template.xml sce_sys/livearea/contents/template.xml
	)
	install(FILES "${CMAKE_BINARY_DIR}/tensy.vpk" DESTINATION .)
elseif(NINTENDO_3DS)
	ctr_create_3dsx(${PROJ})
elseif(APPLE)
	set(BUNDLE_PATH "${PROJ}.app/Contents")

	install(DIRECTORY "data/" DESTINATION "${BUNDLE_PATH}/Resources")
	install(FILES "packaging/tensy.icns" DESTINATION "${BUNDLE_PATH}/Resources")
	install(FILES "packaging/Info.plist" DESTINATION "${BUNDLE_PATH}")
	install(FILES "packaging/PkgInfo" DESTINATION "${BUNDLE_PATH}")
	install(TARGETS ${PROJ} DESTINATION "${BUNDLE_PATH}/MacOS")
else()
	if(EMSCRIPTEN)
		install(FILES "packaging/index.html" DESTINATION .)
		install(FILES ${CMAKE_BINARY_DIR}/${PROJ}.wasm DESTINATION .)
	endif()

	install(TARGETS ${PROJ} DESTINATION .)
endif()

# Generate embedded data files as header files to be included

if(GENERATE_DATA_FILES)
	include(cmake/GenerateDataFiles.cmake)
	generate_data_files()
	add_custom_target(generate_data DEPENDS ${GENERATED_DATA_HEADERS})
	add_dependencies(${PROJ} generate_data)
endif()
