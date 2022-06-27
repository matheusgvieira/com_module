#pragma once

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "wifi.h"
#include "mqtt_client.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines
#define TAG_MQTT "MQTT"

// Variables
const uint32_t WIFI_CONNEECTED = BIT1;
const uint32_t MQTT_CONNECTED = BIT2;
const uint32_t MQTT_PUBLISHED = BIT3;

// Functions
void mqtt_event_handler_cb(esp_mqtt_event_handle_t event);
void MQTTLogic(int sensorReading);

#ifdef __cplusplus
}
#endif