#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "mqtt.h"
#include "structures.h"
#include "uart.h"
#include "led.h"

data_input module = {.read_uart = "", .voltage = 0.0, .current = 0.0, .update = 0};

led_rgb led_main = {.pin = 25, .color= "white"};

void app_main()
{
    init_led(&led_main);
    set_state_led(&led_main, 1);

    get_init_nvs(&module);

    mqttInit();
    initializationUart();

//    xTaskCreate(&readByteUart, "readByteUart", 2048, &module, 5, NULL);

    while(1){
        mqttPublish(1.0, "tcc/state", &module);

        module.read_uart = readByteUart();

        if (strcmp(module.read_uart, "") != 0) {
            setTypeData(&module);
        }

        if(module.update) {
            mqttPublish(module.current, "tcc/current", &module);
            mqttPublish(module.voltage, "tcc/voltage", &module);
            set_init_nvs(&module);
        }
    }
}