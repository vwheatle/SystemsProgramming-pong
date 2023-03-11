#include <curses.h>

#include "geometry.h"
#include "game.h"
#include "wall.h"

// statically get length of statically-sized array
#define sizeofarr(arr) (sizeof(arr) / sizeof(*arr))

// set up the initial game state.
void game_setup(game_obj *game) {
	game->playing = true;

	// serves (lives)
	game->serves = 3;

	// paddle
	game->wall[0].rect = (rect2i) {{RIGHT_EDGE, PADDLE_START_Y}, PADDLE_SIZE};

	// walls
	game->wall[1].rect =
		(rect2i) {{LEFT_EDGE, TOP_ROW + 1}, {1, BOARD_HEIGHT - 2}};
	game->wall[2].rect = (rect2i) {{LEFT_EDGE, TOP_ROW}, {BOARD_WIDTH, 1}};
	game->wall[3].rect = (rect2i) {{LEFT_EDGE, BOT_ROW}, {BOARD_WIDTH, 1}};

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

		game->ball[i].paddles = &game->wall[0];
		game->ball[i].paddles_len = 1;

		game->ball[i].walls = &game->wall[1];
		game->ball[i].walls_len = sizeofarr(game->wall) - 1;
	}
}

void game_input(game_obj *game, int key) {
	if (key == 'Q') game->playing = false;

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

	vec2i *paddle_pos = &game->wall[0].rect.pos;
	if (key == 'j' && paddle_pos->y <= (BOT_ROW - 1 - PADDLE_HEIGHT))
		paddle_pos->y++;
	else if (key == 'k' && paddle_pos->y > (TOP_ROW + 1))
		paddle_pos->y--;
}

void game_update(game_obj *game) {
	for (size_t i = 0; i < sizeofarr(game->wall); i++)
		wall_update(&game->wall[i]);
	for (size_t i = 0; i < sizeofarr(game->ball); i++) {
		if (game->ball[i].lost) {
			ball_serve(&game->ball[0]);
			game->serves--;
		}
		ball_update(&game->ball[i]);
	}
	if (game->serves <= 0) game->playing = false;
}

// returns a bool indicating if the window was even drawn to at all,
// so i can selectively execute refresh() move()..
bool game_draw(game_obj *game) {
	bool drawn = false;
	for (size_t i = 0; i < sizeofarr(game->ball); i++)
		drawn |= ball_draw(&game->ball[i]);
	for (size_t i = 0; i < 1; i++) drawn |= paddle_draw(&game->wall[i]);
	for (size_t i = 1; i < sizeofarr(game->wall); i++)
		drawn |= wall_draw(&game->wall[i]);
	return drawn;
}
