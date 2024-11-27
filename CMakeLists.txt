cmake_minimum_required(VERSION 3.14)
project(tensy C)
set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, default to Release")
	set(CMAKE_BUILD_TYPE "Release")
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
)

if(WIN32)
	set(SRCS ${SRCS} packaging/tensy.rc)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-mwindows -Wl,-s")
endif()

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} m ${SDL3_LIBRARIES})
