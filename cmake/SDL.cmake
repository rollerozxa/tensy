
set(DEP_SDL_VER "7d27ca282eb1ca85bdfd20d4e19227c63a3f122c")
download_dep_tarball(
	"SDL"
	"${DEP_SDL_VER}"
	"https://github.com/libsdl-org/SDL/archive/${DEP_SDL_VER}.tar.gz"
	"1f0e4124eedd5bb90554adedfea11d9e9a85888ab7fb03ee5fec8c6a5c878e30"
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

add_definitions(-DSDL_LEAN_AND_MEAN=1 -DSDL_HAVE_STB=1)

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
