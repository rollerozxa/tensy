cmake_minimum_required(VERSION 3.10)
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
	lib/*.c
)

if(WIN32)
	set(WINRESOURCE_FILE "packaging/tensy.rc")
	set(WINMANIFEST_FILE "packaging/tensy.manifest")

	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tensy.rc.o
		COMMAND ${CMAKE_RC_COMPILER} -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_CURRENT_BINARY_DIR}
		-i${WINRESOURCE_FILE}
		-o ${CMAKE_CURRENT_BINARY_DIR}/tensy.rc.o
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS ${WINRESOURCE_FILE} ${WINMANIFEST_FILE})
	set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/tensy.rc.o)

	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-mwindows -Wl,-s")
endif()

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} m ${SDL3_LIBRARIES})
