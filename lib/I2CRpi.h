#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string.h>

#ifndef HAL_OK
#define HAL_OK       1 
#endif // HAL_OK
        
                      
#ifndef  HAL_ERROR
#define HAL_ERROR   -1
#endif

#ifndef SUCCESS
#define SUCCESS       1 
#endif // HAL_OK
        
                      
#ifndef  ERROR
#define ERROR   -1
#endif

#define I2C_PATH    "/dev/i2c-"
#ifndef  MAX_NAME
#define MAX_NAME   1024
#endif
typedef struct  I2C_struct{
    char*path;
    int file_description;
    int bus_number;
}I2C_t;

int I2C_Init(I2C_t* instance, int bus);

int I2C_Master_Transmit(I2C_t* instance, uint16_t DevAddress, uint8_t*pData, uint16_t Size);

int I2C_Master_Receive(I2C_t* instance, uint16_t DevAddress, uint8_t*pData, uint16_t Size);

int I2C_IsDeviceReady(I2C_t* instance, uint16_t DevAddress, uint32_t Trials);

#endif