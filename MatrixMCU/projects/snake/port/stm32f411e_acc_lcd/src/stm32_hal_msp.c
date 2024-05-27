#include "stm32f4xx.h"

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c){
//Para I2C1 en PB6 y PB9
//Reloj de bus (I2C1: APB1) 
__HAL_RCC_I2C1_CLK_ENABLE();
//Reloj de bus (GPIOB: AHB1) 
__HAL_RCC_GPIOB_CLK_ENABLE();

__HAL_RCC_SYSCFG_CLK_ENABLE();
__HAL_RCC_PWR_CLK_ENABLE();
//Inicialización de GPIO
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_9;
GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
GPIO_InitStruct.Pull = GPIO_PULLUP;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c){
//Para I2C1 en PB6 y PB9
//Reloj de bus (I2C1: APB1) 
__HAL_RCC_I2C1_CLK_DISABLE();
//Reloj de bus (GPIOB: AHB1) 
__HAL_RCC_GPIOB_CLK_DISABLE();

__HAL_RCC_SYSCFG_CLK_DISABLE();
__HAL_RCC_PWR_CLK_DISABLE();
//Inicialización de GPIO


HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_9);
}