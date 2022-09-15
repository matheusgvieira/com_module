//
// Created by Matheus Gois Vieira on 27/06/22.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

// Structures
typedef struct {
    char * read_uart;

    double current;
    double voltage;
    double power;
    double energy;

   char * topic_current;
   char * topic_voltage;
   char * topic_power;
   char * topic_energy;
   char * topic_debug;

    int8_t relay1;
    int8_t relay2;

    int8_t update;

} com_module;

// Functions
void get_init_nvs(com_module * module);
void set_init_nvs(com_module * module);
void split_tag_module(com_module * module);

#ifdef __cplusplus
}
#endif