//
// Created by Matheus Gois Vieira on 26/06/22.
//
#include "mqtt.h"


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    mqtt_event_handler_cb(event_data);
}

void mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    switch (event->event_id)
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
            xTaskNotify(taskHandle, MQTT_CONNECTED, eSetValueWithOverwrite);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            xTaskNotify(taskHandle, MQTT_PUBLISHED, eSetValueWithOverwrite);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
            break;
    }
}

void MQTTLogic(int sensorReading)
{
    uint32_t command = 0;
    esp_mqtt_client_config_t mqttConfig = {
            .uri = "mqtt://mqtt.eclipse.org:1883"};
    esp_mqtt_client_handle_t client = NULL;

    while (true)
    {
        xTaskNotifyWait(0, 0, &command, portMAX_DELAY);
        switch (command)
        {
            case WIFI_CONNEECTED:
                client = esp_mqtt_client_init(&mqttConfig);
                esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
                esp_mqtt_client_start(client);
                break;
            case MQTT_CONNECTED:
                esp_mqtt_client_subscribe(client, "/topic/my/subscription/1", 2);
                char data[50];
                sprintf(data, "%d", sensorReading);
                printf("sending data: %d", sensorReading);
                esp_mqtt_client_publish(client, "topic/my/publication/1", data, strlen(data), 2, false);
                break;
            case MQTT_PUBLISHED:
                esp_mqtt_client_stop(client);
                esp_mqtt_client_destroy(client);
                esp_wifi_stop();
                return;
            default:
                break;
        }
    }
}