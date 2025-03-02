cmake_minimum_required(VERSION 3.14)
project(tensy C)
set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(ANDROID)
	set(EXTRA_LIBRARIES -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid)
endif()

option(USE_VENDORED_LIBS "Build with vendored libraries for SDL and such" FALSE)

if(USE_VENDORED_LIBS)
	include(cmake/SDL.cmake)
	include(cmake/SDL_mixer.cmake)
else()
	find_package(SDL3 REQUIRED)
	find_package(SDL3_mixer REQUIRED)
endif()

include_directories(
	src/
	lib/
	${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SRCS
	src/*.c
	src/gui/*.c
	lib/*.c
	lib/SDL_QOI/*.c
)

if(NOT VITA)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-s")
endif()

if(WIN32)
	set(SRCS ${SRCS} packaging/tensy.rc)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mwindows")
endif()

if(ANDROID)
	add_library(${PROJECT_NAME} SHARED ${SRCS})
else()
	add_executable(${PROJECT_NAME} ${SRCS})
endif()

target_link_libraries(${PROJECT_NAME}
	m
	SDL3_mixer::SDL3_mixer
	SDL3::SDL3
	${EXTRA_LIBRARIES})

if(VITA)
	include("${VITASDK}/share/vita.cmake" REQUIRED)
	vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
	vita_create_vpk(${PROJECT_NAME}.vpk OZXA00010 ${PROJECT_NAME}.self
		VERSION "00.00"
		NAME Tensy
		FILE packaging/vita/icon.png sce_sys/icon0.png
		FILE packaging/vita/bg.png sce_sys/livearea/contents/bg.png
		FILE packaging/vita/startup.png sce_sys/livearea/contents/startup.png
		FILE packaging/vita/template.xml sce_sys/livearea/contents/template.xml
	)
	install(FILES "${CMAKE_BINARY_DIR}/tensy.vpk" DESTINATION .)
endif()

if(APPLE)
	set(BUNDLE_PATH "${PROJECT_NAME}.app/Contents")

	install(FILES "packaging/tensy.icns" DESTINATION "${BUNDLE_PATH}/Resources")
	install(FILES "packaging/Info.plist" DESTINATION "${BUNDLE_PATH}")
	install(FILES "packaging/PkgInfo" DESTINATION "${BUNDLE_PATH}")
	install(TARGETS ${PROJECT_NAME} DESTINATION "${BUNDLE_PATH}/MacOS")
elseif(NOT VITA)
	if(EMSCRIPTEN)
		install(FILES "packaging/index.html" DESTINATION .)
		install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.wasm DESTINATION .)
	endif()

	install(TARGETS ${PROJECT_NAME} DESTINATION .)
endif()

# Generate embedded data files as header files to be included
include(cmake/GenerateDataFiles.cmake)
generate_data_files()
add_custom_target(generate_data DEPENDS ${GENERATED_DATA_HEADERS})
add_dependencies(${PROJECT_NAME} generate_data)
