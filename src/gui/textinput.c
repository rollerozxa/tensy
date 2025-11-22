#include "textinput.h"
#include "color.h"
#include "consts.h"
#include "draw.h"
#include "font.h"
#include "media/sound.h"
#include "mouse.h"
#include "text.h"

#include <SDL3/SDL.h>
#include <string.h>

extern SDL_Window *window;

static void textinput_start(void) {
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetNumberProperty(props, SDL_PROP_TEXTINPUT_CAPITALIZATION_NUMBER, SDL_CAPITALIZE_NONE);
	SDL_SetBooleanProperty(props, SDL_PROP_TEXTINPUT_AUTOCORRECT_BOOLEAN, false);
	SDL_StartTextInputWithProperties(window, props);
	SDL_DestroyProperties(props);
}

void textinput_clear(TextInput *input) {
	input->buffer[0] = '\0';
	input->cursor_pos = 0;
}

void textinput_settext(TextInput *input, const char *text) {
	strncpy(input->buffer, text, 12); // XXX
	input->cursor_pos = strnlen(input->buffer, TEXTINPUT_BUFFER_SIZE);
}

bool textinput_event(const SDL_Event *ev, TextInput *input) {

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (SDL_PointInRectFloat(&POINT(ev->motion.x, ev->motion.y), &input->rect)) {
			if (!input->_focused) {
				input->_focused = true;
				textinput_start();
				sound_play(SND_CLICK);
			}
			input->_held = true;
			return true;
		} else if (input->_focused) {
			input->_focused = false;
			SDL_StopTextInput(window);
			return true;
		}
	}

	if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP && input->_held)
		input->_held = false;

	if (!input->_focused)
		return false;

	// Handle text input when focused
	if (ev->type == SDL_EVENT_TEXT_INPUT) {
		int len = strlen(input->buffer);
		int text_len = strlen(ev->text.text);

		// Make sure there's enough space in the buffer
		if (len + text_len <= SDL_min(TEXTINPUT_BUFFER_SIZE - 1, input->max_length)) {
			// Make room for insertion
			memmove(input->buffer + input->cursor_pos + text_len,
					input->buffer + input->cursor_pos,
					len - input->cursor_pos + 1);

			// Insert the text
			memcpy(input->buffer + input->cursor_pos, ev->text.text, text_len);
			input->cursor_pos += text_len;
			return true;
		}
	} else if (ev->type == SDL_EVENT_KEY_DOWN) {
		switch (ev->key.key) {
			case SDLK_BACKSPACE:
				// Remove character before cursor
				if (input->cursor_pos > 0) {
					int len = strlen(input->buffer);
					memmove(input->buffer + input->cursor_pos - 1,
							input->buffer + input->cursor_pos,
							len - input->cursor_pos + 1);
					input->cursor_pos--;
				}
				return true;

			case SDLK_DELETE:
				// Remove character after cursor
				if (input->cursor_pos < (int)strlen(input->buffer)) {
					int len = strlen(input->buffer);
					memmove(input->buffer + input->cursor_pos,
							input->buffer + input->cursor_pos + 1,
							len - input->cursor_pos);
				}
				return true;

			case SDLK_LEFT:
				// Move caret to left
				if (input->cursor_pos > 0)
					input->cursor_pos--;

				return true;

			case SDLK_RIGHT:
				// Move caret to right
				if (input->cursor_pos < (int)strlen(input->buffer))
					input->cursor_pos++;

				return true;

			case SDLK_HOME:
				// Move caret to start
				input->cursor_pos = 0;
				return true;

			case SDLK_END:
				// Move caret to end
				input->cursor_pos = strlen(input->buffer);
				return true;

			case SDLK_RETURN:
			case SDLK_KP_ENTER:
				// Unfocus on Enter
				input->_focused = false;
				SDL_StopTextInput(window);
				return true;

			default:
				break;
		}
	}

	return false;
}

void textinput(TextInput *input) {
	SDL_FRect rect = input->rect;

	SDL_FPoint mouse;
	mouse_get_state_scaled(&mouse);

	draw_box_disabled(&rect);

	float text_height = CENTER(rect.h, text_calc_rect("e", 2.0f).h);

	font_set_color(CLR_WHITE);
	text_draw_shadow(input->buffer, rect.x + 5, rect.y + text_height, 2.0f);

	if (!input->_focused)
		return;

	char temp_buffer[TEXTINPUT_BUFFER_SIZE];
	strncpy(temp_buffer, input->buffer, input->cursor_pos);
	temp_buffer[input->cursor_pos] = '\0';

	SDL_FRect cursor_rect = text_calc_rect(temp_buffer, 2.0f);
	float cursor_x = rect.x + 5 + cursor_rect.w;

	draw_set_color(0xFFFFFF);
	SDL_FRect cursor_line = {cursor_x, rect.y + 5, 2, rect.h - 10};

	if ((SDL_GetTicks() / 500) % 2 == 0)
		draw_fill_rect(&cursor_line);
}
