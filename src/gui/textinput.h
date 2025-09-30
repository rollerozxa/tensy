#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#define TEXTINPUT_BUFFER_SIZE 128

typedef struct {
    SDL_FRect rect;
    char buffer[TEXTINPUT_BUFFER_SIZE];
    int cursor_pos;
    bool _focused;
    bool _held;
} TextInput;

#define TEXTINPUT(obj, rect, initial_text) do { \
    obj = (TextInput){rect, {0}, 0, false, false}; \
    if (initial_text) { \
        strncpy(obj.buffer, initial_text, TEXTINPUT_BUFFER_SIZE - 1); \
        obj.cursor_pos = strlen(obj.buffer); \
    } \
} while (0)

void textinput_clear(TextInput *input);
void textinput_settext(TextInput *input, const char *text);
bool textinput_event(const SDL_Event *ev, TextInput *input);
void textinput(TextInput *input);
