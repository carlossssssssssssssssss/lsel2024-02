#ifndef _STM32_BSP_H
#define _STM32_BSP_H

uint16_t bsp_get_accelero_config(void);
int stm32_i2c_init(void);
int stm32_i2c_deinit(void);
void COMPASSACCELERO_IO_Init(void);
void COMPASSACCELERO_IO_ITConfig(void);
void COMPASSACCELERO_IO_Write(uint16_t DeviceAddr,uint8_t RegisterAddr, uint8_t Value);
uint8_t COMPASSACCELERO_IO_Read(uint16_t DeviceAddr,uint8_t RegisterAddr);

#endif //_STM32_BSP_H