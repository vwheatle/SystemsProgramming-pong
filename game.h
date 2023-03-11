#pragma once

#include <stdbool.h>

#include "geometry.h"

#include "wall.h"
#include "ball.h"

typedef struct {
	bool playing;
	int serves;

	ball_obj ball[1];
	wall_obj wall[4];
} game_obj;

void game_setup(game_obj *);
void game_input(game_obj *, int);
void game_update(game_obj *);
bool game_draw(game_obj *);
