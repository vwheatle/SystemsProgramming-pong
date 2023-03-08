#pragma once

#define TOP_ROW    1
#define BOT_ROW    (LINES - 2)
#define LEFT_EDGE  1
#define RIGHT_EDGE (COLS - 2)

#define TICKS_PER_SEC 50 // affects speed

typedef struct {
	int x, y;
} vec2i;
