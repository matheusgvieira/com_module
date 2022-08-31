#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "driver/twai.h"

#include "mqtt.h"

static const char *TAG = "PUB";

static EventGroupHandle_t s_mqtt_event_group;

extern QueueHandle_t module_queue;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT_EVENT_CONNECTED\n");
            xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT_EVENT_DISCONNECTED\n");
            xEventGroupClearBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            printf("MQTT_EVENT_SUBSCRIBED, msg_id=%d\n", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d\n", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT_EVENT_PUBLISHED, msg_id=%d\n", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            printf("MQTT_EVENT_DATA\n");
            break;
        case MQTT_EVENT_ERROR:
            printf("MQTT_EVENT_ERROR\n");
            break;
        default:
            printf("Other event id:%d\n", event->event_id);
            break;
    }
}

void mqtt_publish_module_task(void *pvParameters)
{
    printf("Start Publish Broker:%s\n", CONFIG_BROKER_URL);

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

    com_module module;

    while (1) {


        EventBits_t EventBits = xEventGroupGetBits(s_mqtt_event_group);

        if (xQueueReceive(module_queue, (void *)&( module ), ( TickType_t ) 0 ) == pdTRUE) {
            if (EventBits & MQTT_CONNECTED_BIT) {

                printf("update=[%d]\n", module.update);

                if (module.update) {
                    printf("topic=[%s]\n", module.topic_current);
                    printf("current=[%.2f]\n", module.current);

                    printf("topic=[%s]\n", module.topic_voltage);
                    printf("voltage=[%.2f]\n", module.voltage);

                    printf("topic=[%s]\n", module.topic_power);
                    printf("power=[%.2f]\n", module.power);

                    printf("topic=[%s]\n", module.topic_energy);
                    printf("energy=[%.2f]\n", module.energy);

                    char payload[50];

                    sprintf(payload, "%f", module.current);
                    esp_mqtt_client_publish(mqtt_client, module.topic_current, payload, strlen(payload), 0, 0);

                    sprintf(payload, "%f", module.voltage);
                    esp_mqtt_client_publish(mqtt_client, module.topic_voltage, payload, strlen(payload), 0, 0);

                    sprintf(payload, "%f", module.power);
                    esp_mqtt_client_publish(mqtt_client, module.topic_power, payload, strlen(payload), 0, 0);

                    sprintf(payload, "%f", module.energy);
                    esp_mqtt_client_publish(mqtt_client, module.topic_energy, payload, strlen(payload), 0, 0);

                    set_init_nvs(&module);
                    module.update = 0;
                }
            } else {
                ESP_LOGE(TAG, "mqtt broker not connect");
            }
        }


    }

    printf("Task Delete");
    esp_mqtt_client_stop(mqtt_client);
    vTaskDelete(NULL);
}