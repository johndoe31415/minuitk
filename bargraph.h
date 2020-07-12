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

#ifndef __BARGRAPH_H__
#define __BARGRAPH_H__

#include <stdint.h>
#include "colors.h"
#include "cairo.h"

struct bargraph_data_t {
	unsigned int point_count;
	float *data;
};

typedef uint32_t (*bargraph_color_callback_t)(unsigned int x, float y);

struct bargraph_options_t {
	float min_y, max_y;
	bool autoscale_min_y, autoscale_max_y;
	bargraph_color_callback_t color_callback;
	unsigned int xvalue_count;
	float spacing_ratio;
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void bargraph_draw(struct cairo_swbuf_t *swbuf, const struct bargraph_options_t *options, const struct bargraph_data_t *data);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
