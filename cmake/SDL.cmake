
set(DEP_SDL_VER "25d9096d41931f67add5dceefb587a8d7e51e0a2")

download_dep_tarball(
	"SDL"
	"${DEP_SDL_VER}"
	"https://github.com/libsdl-org/SDL/archive/${DEP_SDL_VER}.tar.gz"
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(DISABLED_FEATURES CAMERA GPU HAPTIC JOYSTICK POWER SENSOR TESTS VULKAN)

if(LINUX)
	list(APPEND DISABLED_FEATURES WAYLAND)
endif()

if(ANDROID OR WIN32 OR LINUX OR EMSCRIPTEN)
	list(APPEND DISABLED_FEATURES HIDAPI)
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

if(NOT WINDOWS_XP)
	add_definitions(-DSDL_LEAN_AND_MEAN=1)
endif()

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
