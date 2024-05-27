#include "stm32f4xx.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32_hal_msp.h"
#include "lsm303dlhc.h"
#include "accelero.h"

uint16_t bsp_get_accelero_config() {
    ACCELERO_InitTypeDef acc_init_config;
    uint16_t ctrl=0;
    acc_init_config.Power_Mode = LSM303DLHC_NORMAL_MODE;
    acc_init_config.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
    acc_init_config.Axes_Enable = LSM303DLHC_AXES_ENABLE;
    acc_init_config.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
    acc_init_config.BlockData_Update = LSM303DLHC_BlockUpdate_Continous;
    acc_init_config.Endianness = LSM303DLHC_BLE_LSB;
    acc_init_config.High_Resolution = LSM303DLHC_HR_ENABLE;
    ctrl |= (acc_init_config.Power_Mode | acc_init_config.AccOutput_DataRate | acc_init_config.Axes_Enable);
    ctrl |= ((acc_init_config.BlockData_Update | acc_init_config.Endianness | acc_init_config.AccFull_Scale | acc_init_config.High_Resolution) << 8);
    return ctrl;
}

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

void COMPASSACCELERO_IO_Init(void){
    stm32_i2c_init();
}

void COMPASSACCELERO_IO_ITConfig(void){   
}

void COMPASSACCELERO_IO_Write(uint16_t DeviceAddr,uint8_t RegisterAddr, uint8_t Value){
    uint8_t datos[]={RegisterAddr,Value};
    HAL_I2C_Master_Transmit(&i2c_handle,DeviceAddr, datos,2, 100);
}

uint8_t COMPASSACCELERO_IO_Read(uint16_t DeviceAddr,uint8_t RegisterAddr){
    
    uint8_t data;

    HAL_I2C_Master_Transmit(&i2c_handle,DeviceAddr, &RegisterAddr, 1, 100);
    HAL_I2C_Master_Receive(&i2c_handle,DeviceAddr, &data, 1, 100);
    return data;
}