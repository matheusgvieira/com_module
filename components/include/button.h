//
// Created by Matheus Gois Vieira on 24/08/22.
//

#ifndef COM_MODULE_BUTTON_H
#define COM_MODULE_BUTTON_H

#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi.h"

// Structure of sensor
typedef struct {
    gpio_num_t pin;
    int state;
} buttons;

void init_button(buttons *button);
int8_t get_state(buttons *button);
void reset_credentials_wifi_button(void *pvParameters);

#endif //COM_MODULE_BUTTON_H
