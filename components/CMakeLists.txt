idf_component_register(SRCS "led.c" "buzzer.c" "uart.c" "wifi.c" "mqtt.c" "structures.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "json" "mqtt"
                    PRIV_REQUIRES nvs_flash)

