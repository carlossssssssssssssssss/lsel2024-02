#include "snake_input.h"
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "stm32_init.h"
#include "stm32_bsp.h"
#include "lsm303dlhc.h"
#include "accelero.h"
#include "stm32f4xx.h"


static ACCELERO_DrvTypeDef* acc = &Lsm303dlhcDrv;

int
snake_input_init(snake_game_t* p_game)
{
  if (!stm32_init()) {
    return 0;
  }
  /* TODO Clear flag */

  acc->Init(bsp_get_accelero_config());

  return 1;
}

void
snake_input_update_new_heading (snake_game_t* p_game)
{
    p_game->new_heading=p_game->snake.heading;
    /* Global variable button_heading indicates last heading value of button */
    /* Change: every USER button pressed, change button_heading COUNTERCLOCKWISE: */
    /* From LEFT to DOWN */

    int16_t data[3];
    acc->GetXYZ(data);
    //printf("Ejex:%d \n",data[0]);
    if(data[0]<-7000){
      p_game->new_heading = UP;
    }
    else if(data[0]>7000){
      p_game->new_heading = DOWN;
    }
    else if(data[1]>7000){
      p_game->new_heading = RIGHT;
    }
    else if(data[1]<-7000){
      p_game->new_heading = LEFT;
    }
    
    /* TODO Check flag, clear and update new_heading and stm32 heading */
    
}

void
snake_input_close(snake_game_t* p_game)
{
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}

