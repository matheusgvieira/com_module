#include "mqtt.h"


esp_mqtt_client_config_t get_mqtt_cfg() {

    const esp_mqtt_client_config_t mqtt_cfg = {
            .broker.address.uri = CONFIG_BROKER_URL,
            .credentials.username = "Token #2",
            .credentials.authentication.password = "a6fbba72-067c-4309-be80-ac15ec1c1aa7",
    };

    return mqtt_cfg;

}