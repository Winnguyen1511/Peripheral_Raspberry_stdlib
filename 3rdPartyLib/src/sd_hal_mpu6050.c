#include "sd_hal_mpu6050.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

/* Who I am register value */
#define MPU6050_I_AM				0x68

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75

/* Gyro sensitivities in degrees/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Acce sensitivities in g/s */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)

SD_MPU6050_Result SD_MPU6050_Init(I2C_t* instance,  SD_MPU6050* DataStruct, SD_MPU6050_Device DeviceNumber, SD_MPU6050_Accelerometer AccelerometerSensitivity, SD_MPU6050_Gyroscope GyroscopeSensitivity)
{
	uint8_t WHO_AM_I = (uint8_t)MPU6050_WHO_AM_I;
	uint8_t temp;

	uint8_t d[2];


	/* Format I2C address */
	DataStruct->Address = (MPU6050_I2C_ADDR | (uint8_t)DeviceNumber) >> 1;
//	DataStruct->Address = MPU6050_I_AM | (uint8_t)DeviceNumber;
	uint8_t address = DataStruct->Address;

	/* Check if device is connected */
	if(HAL_I2C_IsDeviceReady(instance, address,2)!=HAL_OK)
	{
		return SD_MPU6050_Result_Error;
	}
	/* Check who am I */
	//------------------
		/* Send address */
		if(I2C_Master_Transmit(instance, address, &WHO_AM_I, 1) != HAL_OK)
		{
			return SD_MPU6050_Result_Error;
		}

		/* Receive multiple byte */
		if(I2C_Master_Receive(instance, address, &temp, 1) != HAL_OK)
		{
			return SD_MPU6050_Result_Error;
		}

		/* Checking */
		while(temp != MPU6050_I_AM)
		{
				/* Return error */
				return SD_MPU6050_Result_DeviceInvalid;
		}
	//------------------

	/* Wakeup MPU6050 */
	//------------------
		/* Format array to send */
		d[0] = MPU6050_PWR_MGMT_1;
		d[1] = 0x00;

		/* Try to transmit via I2C */
		if(I2C_Master_Transmit(instance, (uint16_t)address , (uint8_t *)d, 2) != HAL_OK)
		{
					return SD_MPU6050_Result_Error;
		}
	//------------------

	/* Set sample rate to 1kHz */
	SD_MPU6050_SetDataRate(instance, DataStruct, SD_MPU6050_DataRate_1KHz);

	/* Config accelerometer */
	SD_MPU6050_SetAccelerometer(instance, DataStruct, AccelerometerSensitivity);

	/* Config Gyroscope */
	SD_MPU6050_SetGyroscope(instance, DataStruct, GyroscopeSensitivity);

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}

SD_MPU6050_Result SD_MPU6050_SetDataRate(I2C_t* instance,  SD_MPU6050* DataStruct, uint8_t rate)
{
	uint8_t d[2];

	uint8_t address = DataStruct->Address;
	/* Format array to send */
	d[0] = MPU6050_SMPLRT_DIV;
	d[1] = rate;

	/* Set data sample rate */
	I2C_Master_Transmit(instance, (uint16_t)address,(uint8_t *)d,2);
	/*{
				return SD_MPU6050_Result_Error;
	}*/

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}

SD_MPU6050_Result SD_MPU6050_SetAccelerometer(I2C_t* instance,  SD_MPU6050* DataStruct, SD_MPU6050_Accelerometer AccelerometerSensitivity)
{
	uint8_t temp;
	
	uint8_t address = DataStruct->Address;
	uint8_t regAdd =(uint8_t )MPU6050_ACCEL_CONFIG;

	/* Config accelerometer */
	I2C_Master_Transmit(instance, (uint16_t)address,&regAdd, 1);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	I2C_Master_Receive(instance, (uint16_t)address, &temp, 1);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	temp = (temp & 0xE7) | (uint8_t)AccelerometerSensitivity << 3;
	I2C_Master_Transmit(instance, (uint16_t)address,&temp, 1);
	/*{
				return SD_MPU6050_Result_Error;
	}*/

	/* Set sensitivities for multiplying gyro and accelerometer data */
	switch (AccelerometerSensitivity) {
		case SD_MPU6050_Accelerometer_2G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_2;
			break;
		case SD_MPU6050_Accelerometer_4G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_4;
			break;
		case SD_MPU6050_Accelerometer_8G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_8;
			break;
		case SD_MPU6050_Accelerometer_16G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_16;
			break;
		default:
			break;
		}

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}

SD_MPU6050_Result SD_MPU6050_SetGyroscope(I2C_t* instance,  SD_MPU6050* DataStruct, SD_MPU6050_Gyroscope GyroscopeSensitivity)
{
	uint8_t temp;
	
	uint8_t address = DataStruct->Address;
	uint8_t regAdd =(uint8_t )MPU6050_GYRO_CONFIG;

	/* Config gyroscope */
	I2C_Master_Transmit(instance, (uint16_t)address,&regAdd, 1);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	I2C_Master_Receive(instance, (uint16_t)address, &temp, 1);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	temp = (temp & 0xE7) | (uint8_t)GyroscopeSensitivity << 3;
	I2C_Master_Transmit(instance, (uint16_t)address,&temp, 1);
	/*{
				return SD_MPU6050_Result_Error;
	}*/

	switch (GyroscopeSensitivity) {
			case SD_MPU6050_Gyroscope_250s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_250;
				break;
			case SD_MPU6050_Gyroscope_500s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_500;
				break;
			case SD_MPU6050_Gyroscope_1000s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_1000;
				break;
			case SD_MPU6050_Gyroscope_2000s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_2000;
				break;
			default:
				break;
		}
	/* Return OK */
	return SD_MPU6050_Result_Ok;
}

SD_MPU6050_Result SD_MPU6050_ReadAccelerometer(I2C_t* instance,  SD_MPU6050* DataStruct)
{
	uint8_t data[6];
	uint8_t reg = MPU6050_ACCEL_XOUT_H;
	
	uint8_t address = DataStruct->Address;

	/* Read accelerometer data */
	I2C_Master_Transmit(instance, (uint16_t)address, &reg, 1);

	I2C_Master_Receive(instance, (uint16_t)address, data, 6);

	/* Format */
	DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}
SD_MPU6050_Result SD_MPU6050_ReadGyroscope(I2C_t* instance,  SD_MPU6050* DataStruct)
{
	uint8_t data[6];
	uint8_t reg = MPU6050_GYRO_XOUT_H;
	
	uint8_t address = DataStruct->Address;

	/* Read gyroscope data */
	I2C_Master_Transmit(instance, (uint16_t)address, &reg, 1);

	I2C_Master_Receive(instance, (uint16_t)address, data, 6);

	/* Format */
	DataStruct->Gyroscope_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Gyroscope_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Gyroscope_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}
SD_MPU6050_Result SD_MPU6050_ReadTemperature(I2C_t* instance,  SD_MPU6050* DataStruct)
{
	uint8_t data[2];
	int16_t temp;
	uint8_t reg = MPU6050_TEMP_OUT_H;
	
	uint8_t address = DataStruct->Address;

	/* Read temperature */
	I2C_Master_Transmit(instance, (uint16_t)address, &reg, 1);

	I2C_Master_Receive(instance, (uint16_t)address, data, 2);

	/* Format temperature */
	temp = (data[0] << 8 | data[1]);
	DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}
SD_MPU6050_Result SD_MPU6050_ReadAll(I2C_t* instance,  SD_MPU6050* DataStruct)
{
	uint8_t data[14];
	int16_t temp;
	uint8_t reg = MPU6050_ACCEL_XOUT_H;
	
	uint8_t address = DataStruct->Address;

	/* Read full raw data, 14bytes */
	I2C_Master_Transmit(instance, (uint16_t)address, &reg, 1);

	I2C_Master_Receive(instance, (uint16_t)address, data, 14);

	/* Format accelerometer data */
	DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

	/* Format temperature */
	temp = (data[6] << 8 | data[7]);
	DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);

	/* Format gyroscope data */
	DataStruct->Gyroscope_X = (int16_t)(data[8] << 8 | data[9]);
	DataStruct->Gyroscope_Y = (int16_t)(data[10] << 8 | data[11]);
	DataStruct->Gyroscope_Z = (int16_t)(data[12] << 8 | data[13]);

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}
SD_MPU6050_Result SD_MPU6050_EnableInterrupts(I2C_t* instance,  SD_MPU6050* DataStruct)
{
	uint8_t temp;
	uint8_t reg[2] = {MPU6050_INT_ENABLE,0x21};
	
	uint8_t address = DataStruct->Address;

	/* Enable interrupts for data ready and motion detect */
	I2C_Master_Transmit(instance, (uint16_t)address, reg, 2);

	uint8_t mpu_reg= MPU6050_INT_PIN_CFG;
	/* Clear IRQ flag on any read operation */
	I2C_Master_Transmit(instance, (uint16_t)address, &mpu_reg, 1);

	I2C_Master_Receive(instance, (uint16_t)address, &temp, 14);
	temp |= 0x10;
	reg[0] = MPU6050_INT_PIN_CFG;
	reg[1] = temp;
	I2C_Master_Transmit(instance, (uint16_t)address, reg, 2);

	/* Return OK */
	return SD_MPU6050_Result_Ok;
}
SD_MPU6050_Result SD_MPU6050_DisableInterrupts(I2C_t* instance,  SD_MPU6050* DataStruct)
{
	uint8_t reg[2] = {MPU6050_INT_ENABLE,0x00};
	
	uint8_t address = DataStruct->Address;

	/* Disable interrupts */
	I2C_Master_Transmit(instance, (uint16_t)address,reg,2!=HAL_OK);
	/* Return OK */
	return SD_MPU6050_Result_Ok;
}
SD_MPU6050_Result SD_MPU6050_ReadInterrupts(I2C_t* instance,  SD_MPU6050* DataStruct, SD_MPU6050_Interrupt* InterruptsStruct)
{
	uint8_t read;

	/* Reset structure */
	InterruptsStruct->Status = 0;
	uint8_t reg = MPU6050_INT_STATUS;
	
	uint8_t address = DataStruct->Address;

	I2C_Master_Transmit(instance, (uint16_t)address, &reg, 1);

	I2C_Master_Receive(instance, (uint16_t)address, &read, 14);

	/* Fill value */
	InterruptsStruct->Status = read;
	/* Return OK */
	return SD_MPU6050_Result_Ok;
}


