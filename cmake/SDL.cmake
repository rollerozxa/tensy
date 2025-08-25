
download_dep_tarball(
	"SDL"
	"b13416d74f715e9f1d2396637d6fc3900130032c"
	"https://github.com/libsdl-org/SDL/archive/b13416d74f715e9f1d2396637d6fc3900130032c.tar.gz"
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(DISABLED_FEATURES CAMERA GPU HAPTIC JOYSTICK POWER SENSOR TESTS VULKAN)

if(LINUX)
	list(APPEND DISABLED_FEATURES WAYLAND)
endif()

if(ANDROID)

elseif(WIN32 OR LINUX OR EMSCRIPTEN)
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
