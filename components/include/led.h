#pragma once

#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"


#ifdef __cplusplus
extern "C" {
#endif

// Structure of sensor
typedef struct {
	gpio_num_t pin;
    char* color;
} led_rgb;

void init_led(led_rgb *led);
void set_state_led(led_rgb *led, uint8_t state);

#ifdef __cplusplus
}
#endif