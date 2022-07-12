#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "mqtt.h"
#include "structures.h"
#include "uart.h"
#include "led.h"
#include "wifi.h"

com_module module = { .read_uart = "",
                      .voltage = 0.0,
                      .current = 0.0,
                      .power = 0.0,
                      .energy = 0.0,
                      .topic_current = "tcc/current",
                      .topic_voltage = "tcc/voltage",
                      .topic_power = "tcc/power",
                      .topic_energy = "tcc/energy",
                      .update = 0,
                      .relay1 = 0,
                      .relay2 = 0};

led_rgb led_main = {.pin = 25, .color= "white"};

mqtt_message_t mqtt_message_state_module = {.topic = "tcc/state", .payload = 1.0, .tag = "current_state"};

mqtt_message_t mqtt_message_relay1 = {.topic = "tcc/rele1", .pin_relay = 33};

mqtt_message_t mqtt_message_relay2 = {.topic = "tcc/rele2", .pin_relay = 32};


void app_main()
{
    init_led(&led_main);

    set_state_led(&led_main, 1);

    get_init_nvs(&module);

    initialization_uart();

    int8_t is_connect_wifi = wifi_init();

    if (is_connect_wifi) {

        xTaskCreate(mqtt_subscriber_task,
                    "subscriber_rele1_state",
                    1024*4,
                    &mqtt_message_relay1,
                    2,
                    NULL
        );

        xTaskCreate(mqtt_subscriber_task,
                    "subscriber_rele2_state",
                    1024*4,
                    &mqtt_message_relay2,
                    2,
                    NULL
        );

        xTaskCreate(mqtt_publish_task,
                    "publish_current_state",
                    1024*4,
                    &mqtt_message_state_module,
                    2,
                    NULL
                    );

        xTaskCreate(read_byte_uart,
                    "read_byte_uart",
                    1024*4,
                    &module,
                    2,
                    NULL
        );

        xTaskCreate(mqtt_publish_module_task,
                    "publish_values_got",
                    1024*4,
                    &module,
                    2,
                    NULL
        );


    }
}