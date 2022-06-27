#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "mqtt.h"

void app_main()
{
    mqttInit();

    while (true){
        mqttPublish(1.12, "tcc/current");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}