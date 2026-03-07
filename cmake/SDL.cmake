
set(DEP_SDL_VER "89393c5519d8cbdc0e3f16a387c37cac2642d0a1")
download_dep_tarball(
	"SDL"
	"${DEP_SDL_VER}"
	"https://github.com/libsdl-org/SDL/archive/${DEP_SDL_VER}.tar.gz"
	"0e433088d666cadc0206e64aa48ee38fd7fbc964858e4676d67d043d67207c28"
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
	add_definitions(-DSDL_LEAN_AND_MEAN=1)
endif()

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
