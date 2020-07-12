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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "display_sdl.h"
#include "ui_events.h"

static void display_sdl_put_pixel(struct display_t *display, unsigned int x, unsigned int y, uint32_t rgb) {
	struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
	uint32_t *target = (uint32_t*)((uint8_t *)ctx->surface->pixels + (y * ctx->surface->pitch) + (x * sizeof(uint32_t)));
	*target = rgb;

}

static void display_sdl_commit(struct display_t *display) {
	struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
//	SDL_UpdateWindowSurface(ctx->window);			/* TODO: this causes flickering, is it needed? */
	SDL_RenderPresent(ctx->renderer);
}

static void display_sdl_fill(struct display_t *display, uint32_t rgb) {
	struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
	if (!ctx->renderer) {
		SDL_FillRect(ctx->surface, NULL, SDL_MapRGB(ctx->surface->format, GET_R(rgb), GET_G(rgb), GET_B(rgb)));
		SDL_UpdateWindowSurface(ctx->window);
	} else {
		SDL_SetRenderDrawColor(ctx->renderer, GET_R(rgb), GET_G(rgb), GET_B(rgb), 255);
		SDL_RenderClear(ctx->renderer);
	}
}




static void display_sdl_handle_event(struct display_t *display, SDL_Event *event) {
	if (!display->hmi_events.event_callback) {
		/* We cannot callback anyways, discard event */
		return;
	}

	if (event->type == SDL_WINDOWEVENT) {
		if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
			display->hmi_events.event_callback(EVENT_QUIT, NULL, display->hmi_events.callback_ctx);
		}
	} else if (event->type == SDL_KEYDOWN) {
		struct ui_event_keypress_t ui_event = {
			.sdl.key = event->key.keysym.sym,
			.sdl.mod = event->key.keysym.mod,
		};

		if (((ui_event.sdl.mod & KMOD_LCTRL) == KMOD_LCTRL) && (ui_event.sdl.key == SDLK_ESCAPE)) {
			/* Ctrl-Escape */
			display->hmi_events.event_callback(EVENT_QUIT, NULL, display->hmi_events.callback_ctx);
		}

#ifdef DEVELOPMENT
		if (ui_event.key == SDLK_F12) {
			/* Toggle fullscreen mode */
			static bool is_fullscreen = false;
			struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
			SDL_SetWindowFullscreen(ctx->window, is_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
			//SDL_SetWindowFullscreen(ctx->window, is_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
			is_fullscreen = !is_fullscreen;
		} else if (ui_event.key == SDLK_ESCAPE) {
			display->hmi_events.event_callback(EVENT_QUIT, NULL, display->hmi_events.callback_ctx);
		}
#endif

		display->hmi_events.event_callback(EVENT_KEYPRESS, &ui_event, display->hmi_events.callback_ctx);
	} else if (event->type == SDL_TEXTINPUT) {
		struct ui_event_textinput_t ui_event;
		strncpy(ui_event.text, event->text.text, sizeof(ui_event.text));
		display->hmi_events.event_callback(EVENT_TEXTINPUT, &ui_event, display->hmi_events.callback_ctx);
	} else {
		//printf("Unhandled event type 0x%x\n", event->type);
	}
}

static void* display_sdl_eventthread_fnc(void *vdisplay) {
	struct display_t *display = (struct display_t*)vdisplay;
	while (display->hmi_events.thread_running) {
		SDL_Event event;
		if (SDL_WaitEventTimeout(&event, 500)) {
			display_sdl_handle_event(display, &event);
		}
	}
	return NULL;
}

static bool display_sdl_init(struct display_t *display, void *init_ctx) {
	struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
	struct display_sdl_init_t *initctx = (struct display_sdl_init_t*)init_ctx;
	display->width = initctx->width;
	display->height = initctx->height;
	display->bits_per_pixel = 32;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		  fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
		  return false;
	}

	ctx->window = SDL_CreateWindow("Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display->width, display->height, SDL_WINDOW_SHOWN);
	if (!ctx->window) {
		fprintf(stderr, "Could not create SDL window: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
	if (!ctx->renderer) {
		fprintf(stderr, "Could not create SDL renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(ctx->window);
		SDL_Quit();
		return false;
	}

#ifndef DEVELOPMENT
	/* Disable mouse cursor if not developing */
	SDL_ShowCursor(SDL_DISABLE);
#endif

	SDL_StartTextInput();
//	SDL_SetWindowFullscreen(ctx->window, SDL_WINDOW_FULLSCREEN);

	ctx->surface = SDL_GetWindowSurface(ctx->window);
	return true;
}

static void display_sdl_free(struct display_t *display) {
	struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
	SDL_DestroyWindow(ctx->window);
	SDL_Quit();
}

static unsigned int display_sdl_get_ctx_size(void) {
	return sizeof(struct display_sdl_ctx_t);
}

void display_sdl_register_events(struct display_t *display, ui_event_cb_t event_callback, void *ctx) {
	display->hmi_events.event_callback = event_callback;
	display->hmi_events.callback_ctx = ctx;
	display->hmi_events.thread_running = true;
	pthread_create(&display->hmi_events.event_thread, NULL, display_sdl_eventthread_fnc, display);
}

/* When we use an accelerated renderer, we simply cannot directly access the
 * target surface anymore (it's implicit), therefore, blit_buffer *must* work
 */
static bool display_sdl_blit_buffer(struct display_t *display, uint32_t *source, unsigned int width, unsigned int height) {
	struct display_sdl_ctx_t *ctx = (struct display_sdl_ctx_t*)display->drv_context;
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(source, width, height, 32, 4 * width, COLOR_RMASK, COLOR_GMASK, COLOR_BMASK, 0);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(ctx->renderer, surface);
	if (surface) {
		SDL_RenderCopy(ctx->renderer, texture, NULL, NULL);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
		return true;
	} else {
		return false;
	}
}

const struct display_calltable_t display_sdl_calltable = {
	.init = display_sdl_init,
	.free = display_sdl_free,
	.fill = display_sdl_fill,
	.commit = display_sdl_commit,
	.put_pixel = display_sdl_put_pixel,
	.get_ctx_size = display_sdl_get_ctx_size,
	.blit_buffer = display_sdl_blit_buffer,
};
