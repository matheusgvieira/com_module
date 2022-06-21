#include <stdio.h>
#include "led.h"
#include "uart.h"
//#include "wifi.h"


void app_main(void)
{
    initializationUart();

    xTaskCreate(readByteUart, "readByteUart", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(writeByteUart, "writeByteUart", 1024*2, NULL, configMAX_PRIORITIES-2, NULL);

}