#include "SDL_QOI.h"
#include <limits.h>

#define QOI_MALLOC(sz) SDL_malloc(sz)
#define QOI_FREE(p)    SDL_free(p)
#define QOI_ZEROARR(a) SDL_zeroa(a)

#define QOI_NO_STDIO
#define QOI_IMPLEMENTATION
#include "qoi.h"

SDL_Surface *SDL_LoadQOI_IO(SDL_IOStream *src)
{
    void *data;
    size_t size;
    void *pixel_data;
    qoi_desc image_info;
    SDL_Surface *surface = NULL;

    data = (void *)SDL_LoadFile_IO(src, &size, false);
    if ( !data ) {
        return NULL;
    }
    if ( size > INT_MAX ) {
        SDL_free(data);
        SDL_SetError("QOI image is too big.");
        return NULL;
    }

    pixel_data = qoi_decode(data, (int)size, &image_info, 4);
    /* pixel_data is in R,G,B,A order regardless of endianness */
    SDL_free(data);
    if ( !pixel_data ) {
        SDL_SetError("Couldn't parse QOI image");
        return NULL;
    }

    surface = SDL_CreateSurfaceFrom(image_info.width,
                                    image_info.height,
                                    SDL_PIXELFORMAT_RGBA32,
                                    pixel_data,
                                    (image_info.width * 4));
    if ( !surface ) {
        QOI_FREE(pixel_data);
        SDL_SetError("Couldn't create SDL_Surface");
        return NULL;
    }

    /* Let SDL manage the memory now */
    surface->flags &= ~SDL_SURFACE_PREALLOCATED;

    return surface;
}
