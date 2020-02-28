#include "I2CRpi.h"


int I2C_Init(I2C_t* instance, int bus)
{
    instance->bus_number = bus;
    instance->path = (char*)malloc(MAX_NAME * sizeof(char));
    strcat(instance->path, I2C_PATH);
    char bus_char[3];
    sprintf(bus_char, "%d", bus);
    strcat(instance->path, bus_char);
    instance->file_description = open(instance->path, O_RDWR);
    if(!(instance->file_description))
    {
        printf("Error: Failed to open bus %s\n", instance->path);
        return ERROR;
    }
    return SUCCESS;
}


int I2C_Master_Transmit(I2C_t* instance, uint16_t DevAddress, uint8_t*pData, uint16_t Size)
{
    if(ioctl(instance->file_description, I2C_SLAVE, DevAddress) < 0)
    {
	    printf("ioctl: Failed to connect to device\n");
	    return ERROR;
    }
    if(write(instance->file_description, pData, Size)!=Size)
    {	
		perror("write");
        return ERROR;
	}
    return SUCCESS;
}

int I2C_Master_Receive(I2C_t* instance, uint16_t DevAddress, uint8_t*pData, uint16_t Size)
{
    if(ioctl(instance->file_description, I2C_SLAVE, DevAddress) < 0)
    {
	    printf("ioctl: Failed to connect to device\n");
	    return ERROR;
    }
    if(read(instance->file_description, pData, Size)!=Size)
	{	
		perror("read");
        return ERROR;
	}

    return SUCCESS;
}

int I2C_IsDeviceReady(I2C_t* instance, uint16_t DevAddress, uint32_t Trials)
{
    int i;
        
    for(i = 0; i < Trials; i++)
        if(ioctl(instance->file_description, I2C_SLAVE, DevAddress) < 0)
	    {
		    printf("ioctl: Failed to connect to the device\n");
		    return ERROR;
	    }
        else return SUCCESS;
    return ERROR;
}