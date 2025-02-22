#ifndef __I2C_CLIENT_H
#define __I2C_CLIENT_H

#include "main.h"

enum State { stop, waiting, getRegisterAddress, getData, sendData };

typedef struct i2c_client_init_t i2c_client_init_t;
typedef void (*_create_error_handler)();

struct i2c_client_init_t{
  I2C_HandleTypeDef *hi2c;
  void (*error_handler)();
};


struct i2c_client_data_t{
  enum State transferState;
  uint8_t registerAddress;
  uint8_t receiveBuffer;
  uint8_t i2c_memory[0xFF];
};

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t direction, uint16_t addrMatchCode);
void I2C_Client_Init(i2c_client_init_t *i2c_init);
void I2C_Error_Handler(void);
void I2C_Set_Register(uint8_t reg, uint8_t data);
uint8_t I2C_Get_Register(uint8_t reg);

#endif