
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

foreach(feature GME MOD MIDI OPUS FLAC MP3 WAVE WAVPACK SAMPLES)
    set(SDLMIXER_${feature} OFF CACHE BOOL "" FORCE)
endforeach()

add_subdirectory(lib/SDL_mixer EXCLUDE_FROM_ALL)
