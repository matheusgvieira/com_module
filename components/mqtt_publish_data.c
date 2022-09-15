#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "events.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "mqtt.h"
#include "button.h"

static const char *TAG = "MQTT_PUBLISH_DATA";
extern QueueHandle_t module_queue;

buttons button_debug = {.state = 0, .pin = 19};

void mqtt_publish_data_task(void *pvParameters)
{

    const esp_mqtt_client_config_t mqtt_cfg = get_mqtt_cfg();
    esp_mqtt_client_handle_t mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler_publish, mqtt_client);
    esp_mqtt_client_start(mqtt_client);

    uint32_t mqtt_state = 0;
    com_module module;

    while (1) {


        if (xQueueReceive(module_queue, (void *)&( module ), ( TickType_t ) 0 ) == pdTRUE) {

            xTaskNotifyWait(0, 0, &mqtt_state, portMAX_DELAY);
            if (mqtt_state == MQTT_CONNECTED) {

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

                    get_state(&button_debug);
                    if (button_debug.state == 1) {
                        esp_mqtt_client_publish(mqtt_client, module.topic_debug, module.read_uart, strlen(module.read_uart), 0, 0);
                    }

                    set_init_nvs(&module);
                    module.update = 0;
                }
            } else {
                ESP_LOGE(TAG, "mqtt broker not connect");
            }
        }


    }

    printf("Task Delete\n");
    esp_mqtt_client_stop(mqtt_client);
    vTaskDelete(NULL);
}