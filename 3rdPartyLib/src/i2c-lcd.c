/** 
Edit by modify: Ngoc Hang 
Porting to linux: Winnguyen
**/

#include "i2c-lcd.h"
//extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly
#ifndef SLAVE_ADDRESS_LCD
#define SLAVE_ADDRESS_LCD 0x27 // change this according to ur setup
#endif

int lcd_send_cmd (I2C_t* instance, uint8_t cmd)
{
  	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	if(!I2C_Master_Transmit (instance, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4))
	{
		printf("Error: Cannot send lcd cmd %c\n", cmd);
		return ERROR;
	}
	return SUCCESS;
}

int lcd_send_data (I2C_t* instance, uint8_t data)
{
	uint8_t data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	if(!I2C_Master_Transmit (instance, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4))
	{
		printf("Error: Cannot send lcd data %s\n", data_t);
		return ERROR;
	}
	return SUCCESS;
}

int lcd_init (I2C_t* instance)
{
	if(!lcd_send_cmd (instance, 0x33))	return ERROR; /* set 4-bits interface */
	if(!lcd_send_cmd (instance,0x32)) return ERROR;
	usleep(50000);
	if(!lcd_send_cmd (instance, 0x28)) return ERROR; /* start to set LCD function */
	usleep(50000);
	if(!lcd_send_cmd (instance, 0x01)) return ERROR; /* clear display */
	usleep(50000);
	if(!lcd_send_cmd (instance, 0x06)) return ERROR; /* set entry mode */
	usleep(50000);
	if(!lcd_send_cmd (instance, 0x0c)) return ERROR; /* set display to on */	
	usleep(50000);
	if(!lcd_send_cmd (instance, 0x02)) return ERROR; /* move cursor to home and set data address to 0 */
	usleep(50000);
	if(!lcd_send_cmd (instance, 0x80)) return ERROR;
	usleep(50000);
	return SUCCESS;
}

int lcd_send_string (I2C_t* instance, uint8_t *str)
{
	while (*str) 
		if(!lcd_send_data (instance, *str++))
			return ERROR;
	return	SUCCESS;
}

int lcd_clear_display (I2C_t* instance)
{
	if(!lcd_send_cmd (instance, 0x01)) return ERROR; //clear display
	return ERROR;
}

int lcd_goto_XY (I2C_t* instance, int row, int col)
{
	uint8_t pos_Addr;
	if(col < 0 || col > MAX_COL)
	{
		printf("Error: col=0->15\n");
		return ERROR;
	}
	if(row == 0) 
	{
		pos_Addr = 0x80 + col;
	}
	else if(row == MAX_ROW)
	{
		pos_Addr = 0x80 | (0x40 + col);
	}
	else{
		printf("Error: row=0->1\n");
		return ERROR;
	}
	if(!lcd_send_cmd(instance, pos_Addr)) return ERROR;

	return SUCCESS;
}

int lcd_send_line(I2C_t* instance, uint8_t* str, int line)
{
	if(!lcd_goto_XY(instance, line, 0))
		return ERROR;
	if(!lcd_send_string(instance, (uint8_t*)"               "))
		return ERROR;
	int len = (int)strlen((char*)str);
	if(len > MAX_COL)
	{
		printf("Warning: string size out of bound\n");
	}

	if(!lcd_goto_XY(instance, line, 0))
		return ERROR;
	if(!lcd_send_string(instance, str))
	{
		printf("Error: Cannot send string %s to line %d\n", str, line);
		return ERROR;
	}
	return SUCCESS;
}
