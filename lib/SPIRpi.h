#ifndef SPI_RPI_H
#define SPI_RPI_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include "GPIORpi.h"
#define SPI_PATH "/dev/spidev0."
#ifndef SUCCESS
#define SUCCESS     1
#endif

#ifndef ERROR
#define ERROR       -1
#endif

#ifndef MAX_NAME
#define MAX_NAME    1024
#endif

#define SPI_FREQ_100K   100000
#define SPI_FREQ_500K   500000
#define SPI_FREQ_1MHZ   1000000
#define SPI_FREQ_4MHZ   4000000
#define SPI_FREQ_8MHZ   8000000

#define SPI_WORD_LEN_8      8
#define SPI_WORD_LEN_16     16
#define SPI_WORD_LEN_32     32
typedef enum SPI_mode_enum{
    POL0_PHA0=0,
    POL0_PHA1,
    POL1_PHA0,
    POL1_PHA1
}SPI_mode_t;

typedef struct SPI_struct{
    char* path;
    int file_description;
    int bus_number;
    SPI_mode_t mode;
    uint32_t speed;
    uint8_t  bits_per_words;
    GPIO_t cs_pin;
}SPI_t;

int SPI_Init(SPI_t* instance, int bus, SPI_mode_t mode, uint32_t speed, uint8_t word_len, int cs_pin_num);

int SPI_Transmit(SPI_t* instance, uint8_t* pData, uint16_t Size);
int SPI_Receive(SPI_t* instance, uint8_t* pData, uint16_t Size);

int SPI_TransmitReceive(SPI_t* instance, uint8_t* pTxData, uint8_t* pRxData, uint16_t Size);



#endif