
download_dep_tarball(
    "libxmp"
    "4.6.3"
    "https://github.com/libxmp/libxmp/releases/download/libxmp-4.6.3/libxmp-4.6.3.tar.gz"
)

file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/lib/SDL_mixer/external/libxmp")
file(CREATE_LINK
    "${CMAKE_SOURCE_DIR}/lib/libxmp"
    "${CMAKE_SOURCE_DIR}/lib/SDL_mixer/external/libxmp"
    SYMBOLIC
)
