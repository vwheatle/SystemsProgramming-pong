/* bounce.h */

/* some settings for the game */

#define BLANK      ' '
#define DFL_SYMBOL 'o'

#define TOP_ROW    5
#define BOT_ROW    20
#define LEFT_EDGE  10
#define RIGHT_EDGE 70

#define X_INIT 10 // starting column
#define Y_INIT 10 // starting row

#define TICKS_PER_SEC 50 // affects speed

#define X_TTM 5
#define Y_TTM 8

typedef struct {
	int x, y;
} vec2i;

struct ball_obj {
	vec2i pos, dir;
	vec2i old_pos;

	vec2i ticks_total;
	vec2i ticks_left;

	char symbol;
	bool redraw;
};

void ball_setup(struct ball_obj *);
void ball_update(struct ball_obj *);
void ball_draw(struct ball_obj *);

bool bounce_or_lose(struct ball_obj *);

void ball_setup(struct ball_obj *ball) {
	ball->pos.x = X_INIT;
	ball->pos.y = Y_INIT;

	ball->old_pos = ball->pos;

	ball->dir.x = 1;
	ball->dir.y = 1;

	ball->ticks_left.x = ball->ticks_total.x = X_TTM;
	ball->ticks_left.y = ball->ticks_total.y = Y_TTM;

	ball->symbol = DFL_SYMBOL;
}

void ball_update(struct ball_obj *ball) {
	// old position
	ball->old_pos = ball->pos;

	if (ball->ticks_total.y > 0 && ball->ticks_left.y-- == 1) {
		ball->pos.y += ball->dir.y;               // move y
		ball->ticks_left.y = ball->ticks_total.y; // reset timer
		ball->redraw = true;
	}

	if (ball->ticks_total.x > 0 && ball->ticks_left.x-- == 1) {
		ball->pos.x += ball->dir.x;               // move x
		ball->ticks_left.x = ball->ticks_total.x; // reset timer
		ball->redraw = true;
	}
}

void ball_draw(struct ball_obj *ball) {
	if (ball->redraw) {
		// erase old character
		mvaddch(ball->old_pos.y, ball->old_pos.x, BLANK);
		// draw new character
		mvaddch(ball->pos.y, ball->pos.x, ball->symbol);

		bounce_or_lose(ball);

		move(LINES - 1, COLS - 1);
		refresh();

		ball->redraw = false;
	}
}

bool bounce_or_lose(struct ball_obj *ball) {
	bool r = false;

	if (ball->pos.y == TOP_ROW) {
		ball->dir.y = 1;
		r = true;
	} else if (ball->pos.y == BOT_ROW) {
		ball->dir.y = -1;
		r = true;
	}
	if (ball->pos.x == LEFT_EDGE) {
		ball->dir.x = 1;
		r = true;
	} else if (ball->pos.x == RIGHT_EDGE) {
		ball->dir.x = -1;
		r = true;
	}

	return r;
}
