#ifndef __MINUITK_H__
#define __MINUITK_H__

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <cairo/cairo.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#define UINT8(x)					((x) & 0xff)
#define GET_R(rgb)					UINT8((rgb) >> 16)
#define GET_G(rgb)					UINT8((rgb) >> 8)
#define GET_B(rgb)					UINT8((rgb) >> 0)

#define COLOR_RMASK					0x00ff0000
#define COLOR_GMASK					0x0000ff00
#define COLOR_BMASK					0x000000ff

#define MK_RGB(r, g, b)				((UINT8(r) << 16) | (UINT8(g) << 8) | (UINT8(b) << 0))

#define COLOR_BLACK					MK_RGB(0x00, 0x00, 0x00)
#define COLOR_RED					MK_RGB(0xff, 0x00, 0x00)
#define COLOR_GREEN					MK_RGB(0x00, 0xff, 0x00)
#define COLOR_BLUE					MK_RGB(0x00, 0x00, 0xff)
#define COLOR_WHITE					MK_RGB(0xff, 0xff, 0xff)
#define COLOR_GRAY					MK_RGB(0x80, 0x80, 0x80)
#define COLOR_BS_RED				MK_RGB(0xdf, 0x20, 0x33)
#define COLOR_BS_BLUE				MK_RGB(0x38, 0x6c, 0xb6)
#define COLOR_BS_DARKBLUE			MK_RGB(0x0c, 0x1d, 0x2f)

#define COLOR_TURQUOISE				MK_RGB(0x1a, 0xbc, 0x9c)
#define COLOR_GREEN_SEA				MK_RGB(0x16, 0xa0, 0x85)
#define COLOR_EMERLAND				MK_RGB(0x2e, 0xcc, 0x71)
#define COLOR_NEPHRITIS				MK_RGB(0x27, 0xae, 0x60)
#define COLOR_PETER_RIVER			MK_RGB(0x34, 0x98, 0xdb)
#define COLOR_BELIZE_HOLE			MK_RGB(0x29, 0x80, 0xb9)
#define COLOR_AMETHYST				MK_RGB(0x9b, 0x59, 0xb6)
#define COLOR_WISTERIA				MK_RGB(0x8e, 0x44, 0xad)
#define COLOR_WET_ASPHALT			MK_RGB(0x34, 0x49, 0x5e)
#define COLOR_MIDNIGHT_BLUE			MK_RGB(0x2c, 0x3e, 0x50)
#define COLOR_SUN_FLOWER			MK_RGB(0xf1, 0xc4, 0x0f)
#define COLOR_ORANGE				MK_RGB(0xf3, 0x9c, 0x12)
#define COLOR_CARROT				MK_RGB(0xe6, 0x7e, 0x22)
#define COLOR_PUMPKIN				MK_RGB(0xd3, 0x54, 0x00)
#define COLOR_ALIZARIN				MK_RGB(0xe7, 0x4c, 0x3c)
#define COLOR_POMEGRANATE			MK_RGB(0xc0, 0x39, 0x2b)
#define COLOR_CLOUDS				MK_RGB(0xec, 0xf0, 0xf1)
#define COLOR_SILVER				MK_RGB(0xbd, 0xc3, 0xc7)
#define COLOR_CONCRETE				MK_RGB(0x95, 0xa5, 0xa6)
#define COLOR_ASBESTOS				MK_RGB(0x7f, 0x8c, 0x8d)

enum ui_eventtype_t {
	EVENT_QUIT,
	EVENT_KEYPRESS,
	EVENT_TEXTINPUT
};

struct ui_event_sdl_keypress_t {
	SDL_Keycode key;
	uint16_t mod;
};

struct ui_event_keypress_t {
	union {
		struct ui_event_sdl_keypress_t sdl;
	};
};

struct ui_event_textinput_t {
	char text[32];
};

typedef void (*ui_event_cb_t)(enum ui_eventtype_t event_type, void *event, void *ctx);

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

struct cairo_swbuf_t {
	cairo_surface_t *surface;
	cairo_t *ctx;
	unsigned int width, height;
};

enum xanchor_t {
	XPOS_LEFT,
	XPOS_CENTER,
	XPOS_RIGHT,
};

enum yanchor_t {
	YPOS_TOP,
	YPOS_CENTER,
	YPOS_BOTTOM
};

struct anchored_placement_t {
	int xoffset, yoffset;
	struct {
		enum xanchor_t x;
		enum yanchor_t y;
	} dst_anchor;
	struct {
		enum xanchor_t x;
		enum yanchor_t y;
	} src_anchor;
};

struct font_placement_t {
	struct anchored_placement_t placement;
	const char *font_face;
	unsigned int font_size;
	uint32_t font_color;
	bool font_bold;
	unsigned int last_width;
	unsigned int max_width_deviation;
};

struct rect_placement_t {
	struct anchored_placement_t placement;
	unsigned int width, height;
	unsigned int round;
	uint32_t color;
	bool fill;
};

struct coordinate_t {
	int x, y;
};

struct placement_t {
	struct coordinate_t top_left;
	struct coordinate_t bottom_right;
#if CAIRO_DEBUG
	struct coordinate_t anchor;
#endif
};

struct table_definition_t {
	unsigned int columns, rows;
	unsigned int *column_widths;
	unsigned int row_height;
	struct anchored_placement_t anchor;
	struct font_placement_t font_default;
	void (*rendering_callback)(char *dest_buf, unsigned int dest_buf_length, struct font_placement_t *placement, unsigned int x, unsigned int y, void *ctx);
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
struct cairo_swbuf_t *create_swbuf(unsigned int width, unsigned int height);
void swbuf_clear(struct cairo_swbuf_t *surface, uint32_t bgcolor);
uint32_t* swbuf_get_pixel_data(const struct cairo_swbuf_t *surface);
uint32_t swbuf_get_pixel(const struct cairo_swbuf_t *surface, unsigned int x, unsigned int y);
void swbuf_render_table(struct cairo_swbuf_t *surface, const struct table_definition_t *table, void *ctx);
unsigned int swbuf_text(struct cairo_swbuf_t *surface, const struct font_placement_t *placement, const char *fmt, ...);
void swbuf_rect(struct cairo_swbuf_t *surface, const struct rect_placement_t *placement);
void swbuf_circle(struct cairo_swbuf_t *surface, unsigned int x, unsigned int y, unsigned int radius, uint32_t color);
void swbuf_dump(struct cairo_swbuf_t *surface, const char *png_filename);
void free_swbuf(struct cairo_swbuf_t *buffer);
void cairo_addfont(const char *font_ttf_filename);
void cairo_cleanup(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

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


struct display_fb_ctx_t {
	int fd;
	uint8_t *screen;
};

extern const struct display_calltable_t display_fb_calltable;

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
/***************  AUTO GENERATED SECTION ENDS   ***************/

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void blit_swbuf_on_display(struct cairo_swbuf_t *swbuf, struct display_t *target);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
