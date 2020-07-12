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

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "colors.h"
#include "ui_events.h"

struct display_t;
struct display_calltable_t;

struct hmi_event_t {
	ui_event_cb_t event_callback;
	void *callback_ctx;
	pthread_t event_thread;
	bool thread_running;
};

struct display_t {
	unsigned int width;
	unsigned int height;
	unsigned int bits_per_pixel;
	const struct display_calltable_t *calltable;
	struct hmi_event_t hmi_events;
	uint8_t drv_context[];
};

struct display_calltable_t {
	bool (*init)(struct display_t *display, void *init_ctx);
	void (*free)(struct display_t *display);
	void (*fill)(struct display_t *display, uint32_t color);
	void (*put_pixel)(struct display_t *display, unsigned int x, unsigned int y, uint32_t color);
	void (*commit)(struct display_t *display);
	bool (*blit_buffer)(struct display_t *display, uint32_t *source, unsigned int width, unsigned int height);
	unsigned int (*get_ctx_size)(void);
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
struct display_t* display_init(const struct display_calltable_t *calltable, void *init_ctx);
void display_free(struct display_t *display);
void display_fill(struct display_t *display, uint32_t color);
void display_commit(struct display_t *display);
void display_put_pixel(struct display_t *display, unsigned int x, unsigned int y, uint32_t color);
void display_test(struct display_t *display);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
