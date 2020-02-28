# Peripheral_Raspberry_stdlib
Author: Nguyen Huynh Dang Khoa - Victor Nguyen  
January-February 2020
Major: Embedded System - Computer Science
University: Danang University of Sciences and Technology

## About
Here are some peripheral library in C that you can use on Raspberry Pi 3B+.  
There are some features in this repo:  
- GPIO and GPIO Interrupt handler library. These based on basic file operation on  
  sys/class/gpio/ directory. The Interrupt handler is implemented on software interface  
  that use "inotify" file listener in C. Basically we have a thread that poll the value  
  file in gpio directory, if there is an modification in the file, interrupt handler will  
  handle. See the example of the project here: [GPIO_Interrupt](https://github.com/Winnguyen1511/GPIO_Interrupt)  
- I2C library, I port this library from I2C library of STM32 HAL, the usage is quite the same,  
  see this example here: [I2C](https://github.com/Winnguyen1511/I2C_ADC_MSP430)  
- SPI library, I also port from SPI HAL STM32.  
- 3rd Party Library including: MPU6050, LCD 16x2, RFID RC522:[MPU6050](https://github.com/Winnguyen1511/MPU6050_Linux_Rasp3), [LCD](https://github.com/Winnguyen1511/LCD_RPi), [RC522](https://github.com/Winnguyen1511/testRFID)  

## How to use ?
- Just deep into my example projects in my github repo, try to run on Raspberry Pi 3B+.  
- Mostly I use GPIO17, dev-i2c1, devspi0.0 to code in my projects, you can try to modify it
  in my library freely.  
- If you have trouble, maybe you haven't enable i2c and spi in your dt_param in /boot/config.txt  
  Try to enable all of them  
- If there are any problem, please feel free to contact me at my [mail](khoanguyen1507dn@gmail.com)  

## Hope you love this.

