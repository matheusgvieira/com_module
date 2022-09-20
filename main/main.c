#include "generic_digital_output.h"
#include "freertos/task.h"
#include "structures.h"
#include "esp_log.h"
#include "button.h"
#include "mqtt.h"
#include "uart.h"
#include "wifi.h"

const char* TAG = "STRBIOT";

TaskHandle_t mqtt_task_handle;

com_module module = { .read_uart = "",
                      .voltage = 0.0,
                      .current = 0.0,
                      .power = 0.0,
                      .energy = 0.0,
                      .topic_current = "tcc/current",
                      .topic_voltage = "tcc/voltage",
                      .topic_power = "tcc/power",
                      .topic_energy = "tcc/energy",
                      .topic_debug = "/debug",
                      .update = 0,
                      .relay1 = 0,
                      .relay2 = 0};

digital_output led_main = {.pin = 25, .tag= "white"};

mqtt_message_t mqtt_message_state_module = {.topic = "tcc/state", .payload = 1.0, .tag = "current_state"};

mqtt_message_t mqtt_message_relay1 = {.topic = "tcc/relay_1", .pin_relay = 33};

mqtt_message_t mqtt_message_relay2 = {.topic = "tcc/relay_2", .pin_relay = 32};

static const int module_queue_len = 5;

QueueHandle_t module_queue;

SemaphoreHandle_t xSemaphore = NULL;

wifi_credentials credentials = {.ssid = "", .password = ""};

void app_main()
{
    initialize_digital_output(&led_main);

    set_state_digital_output(&led_main, 1);

    get_init_nvs(&module);

    initialization_uart();

    module_queue = xQueueCreate(module_queue_len, sizeof (com_module));

    get_wifi_credentials(&credentials);

    ESP_LOGI(TAG, "ssid = %s\n", credentials.ssid);
    ESP_LOGI(TAG, "password = %s\n", credentials.password);

    if (check_credentials(&credentials)) {
        setup_wifi();
    } else {
        if (wifi_connect_sta(&credentials)) {

            xTaskCreate(mqtt_publish_state_task,
                        "publish_current_state",
                        1024*4,
                        &mqtt_message_state_module,
                        2,
                        &mqtt_task_handle
            );

            xTaskCreate(mqtt_subscriber_relay_1_task,
                        "subscriber_rele1_state",
                        1024*4,
                        &mqtt_message_relay1,
                        2,
                        NULL
            );

            xTaskCreate(mqtt_subscriber_relay_2_task,
                        "subscriber_rele2_state",
                        1024*4,
                        &mqtt_message_relay2,
                        2,
                        NULL
            );


            xTaskCreate(receive_message_uart,
                        "read_byte_uart",
                        1024*4,
                        &module,
                        2,
                        NULL
            );

            xTaskCreate(mqtt_publish_data_task,
                        "publish_values_got",
                        1024*4,
                        &module,
                        2,
                        &mqtt_task_handle
            );

            xTaskCreate(reset_credentials_wifi_button,
                        "reset_credentials_wifi_button",
                        1024*4,
                        NULL,
                        2,
                        NULL
            );

        }
    }
}