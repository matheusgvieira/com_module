//
// Created by Matheus Gois Vieira on 27/06/22.
//

#include "structures.h"

static void split_tag_value(char *tag_value, split *tag_value_splited) {
    tag_value_splited->tag = strtok(tag_value, "-");
    tag_value_splited->value = strtok(NULL, "-");
}

void setTypeData(data_input *module) {
    split tag_value_splited = {.tag = "", .value = ""};
    float voltage = 0.0, current = 0.0;

    split_tag_value(module ->read_uart, &tag_value_splited);

    if(strcmp(tag_value_splited.tag, "V") == 0) {
        voltage = atof(tag_value_splited.value);

        if(module -> voltage != voltage) {
            module -> voltage = voltage;
            module -> update = 1;
        }
    }
    if(strcmp(tag_value_splited.tag, "C") == 0) {
        current = atof(tag_value_splited.value);

        if(module -> current != current) {
            module -> current = current;
            module -> update = 1;
        }
    }
};
