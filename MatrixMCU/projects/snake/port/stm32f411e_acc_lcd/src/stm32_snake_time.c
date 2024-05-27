#include "snake_time.h"
#include "stm32_init.h"

#include "stm32f4xx.h"

int 
snake_time_init (snake_game_t* p_game) 
{
  return stm32_init();
}

void 
snake_time_close() 
{
}

void 
snake_time_delay_ms (uint32_t ms) 
{
  HAL_Delay(ms);
}
