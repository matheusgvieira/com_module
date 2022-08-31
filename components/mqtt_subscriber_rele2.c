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
#include "led.h"

//static const char *TAG = "SUB";

static EventGroupHandle_t s_mqtt_event_group;

#define MQTT_CONNECTED_BIT BIT0

extern QueueHandle_t xQueue_mqtt_tx;
extern QueueHandle_t xQueue_mqtt_rx;

static QueueHandle_t xQueueSubscribe;

led_rgb relay2;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT_EVENT_CONNECTED\n");
            xEventGroupSetBits(s_mqtt_event_group, MQTT_CONNECTED_BIT);
            //esp_mqtt_client_subscribe(mqtt_client, CONFIG_SUB_TOPIC, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf( "MQTT_EVENT_DISCONNECTED\n");
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

            mqtt_message_t message;

            message.topic = event->topic;
            message.payload_received = atoi(event->data);

            xQueueSend(xQueueSubscribe, (void *)&message, 0);

            break;
        case MQTT_EVENT_ERROR:
            printf("MQTT_EVENT_ERROR\n");
            break;
        default:
            printf("Other event id:%d\n", event->event_id);
            break;
    }
}

void mqtt_subscriber_rele2_task(void *pvParameters)
{
    mqtt_message_t message = *(mqtt_message_t *) pvParameters;

    relay2.pin = message.pin_relay;
    relay2.color= "blue";

    init_led(&relay2);

    printf("Start Subscribe Broker:%s", CONFIG_BROKER_URL);

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

    xQueueSubscribe = xQueueCreate( 10, sizeof(mqtt_message_t) );

    if( xQueueSubscribe != NULL )
    {
        // Subscribe topic
        esp_mqtt_client_subscribe(mqtt_client, message.topic, 0);

        while (1) {
            printf("Wait for %s\n", message.topic);

            if (xQueueReceive(xQueueSubscribe, (void *)&message, portMAX_DELAY) == pdTRUE) {
                printf("TOPIC=[%s]\n", message.topic);

                printf("DATA=[%d]\n", message.payload_received);

                set_state_led(&relay2, message.payload_received);

                writeByteUart(1, message.payload_received);
            };
        }
    }

    // Never reach here
    printf("Task Delete");
    vTaskDelete(NULL);
}
