#include "snake_input.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static int kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

int
snake_input_init(snake_game_t* p_game)
{
  return 1;
}

void
snake_input_update_new_heading (snake_game_t* p_game)
{
  p_game->new_heading = p_game->snake.heading;
  //TODO
  //kbhit() returns the amount of keys pressed
  //getchar() reads one of the keys
  //Read until no key pressed is left
  //UP: 'w'
  //DOWN: 's'
  //LEFT: 'a'
  //RIGHT: 'd'
    
}

void
snake_input_close(snake_game_t* p_game)
{
}




