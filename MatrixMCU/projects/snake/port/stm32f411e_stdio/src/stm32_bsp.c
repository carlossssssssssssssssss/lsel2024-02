#include "stm32f4xx.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32_hal_msp.h"

I2C_HandleTypeDef  i2c_handle;

int stm32_i2c_init(void){
    i2c_handle.Instance=I2C1;
    i2c_handle.Init.ClockSpeed=100000;
    i2c_handle.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;
    static int result = 0;
    if (!result) {
        if(HAL_I2C_Init(&i2c_handle) != HAL_OK) {
            result = 0;
        } else {
            result = 1;
        }
    }
  return result;
}

int stm32_i2c_deinit(void){
    i2c_handle.Instance=I2C1;
    i2c_handle.Init.ClockSpeed=100000;
    i2c_handle.Init.AddressingMode=I2C_ADDRESSINGMODE_7BIT;    
    static int result = 0;
    if (!result) {
        if(HAL_I2C_DeInit(&i2c_handle) != HAL_OK) {
            result = 0;
        } else {
            result = 1;
        }
    }
  return result;
}

int COMPASSACCELERO_IO_Init(void){
    return stm32_i2c_init();
}

void COMPASSACCELERO_IO_ITConfig(void){   
}

void COMPASSACCELERO_IO_Write(uint16_t DeviceAddr,uint8_t RegisterAddr, uint8_t Value){
    uint8_t datos[]={RegisterAddr,Value};
    HAL_I2C_Master_Transmit(&i2c_handle,DeviceAddr, datos,2, 100);
}

uint8_t COMPASSACCELERO_IO_Read(uint16_t DeviceAddr,uint8_t RegisterAddr){
    uint8_t datos[]={RegisterAddr,0};
    HAL_I2C_Master_Transmit(&i2c_handle,DeviceAddr, datos,2, 100);
    return datos[1];
}