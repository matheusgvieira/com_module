//
// Created by Matheus Gois Vieira on 14/09/22.
//

#ifndef COM_MODULE_EVENTS_H
#define COM_MODULE_EVENTS_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mqtt.h"

void mqtt_event_handler_publish(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

#endif //COM_MODULE_EVENTS_H
