
#ifdef ASSETLOADER_LOOSE
	#ifdef SDL_PLATFORM_VITA
		#define PREFIX "app0:data/"
	#else
		#define PREFIX ""
	#endif

	#define ASSETLOADER_MUSIC(data, path) MIX_LoadAudio(mixer, PREFIX "music/" path, false)
	#define ASSETLOADER_SOUND(data, path) MIX_LoadAudio(mixer, PREFIX "sounds/" path, true)
	#define ASSETLOADER_QOI(data, path) SDL_LoadQOI(PREFIX "textures/" path)
	#define ASSETLOADER_PNG(data, path) SDL_LoadPNG(PREFIX "textures/" path)

	#define EMBEDDED_DATA false
#else
	#define ASSETLOADER_MUSIC(data, path) MIX_LoadAudio_IO(mixer, SDL_IOFromMem(data, data##_len), false, false)
	#define ASSETLOADER_SOUND(data, path) MIX_LoadAudio_IO(mixer, SDL_IOFromMem(data, data##_len), true, false)
	#define ASSETLOADER_QOI(data, path) SDL_LoadQOI_IO(SDL_IOFromMem(data, data##_len))
	#define ASSETLOADER_PNG(data, path) SDL_LoadPNG_IO(SDL_IOFromMem(data, data##_len), false)

	#define EMBEDDED_DATA true
#endif
