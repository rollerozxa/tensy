
#ifdef SDL_PLATFORM_ANDROID
	#define ASSETLOADER_MUSIC(data, path) MIX_LoadAudio(mixer, "music/" path, false)
	#define ASSETLOADER_SOUND(data, path) MIX_LoadAudio(mixer, "sounds/" path, true)
	#define ASSETLOADER_QOI(data, path) SDL_LoadQOI("textures/" path)

	#define EMBEDDED_DATA false
#else
	#define ASSETLOADER_MUSIC(data, path) MIX_LoadAudio_IO(mixer, SDL_IOFromMem(data, data##_len), false, false)
	#define ASSETLOADER_SOUND(data, path) MIX_LoadAudio_IO(mixer, SDL_IOFromMem(data, data##_len), true, false)
	#define ASSETLOADER_QOI(data, path) SDL_LoadQOI_IO(SDL_IOFromMem(data, data##_len))

	#define EMBEDDED_DATA true
#endif
