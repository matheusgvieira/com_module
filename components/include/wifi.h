#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines
#define SSID "Fixtell_Wi-fi_FamiliaDeDeus"
#define PASSWORD "bily6523"

#define EXAMPLE_ESP_WIFI_SSID      "FIXTELL_TERRIO"
#define EXAMPLE_ESP_WIFI_PASS      "bily6523"
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
// Variables


extern TaskHandle_t taskHandle;
extern const uint32_t WIFI_CONNEECTED;

// Functions
void wifiInit();


#ifdef __cplusplus
}
#endif