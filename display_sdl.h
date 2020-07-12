/*
	minuitk - Minimalist UI toolkit
	Copyright (C) 2020-2020 Johannes Bauer

	This file is part of minuitk.

	minuitk is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; this program is ONLY licensed under
	version 3 of the License, later versions are explicitly excluded.

	minuitk is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

	Johannes Bauer <JohannesBauer@gmx.de>
*/

#ifndef __DISPLAY_SDL_H__
#define __DISPLAY_SDL_H__

#include <SDL2/SDL.h>
#include <pthread.h>
#include "display.h"

struct display_sdl_ctx_t {
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Renderer *renderer;
};

struct display_sdl_init_t {
	unsigned int width, height;
};

extern const struct display_calltable_t display_sdl_calltable;

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void display_sdl_register_events(struct display_t *display, ui_event_cb_t event_callback, void *ctx);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
