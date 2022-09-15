#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "events.h"
#include <stdio.h>
#include "mqtt.h"

static const char *TAG = "MQTT_PUBLISH_STATE";

void mqtt_publish_state_task(void *pvParameters)
{
    const esp_mqtt_client_config_t mqtt_cfg = get_mqtt_cfg();
    esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler_publish, mqtt_client);

    mqtt_message_t message = *(mqtt_message_t *) pvParameters;
    uint32_t mqtt_state = 0;

    while (1) {
        printf("topic=[%s]\n", message.topic);
        printf("data=[%.2f]\n", message.payload);

        xTaskNotifyWait(0, 0, &mqtt_state, portMAX_DELAY);

        if (mqtt_state == MQTT_CONNECTED) {
            char payload[50];
            sprintf(payload, "%f", message.payload);
            esp_mqtt_client_publish(mqtt_client, message.topic, payload, strlen(payload), 0, 0);
        } else {
            ESP_LOGE(TAG, "mqtt broker not connect");
        }

        if(strcmp(message.tag, "current_state") == 0) {
            vTaskDelay( 10000 / portTICK_PERIOD_MS);
        }
    }

    printf("Task Delete\n");
    esp_mqtt_client_stop(mqtt_client);
    vTaskDelete(NULL);
}