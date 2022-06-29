#pragma once

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "mqtt_client.h"
#include "structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines
#define TAG_MQTT "MQTT"

// Variables

// Functions
void mqttPublish(float value, char* topic, data_input *data_in);
void mqttSubscribe(float value, char* topic);
void mqttInit(void);

#ifdef __cplusplus
}
#endif