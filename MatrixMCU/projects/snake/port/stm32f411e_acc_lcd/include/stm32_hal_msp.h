#ifndef _STM32_HAL_MSP_H
#define _STM32_HAL_MSP_H

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);

#endif //STM32_HAL_MSP_H
