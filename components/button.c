#include "button.h"

buttons button_reset_wifi = {.pin = 32, .state = 0};

void init_button(buttons *button) {
    gpio_set_direction(button->pin, GPIO_MODE_INPUT);
};

int8_t get_state(buttons *button){

    button->state = gpio_get_level(button->pin);

    return button->state;
};

void reset_credentials_wifi_button(void *pvParameters) {
    init_button(&button_reset_wifi);
    printf("\nBUTTON | initialization!!\n");

    while(1) {
        get_state(&button_reset_wifi);
        if (button_reset_wifi.state == 1) {
            reset_wifi_credentials();
            printf("Button state => %d\n", button_reset_wifi.state);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}