idf_component_register(SRCS "led.c" "buzzer.c" "uart.c" "wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "json"
                    PRIV_REQUIRES nvs_flash)

