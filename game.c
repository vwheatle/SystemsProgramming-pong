#include <curses.h>

#include "geometry.h"
#include "game.h"

#define sizeofarr(arr) (sizeof(arr) / sizeof(*arr))

void game_setup(game_obj *game) {
	game->wall[0].rect =
		(rect2i) {{RIGHT_EDGE - PADDLE_OFFSET_X, PADDLE_START_Y}, PADDLE_SIZE};
	game->wall[1].rect =
		(rect2i) {{LEFT_EDGE + PADDLE_OFFSET_X, PADDLE_START_Y}, PADDLE_SIZE};

	game->wall[2].rect = (rect2i) {
		{LEFT_EDGE + PADDLE_OFFSET_X * 5, PADDLE_START_Y - 1}, {7, 7}};
	game->wall[3].rect = (rect2i) {
		{RIGHT_EDGE - PADDLE_OFFSET_X * 7, PADDLE_START_Y - 1}, {7, 7}};

	for (size_t i = 0; i < sizeofarr(game->wall); i++) {
		wall_setup(&game->wall[i]);
	}
	for (size_t i = 0; i < sizeofarr(game->ball); i++) {
		ball_setup(&game->ball[i]);

		game->ball[i].pos.y += (game->ball[i].pos.x + i) / 16;
		game->ball[i].pos.x += i % 16;

		if (i & 1) game->ball[i].dir.x = -1;
		if (i & 2) game->ball[i].dir.y = -1;

		game->ball[i].ticks_total.x += i >> 2;
		game->ball[i].ticks_total.y += i >> 1;

		game->ball[i].walls = game->wall;
		game->ball[i].walls_len = sizeofarr(game->wall);
	}
}

void game_input(game_obj *game, int key) {
	for (size_t i = 0; i < sizeofarr(game->ball); i++) {
		if (key == 'f')
			game->ball[i].ticks_total.x--;
		else if (key == 's')
			game->ball[i].ticks_total.x++;
		else if (key == 'F')
			game->ball[i].ticks_total.y--;
		else if (key == 'S')
			game->ball[i].ticks_total.y++;
	}
	if (key == 'j')
		game->wall[0].rect.pos.y++, game->wall[1].rect.pos.y++;
	else if (key == 'k')
		game->wall[0].rect.pos.y--, game->wall[1].rect.pos.y--;
	if (key == 'h')
		game->wall[0].rect.pos.x--, game->wall[1].rect.pos.x++;
	else if (key == 'l')
		game->wall[0].rect.pos.x++, game->wall[1].rect.pos.x--;
}

void game_update(game_obj *game) {
	for (size_t i = 0; i < sizeofarr(game->wall); i++)
		wall_update(&game->wall[i]);
	for (size_t i = 0; i < sizeofarr(game->ball); i++)
		ball_update(&game->ball[i]);
}

bool game_draw(game_obj *game) {
	bool drawn = false;
	for (size_t i = 0; i < sizeofarr(game->ball); i++)
		drawn |= ball_draw(&game->ball[i]);
	for (size_t i = 0; i < sizeofarr(game->wall); i++)
		drawn |= wall_draw(&game->wall[i]);
	return drawn;
}
