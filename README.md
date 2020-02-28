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
  see this example here: [I2C]
