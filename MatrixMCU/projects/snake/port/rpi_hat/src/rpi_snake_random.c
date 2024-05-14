#include "snake_random.h"

#include <stdlib.h>
#include <time.h>

int
snake_random_init(snake_game_t* p_game)
{
  /* Get from original code. Return 0 if wrong and 1 if correct */
  srand (time(NULL));
  return 1;
}

int
snake_random_get(int limit)
{
  /* Get from original code */
  /* return random number between 0 (included) and limit (not included). */
  /* Use modulo (%) */
  return rand()%limit;
}

void
snake_random_close(snake_game_t* p_game)
{
  /* Get from original code, if something to close */
}
