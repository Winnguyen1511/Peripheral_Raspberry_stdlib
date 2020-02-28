#ifndef GPIO_IRQ_HANDLER_H
#define GPIO_IRQ_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "GPIORpi.h" 

void* IRQ_Handler_GPIO17(void* arg);

void* IRQ_Handler_GPIO18(void* arg);

void* IRQ_Handler_GPIO27(void* arg);

void* IRQ_Handler_GPIO22(void* arg);

void* IRQ_Handler_GPIO23(void* arg);

void* IRQ_Handler_GPIO24(void* arg);

void* IRQ_Handler_GPIO25(void* arg);

void* IRQ_Handler_GPIO05(void* arg);

void* IRQ_Handler_GPIO06(void* arg);

void* IRQ_Handler_GPIO12(void* arg);

void* IRQ_Handler_GPIO13(void* arg);

void* IRQ_Handler_GPIO19(void* arg);

void* IRQ_Handler_GPIO16(void* arg);

void* IRQ_Handler_GPIO26(void* arg);

void* IRQ_Handler_GPIO20(void* arg);

void* IRQ_Handler_GPIO21(void* arg);


int GPIO_EXTI_Init(GPIO_t* instance);

extern void GPIO_EXTI_Callback(int gpio_num) __attribute__((weak));

#endif