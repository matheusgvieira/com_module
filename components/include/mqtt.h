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
#include "generic_digital_output.h"
#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines
#define TAG_MQTT "MQTT"

#define CONFIG_BROKER_URL "mqtt://mqtt.tago.io:1883"

#define MQTT_DISCONNECTED   BIT1
#define MQTT_CONNECTED      BIT2
#define MQTT_PUBLISHED      BIT3

#define DEBUG_MODE      0

// Structs
typedef struct {
    char* topic;
    float payload;
    char* tag;
    int8_t payload_received;
    gpio_num_t pin_relay;
} mqtt_message_t;

// Functions
esp_mqtt_client_config_t get_mqtt_cfg(void);
void mqtt_subscriber_relay_1_task(void *pvParameters);
void mqtt_subscriber_relay_2_task(void *pvParameters);
void mqtt_publish_state_task(void *pvParameters);
void mqtt_publish_data_task(void *pvParameters);

#ifdef __cplusplus
}
#endif