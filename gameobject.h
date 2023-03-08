#pragma once

#include <stdbool.h>

typedef void game_obj_update_fn(void *);
typedef bool game_obj_draw_fn(void *);
typedef void game_obj_undraw_fn(void *);

typedef struct {
	game_obj_update_fn *update;
	game_obj_draw_fn *draw;
	game_obj_undraw_fn *undraw;
} game_obj_vtable;

typedef struct {
	game_obj_vtable vtable;
	void *self;
} game_obj;
