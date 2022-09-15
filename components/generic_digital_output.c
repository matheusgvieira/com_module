/*
    author: Matheus Gois
    subject: digital_output for flag states
    struct: digital_output led_any = {.pin = 2, .tag = "red", .time = 1000};
*/

#include <stdio.h>
#include "generic_digital_output.h"

static const char *TAG = "DIGITAL_OUTPUT";

void set_state_digital_output(digital_output *digital_output, uint8_t state)
{
    gpio_set_level(digital_output->pin, state);
}

void initialize_digital_output(digital_output *digital_output)
{
    ESP_LOGI(TAG, "Configured GPIO OUTPUT %d %s!", digital_output -> pin , digital_output -> tag );

    gpio_reset_pin(digital_output -> pin);
    gpio_set_direction(digital_output -> pin, GPIO_MODE_OUTPUT);
}

void toggle_digital_output(digital_output *digital_output, TickType_t time) {
    gpio_set_level(digital_output->pin, 1);
    vTaskDelay(time / portTICK_PERIOD_MS);
    gpio_set_level(digital_output->pin, 0);
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void toggle_digital_output_task(void *pvParameters) {
    digital_output digital_io = *(digital_output *) pvParameters;

    while(1) {
        gpio_set_level(digital_io.pin, 1);
        vTaskDelay(digital_io.time / portTICK_PERIOD_MS);
        gpio_set_level(digital_io.pin, 0);
        vTaskDelay(digital_io.time / portTICK_PERIOD_MS);
    }
}