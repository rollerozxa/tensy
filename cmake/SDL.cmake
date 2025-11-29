
set(DEP_SDL_VER "defd7895da3253064653dc56fb34f1b98b538aa4")
download_dep_tarball(
	"SDL"
	"${DEP_SDL_VER}"
	"https://github.com/libsdl-org/SDL/archive/${DEP_SDL_VER}.tar.gz"
	"a0a57364ad65a67ce49fb7614bd51cfa063a54d86645ef29b86788d506e1355e"
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

set(DISABLED_FEATURES CAMERA GPU HAPTIC HIDAPI POWER SENSOR TESTS VULKAN)

if(LINUX)
	list(APPEND DISABLED_FEATURES WAYLAND)
endif()

if(NOT VITA)
	list(APPEND DISABLED_FEATURES JOYSTICK)
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

add_definitions(-DSDL_LEAN_AND_MEAN=1)

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
