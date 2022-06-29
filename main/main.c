#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "mqtt.h"
#include "structures.h"
#include "uart.h"

data_input module = {.read_uart = "", .voltage = 0.0, .current = 0.0, .update = 0};

void app_main()
{
    mqttInit();
    initializationUart();

//    xTaskCreate(&readByteUart, "readByteUart", 2048, &module, 5, NULL);

    while(1){
        module.read_uart = readByteUart();
        setTypeData(&module);

        if(module.update) {
            mqttPublish(module.current, "tcc/current", &module);
            mqttPublish(module.voltage, "tcc/voltage", &module);
        }
    }
}