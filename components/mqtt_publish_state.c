#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "mqtt_client.h"

#include "mqtt.h"

static const char *TAG = "PUB_STATE";

static EventGroupHandle_t s_mqtt_event_group;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT_EVENT_CONNECTED");
            xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT_EVENT_DISCONNECTED");
            xEventGroupClearBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            printf("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            printf("MQTT_EVENT_DATA");
            break;
        case MQTT_EVENT_ERROR:
            printf("MQTT_EVENT_ERROR");
            break;
        default:
            printf("Other event id:%d", event->event_id);
            break;
    }
}

void mqtt_publish_state_task(void *pvParameters)
{
    printf("Start Publish Broker:%s", CONFIG_BROKER_URL);

    const esp_mqtt_client_config_t mqtt_cfg = {
            .broker.address.uri = CONFIG_BROKER_URL,
            .credentials.username = "Token #2",
            .credentials.authentication.password = "a6fbba72-067c-4309-be80-ac15ec1c1aa7",
    };

    s_mqtt_event_group = xEventGroupCreate();
    esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
    xEventGroupClearBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
    esp_mqtt_client_start(mqtt_client);
    xEventGroupWaitBits(s_mqtt_event_group, MQTT_CONNECTED_BIT, false, true, portMAX_DELAY);
    printf("Connect to MQTT Server");

    mqtt_message_t message = *(mqtt_message_t *) pvParameters;

    while (1) {
        printf("topic=[%s]\n", message.topic);
        printf("data=[%.2f]\n", message.payload);

        EventBits_t EventBits = xEventGroupGetBits(s_mqtt_event_group);

        printf("EventBits=%lx", EventBits);

        if (EventBits & MQTT_CONNECTED_BIT) {
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

    printf("Task Delete");
    esp_mqtt_client_stop(mqtt_client);
    vTaskDelete(NULL);
}