#pragma once

#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#ifdef __cplusplus
extern "C" {
#endif

// Structure of sensor
typedef struct {
	gpio_num_t pin;
    char* tag;
    int time;
} digital_output;

void initialize_digital_output(digital_output *digital_output);
void set_state_digital_output(digital_output *digital_output, uint8_t state);
void toggle_digital_output(digital_output *digital_output, TickType_t time);
void toggle_digital_output_task(void *pvParameters);

#ifdef __cplusplus
}
#endif