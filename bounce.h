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

/** the ping pong ball **/

struct ppball {
	vec2i pos, dir;
	vec2i ttm;
	vec2i ttg;
	char symbol;
};
