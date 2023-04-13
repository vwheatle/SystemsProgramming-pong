V Wheatley  
Systems Programming

# `pong_game`

I found out how to split my source across files this time. It shows in the folder structure of [this project's git repository](https://github.com/vwheatle/SystemsProgramming-pong/tree/original). This means there's a lot of files, and functionality is a bit scattered across each.

The code base revolves around several game objects (`ball_obj`, `wall_obj`) with game loop callbacks (`_setup`, `_update`, `_draw`). The basic structure of the game is as follows:

- `main.c` holds all the signal scheduling and curses setting parts of the program.
- `game.h` defines the game state struct that `main.c` holds.
- `game.c` holds the callbacks that `main.c` calls whenever a tick goes by.
- `geometry.h` defines some geometry structures I use (such as integer vector types) -- and a few utility functions that justify the existence of a `geometry.o` in the linking step...
- `geometry.c` implements those utility functions.
- `ball.h` defines the pong ball's structure and game loop callbacks.
- `ball.c` holds all the initial state of the pong ball and implements its game loop callbacks. it also holds all the code needed for the ball to bounce off a wall.
- `wall.h` defines the structure of a wall (and some paddle-related stuff, as I made the mistake of making paddles and walls the same thing)
- `wall.c` defines how to draw a rectangle (mostly unused except for its special case of 1-width / 1-height walls).
- `set_ticker.h` is a single-header thing containing the utility function to set a time interval for triggering SIGALRM signals.

The headers rely on the non-standard `#pragma once` preprocessor tag, but this shouldn't pose a problem and should work on most modern compilers.

Note that in my opinion, the comments are lacking here. I'll go through later and add more comments in a commit to the git repository.
## `main.c`

```c
```
