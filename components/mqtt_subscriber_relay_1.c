#include "generic_digital_output.h"
#include "freertos/event_groups.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "events.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "mqtt.h"

static QueueHandle_t xQueueSubscribe;

digital_output relay_1;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT_EVENT_CONNECTED\n");
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT_EVENT_DISCONNECTED\n");
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
            printf("MQTT_EVENT_DATA");

            mqtt_message_t message;

            message.topic = event->topic;
            message.payload_received = atoi(event->data);

            xQueueSend(xQueueSubscribe, (void *)&message, 0);

            break;
        case MQTT_EVENT_ERROR:
            printf("MQTT_EVENT_ERROR");
            break;
        default:
            printf("Other event id:%d", event->event_id);
            break;
    }
}

void mqtt_subscriber_relay_1_task(void *pvParameters)
{
    mqtt_message_t message = *(mqtt_message_t *) pvParameters;

    relay_1.pin = message.pin_relay;
    relay_1.tag = "RELAY_1";

    initialize_digital_output(&relay_1);

    const esp_mqtt_client_config_t mqtt_cfg = get_mqtt_cfg();
    esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
    esp_mqtt_client_start(mqtt_client);

    xQueueSubscribe = xQueueCreate( 10, sizeof(mqtt_message_t) );

    if( xQueueSubscribe != NULL )
    {
        esp_mqtt_client_subscribe(mqtt_client, message.topic, 0);

        while (1) {
            printf("Wait for %s\n", message.topic);

            if (xQueueReceive(xQueueSubscribe, (void *)&message, portMAX_DELAY) == pdTRUE) {
                printf("\nTOPIC=[%s]\n", message.topic);

                printf("\nDATA=[%d]\n", message.payload_received);

                set_state_digital_output(&relay_1, message.payload_received);

                send_message_uart(0, message.payload_received);
            };
        }
    }

    printf("Task Delete\n");
    vTaskDelete(NULL);
}
