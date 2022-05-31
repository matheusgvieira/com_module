#include <stdio.h>
#include "led.h"
#include "uart.h"
#include "buzzer.h"

led_rgb led_2 = {.pin = 2, .tag = "red"};

void app_main(void)
{
    init_led(&led_2);
    set_state_led(&led_2, 1);

    while (1)
    {
        run_motor(hall_sensor_value);
    }
}