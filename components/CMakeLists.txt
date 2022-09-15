idf_component_register(SRCS "generic_digital_output.c" "uart.c" "wifi.c" "structures.c" "events.c" "mqtt_publish_data.c" "mqtt_subscriber_relay_1.c" "mqtt_publish_state.c" "mqtt_subscriber_relay_2.c" "button.c" "mqtt_config.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "json" "mqtt" "spiffs" "esp_http_server" "esp_eth" "esp_wifi"
                    PRIV_REQUIRES nvs_flash)

