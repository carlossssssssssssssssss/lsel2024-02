#include "snake_display.h"

#define _GNU_SOURCE

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32_init.h"

#define MAX_X 16
#define MAX_Y 16

#define CHAR_APPLE '*'
#define CHAR_HEAD '@'
#define CHAR_BODY 'o'
#define CHAR_EMPTY ' '

struct fb_t {
  char pixel[MAX_Y][MAX_X];
};
static struct fb_t fb_s;
static struct fb_t* fb = &fb_s;

static void
clear_screen()
{
  printf("\e[1;1H\e[2J");
//  fsync(1);
}

static void
print_hlimit(snake_game_t* p_game)
{
  _write("+");

  /* TODO print row of limit with - */
  for(int x=0;x<p_game->limits.x;x++){
    printf("-");
  }
  printf("+\r\n");
}


static void
print_row(snake_game_t* p_game, int y)
{
  printf("|");

  /* TODO print row of game from fb struct */

  printf("|\r\n");
}



void 
snake_display_render(snake_game_t* p_game)
{
  /* Change: Each element is a char */
  /* - Apple is CHAR_APPLE */
  /* - Snake head is CHAR_HEAD */
  /* - Snake body CHAR_BODY */
  /* - Empty pixels are CHAR_EMPTY */
  //struct segment_t *seg_i;

  /* Set Blank */
  memset(fb, CHAR_EMPTY, sizeof(struct fb_t));

  /* TODO Set Apple in its position */

  /* TODO Set snake body in their positions, starting with tail */

  /* TODO Set snake head in its position */

  
  clear_screen();
  print_hlimit(p_game); 

  /* TODO Print every row with print_row */

  print_hlimit(p_game); 
}

int
snake_display_init(snake_game_t* p_game)
{
  /* TODO Set limits in p_game */


  return stm32_init();
}

void
snake_display_close(snake_game_t* p_game)
{
}
