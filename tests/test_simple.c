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
#include <unistd.h>
#include <minuitk.h>

static void event_callback(enum ui_eventtype_t event_type, void *event, void *ctx) {
	if (event_type == EVENT_QUIT) {
		exit(0);
	}
}

int main(int argc, char **argv) {
	struct display_sdl_init_t init_params = {
		.width = 800, .height = 600,
	};

	struct display_t *display = display_init(&display_sdl_calltable, &init_params);
	if (!display) {
		fprintf(stderr, "Couldn't initialize display.\n");
		return 1;
	}

	display_sdl_register_events(display, event_callback, NULL);

	struct cairo_swbuf_t *swbuf = create_swbuf(display->width, display->height);

	int counter = 0;
	while (true) {
		counter++;
		swbuf_clear(swbuf, COLOR_BLACK);
		swbuf_text(swbuf, &(const struct font_placement_t){
			.font_size = 48,
			.font_color = COLOR_CARROT,
		}, "Test %d!", counter);

		blit_swbuf_on_display(swbuf, display);
		display_commit(display);
		sleep(1);
	}
	return 0;
}
