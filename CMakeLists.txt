cmake_minimum_required(VERSION 3.14)
project(tensy C)
set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(ANDROID)
	set(SDL3_DIR "android/deps/${ANDROID_ABI}/lib/cmake/SDL3")
	set(EXTRA_LIBRARIES -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid)
endif()

find_package(SDL3 REQUIRED)

include_directories(
	src/
	lib/
)

include_directories(${SDL3_INCLUDE_DIRS})

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

target_link_libraries(${PROJECT_NAME} m ${SDL3_LIBRARIES} ${EXTRA_LIBRARIES})
