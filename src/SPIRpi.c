#include "SPIRpi.h"

int SPI_Init(SPI_t* instance, int bus, SPI_mode_t mode, uint32_t speed, uint8_t word_len, int cs_pin_num)
{

    instance->bus_number = bus;
    instance->path = (char*)malloc(MAX_NAME * sizeof(char));
    strcat(instance->path, SPI_PATH);
    char bus_char[3];
    sprintf(bus_char, "%d", bus);
    strcat(instance->path, bus_char);
    instance->file_description = open(instance->path, O_RDWR);
    if(! ( instance->file_description ))
    {
        printf("Error: Failed to open bus %s\n", instance->path);
        return ERROR;
    }
    instance->mode = mode;
    if(ioctl(instance->file_description, SPI_IOC_WR_MODE, &(instance->mode)) == -1)
    {
        printf("Error: Cannot set mode to %d\n", instance->mode);
        return ERROR;
    }
    instance->speed = speed;
    instance->bits_per_words = word_len;

    if(GPIO_Init_Custom(&(instance->cs_pin), cs_pin_num, OUTPUT, HIGH_ACTIVE, NONE, HIGH) < 0)
    {
        printf("Error: Cannot create CS_PIN %d\n", cs_pin_num);
        return ERROR;
    }

    return SUCCESS;
}

int SPI_Transmit(SPI_t* instance, uint8_t* pData, uint16_t Size)
{
    uint8_t tmp[Size];
    struct spi_ioc_transfer transfer;
    transfer.tx_buf = (unsigned long)pData;
    transfer.rx_buf = (unsigned long)tmp;
    transfer.len = Size;
    transfer.speed_hz = instance->speed;
    transfer.bits_per_word = instance->bits_per_words;
    transfer.delay_usecs = 0;

    if(ioctl(instance->file_description, SPI_IOC_MESSAGE(1), &transfer) < 0)
    {
        printf("Error: Failed to send data\n");
        return ERROR;
    }
    return SUCCESS;
}
int SPI_Receive(SPI_t* instance, uint8_t* pData, uint16_t Size)
{
    uint8_t tmp[Size];
    struct spi_ioc_transfer transfer;
    transfer.rx_buf = (unsigned long)pData;
    transfer.tx_buf = (unsigned long)tmp;
    transfer.len = Size;
    transfer.speed_hz = instance->speed;
    transfer.bits_per_word = instance->bits_per_words;
    transfer.delay_usecs = 0;

    if(ioctl(instance->file_description, SPI_IOC_MESSAGE(1), &transfer) < 0)
    {
        printf("Error: Failed to send data\n");
        return ERROR;
    }
    return SUCCESS;
}

int SPI_TransmitReceive(SPI_t* instance, uint8_t* pTxData, uint8_t* pRxData, uint16_t Size)
{
    struct spi_ioc_transfer transfer;
    transfer.tx_buf = pTxData;
    transfer.rx_buf = pRxData;
    transfer.len = Size;
    transfer.speed_hz = instance->speed;
    transfer.bits_per_word = instance->bits_per_words;
    transfer.delay_usecs = 0;

    if(ioctl(instance->file_description, SPI_IOC_MESSAGE(1), &transfer) < 0)
    {
        printf("Error: Failed to send data\n");
        return ERROR;
    }
    return SUCCESS;
}