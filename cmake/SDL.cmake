
set(DEP_SDL_VER "ac3afa1f385518712ca58d818a58384d2cc3d683")
download_dep_tarball(
	"SDL"
	"${DEP_SDL_VER}"
	"https://github.com/libsdl-org/SDL/archive/${DEP_SDL_VER}.tar.gz"
	"746d9f7cdf48056bbb577fa87383423b4f15f8159c6988fe2bcc4b62027b4626"
)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(DISABLED_FEATURES CAMERA GPU HAPTIC POWER SENSOR TESTS VULKAN)

if(LINUX)
	list(APPEND DISABLED_FEATURES WAYLAND)
endif()

if(WIN32 OR LINUX OR APPLE)
	list(APPEND DISABLED_FEATURES OPENGLES)
endif()

foreach(feature ${DISABLED_FEATURES})
	set(SDL_${feature} OFF CACHE BOOL "" FORCE)
endforeach()

if(ANDROID OR HAIKU)
	enable_language(CXX)
endif()

if(HAIKU)
	add_definitions(-fPIC)
endif()

if(NOT NINTENDO_3DS)
	add_definitions(-DSDL_LEAN_AND_MEAN=1 -DSDL_HAVE_STB=1)
endif()

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
