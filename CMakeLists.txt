cmake_minimum_required(VERSION 3.14)
project(tensy C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(ANDROID)
	set(SDL3_DIR "android/deps/${ANDROID_ABI}/lib/cmake/SDL3")
	set(SDL3_mixer_DIR "android/deps/${ANDROID_ABI}/lib/cmake/SDL3_mixer")
	set(EXTRA_LIBRARIES -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid)
endif()

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)

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

if(WIN32)
	set(SRCS ${SRCS} packaging/tensy.rc)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-mwindows -Wl,-s")
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
endif()

# Generate embedded data files as header files to be included
include(GenerateDataFiles)
generate_data_files()
add_custom_target(generate_data DEPENDS ${GENERATED_DATA_HEADERS})
add_dependencies(${PROJECT_NAME} generate_data)
