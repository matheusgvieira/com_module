//
// Created by Matheus Gois Vieira on 26/06/22.
//
#include "mqtt.h"

esp_mqtt_client_handle_t client;

int8_t rele1 = 0;
int8_t rele2 = 0;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    int msg_id;
    // your_context_t *context = event->context;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            printf("MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            printf("sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            printf("sent subscribe successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            printf("sent unsubscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            printf("MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            printf("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
//            printf("sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            printf("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            printf("MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            if (strcmp(event->topic, "tcc/rele1") == 0) {
                rele1 = atoi(event -> data);
            }
            if (strcmp(event->topic, "tcc/rele2") == 0) {
                rele2 = atoi(event -> data);
            }

            break;
        case MQTT_EVENT_ERROR:
            printf("MQTT_EVENT_ERROR");
            break;
        default:
            break;
    }
}

static void mqtt_app_start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
            .broker.address.uri = CONFIG_BROKER_URL,
            .credentials.username = "Token #2",
            .credentials.authentication.password = "a6fbba72-067c-4309-be80-ac15ec1c1aa7",
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_publish(float value, char* topic, com_module *data_in) {
    char data[50];
    sprintf(data, "%f", value);
    printf("Sending data to broker: %.2f \n", value);
    esp_mqtt_client_publish(client, topic, data, strlen(data), 0, false);
    data_in -> update = 0;
}

void mqtt_subscribe(char* topic, com_module *data_out) {
    esp_mqtt_client_subscribe(client, topic, 2);
    if (rele1 != 0) {
        data_out -> relay1 = rele1;
        rele1 = 0;
    }
    if (rele2 != 0) {
        data_out -> relay2 = rele2;
        rele2 = 0;
    }
}

void mqtt_init(void) {
    printf("[APP] Startup..");
    printf("[APP] Free memory: %ld bytes", esp_get_free_heap_size());
    printf("[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    nvs_flash_init();
    mqtt_app_start();
}