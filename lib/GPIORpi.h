#ifndef GPIO_RPI_H
#define GPIO_RPI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/inotify.h>

//#include "GPIO_IRQ_Handler.h"

#define GPIO_NUM    26
#define PIN_NUM     40
#define INVALID_IRQ -1
#define GPIO_MAP_INDEX(X)               (X-2)
#define IRQ_Handler_Array_Index(X)      (X-1)  

#define FILE_FLAGS      O_RDWR
#define FILE_PERMS      S_IRUSR | S_IWUSR
#define MAX_PATH_SIZE   1024
#define MAX_NAME_SIZE   6
#define MAX_PIN_SIZE    2
#define GPIO_PATH   "/sys/class/gpio/"
#define EXPORT      "export"
#define UNEXPORT    "unexport"
#define GPIO_PREFIX "gpio"
#define DIRECTION   "/direction"
#define ACTIVE_LOW  "/active_low"
#define VALUE       "/value"
#define EDGE        "/edge"

#define SUCCESS     1
#define ERROR       -1
#define TRUE        1
#define FALSE       0

#define HIGH_HALF   0xFFFF0000
#define LOW_HALF    0x0000FFFF

typedef enum export_enum{UNEXPORTED =0, EXPORTED}export_t;
typedef enum direction_enum{OUTPUT=0, INPUT}direction_t;
typedef enum active_low_enum{HIGH_ACTIVE=0, LOW_ACTIVE}active_low_t;
typedef enum gpio_value_num{LOW=0, HIGH}gpio_value_t;
typedef enum edge_enum{NONE=0, RISING, FALLING, BOTH}edge_t;
typedef enum gpio_command_enum
{
    EXPORT_CMD,
    UNEXPORT_CMD,
    SET_DIR_CMD, 
    SET_ACTIVE_LOW_CMD, 
    SET_VALUE_CMD,
    SET_EDGE_CMD,
    GET_DIR_CMD,
    GET_ACTIVE_LOW_CMD,
    GET_VALUE_CMD,
    GET_EDGE_CMD    
}gpio_command_t;

typedef struct GPIO_struct{
    char* path;
    char* name;
    int gpio_num;
    export_t export_status;
    direction_t direction;
    active_low_t active_low;
    gpio_value_t value;
    edge_t edge;
}GPIO_t;

int GPIO_Init_Default(GPIO_t* instance, int number);
int GPIO_Init_Custom(GPIO_t* instance,\
            int number, direction_t dir,\
            active_low_t act, edge_t ed, gpio_value_t initVal);

int GPIO_Denit(GPIO_t* instance);

int GPIO_export(GPIO_t* instance);
int GPIO_unexport(GPIO_t* instance);

int GPIO_set_active_low(GPIO_t* instance, active_low_t act);
int GPIO_set_value(GPIO_t* instance, gpio_value_t val);
int GPIO_set_direction(GPIO_t* instance, direction_t dir);
int GPIO_set_edge(GPIO_t* instance, edge_t ed);

int GPIO_get_value(GPIO_t* instance, gpio_value_t* retVal);
int GPIO_get_export_status(GPIO_t* instance, export_t* retVal);
int GPIO_get_direction(GPIO_t* instance, direction_t* retVal);
int GPIO_get_active_low(GPIO_t* instance, active_low_t* retVal);
int GPIO_get_edge(GPIO_t* instance, edge_t* retVal);

int ioctl_cmd(GPIO_t* instance, gpio_command_t cmd, void* val);

int ioctl_is_exported(int num);
int ioctl_cmd_export(int num);
int ioctl_cmd_unexport(int num);
int ioctl_cmd_set_active_low(int num, active_low_t val);
int ioctl_cmd_set_dir(int num, direction_t dir);
int ioctl_cmd_set_value(int num, gpio_value_t val);
int ioctl_cmd_set_edge(int num, edge_t ed);

int ioctl_cmd_get_value(int num, gpio_value_t* val);
int ioctl_cmd_get_dir(int num, direction_t* val);
int ioctl_cmd_get_active_low(int num, active_low_t* val);
int ioctl_cmd_get_edge(int num, edge_t* val);

#endif