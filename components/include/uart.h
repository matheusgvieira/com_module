#pragma once

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

#define UART UART_NUM_2
#define RX_BUF_SIZE 1024

// Variables

// Functions
void initializationUart();
void readByteUart(void *arg);
void writeByteUart(void *arg);
//void echo_task(void *arg);


#ifdef __cplusplus
}
#endif