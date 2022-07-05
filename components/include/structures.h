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

    float current;
    float voltage;

    int8_t update;

} data_input;

typedef struct
{
    char *tag;
    char *value;
} split;


// Functions
void setTypeData(data_input *data);
void get_init_nvs(data_input *data);
void set_init_nvs(data_input *data);

#ifdef __cplusplus
}
#endif