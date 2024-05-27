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
  printf("+");

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
  for(int x=0;x<p_game->limits.x;x++){
    printf("%c",fb->pixel[y][x]);
  }
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
  struct segment_t *seg_i;

  /* Set Blank */
  memset(fb, CHAR_EMPTY, sizeof(struct fb_t));

  /* TODO Set Apple in its position */
  fb->pixel[p_game->apple.y][p_game->apple.x] = CHAR_APPLE;

  /* TODO Set snake body in their positions, starting with tail */
	for(seg_i = p_game->snake.tail; seg_i->next; seg_i=seg_i->next) {
    fb->pixel[seg_i->y][seg_i->x] = CHAR_BODY;
  }
  /* TODO Set snake head in its position */
  fb->pixel[seg_i->y][seg_i->x]=CHAR_HEAD;
  
  clear_screen();
  print_hlimit(p_game); 

  /* TODO Print every row with print_row */
  for(int y=0;y<p_game->limits.y;y++){
    print_row(p_game,y);
  }

  print_hlimit(p_game); 
}

int
snake_display_init(snake_game_t* p_game)
{
  /* TODO Set limits in p_game */
  p_game->limits.x = MAX_X;
  p_game->limits.y = MAX_Y;
  memset(fb, CHAR_EMPTY, sizeof(struct fb_t));
  return stm32_init();
}

void
snake_display_close(snake_game_t* p_game)
{
  memset(fb, CHAR_EMPTY, sizeof(struct fb_t));
  clear_screen();
}
