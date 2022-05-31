#pragma once

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/uart.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines
#define TXD_PIN 1
#define RXD_PIN 3
#define RX_BUF_SIZE 1024

// Variables
char byte_message[RX_BUF_SIZE];

// Functions
void initUart();
char* readByte();


#ifdef __cplusplus
}
#endif