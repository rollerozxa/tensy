
set(DEP_LIBXMP_VER "4.6.3")
download_dep_tarball(
    "libxmp"
    "${DEP_LIBXMP_VER}"
    "https://github.com/libxmp/libxmp/releases/download/libxmp-${DEP_LIBXMP_VER}/libxmp-${DEP_LIBXMP_VER}.tar.gz"
)
