//
// Created by Matheus Gois Vieira on 27/06/22.
//

#include "structures.h"

void split_tag_module(com_module * module) {
    char * voltage_element = strtok(module->read_uart, "\n");
    char * current_element = strtok(NULL, "\n");
    char * power_element = strtok(NULL, "\n");
    char * energy_element = strtok(NULL, "\n");

    strtok(voltage_element, "-");
    module->voltage = atof(strtok(NULL, "-"));

    strtok(current_element, "-");
    module->current = atof(strtok(NULL, "-"));

    strtok(power_element, "-");
    module->power = atof(strtok(NULL, "-"));

    strtok(energy_element, "-");
    module->energy = atof(strtok(NULL, "-"));

    module -> update = 1;
}

void get_init_nvs(com_module *data) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    nvs_handle_t nvs_handle;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);

    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Reading voltage from NVS ... \n");
        size_t required_size_voltage;
        nvs_get_str(nvs_handle, "voltage", NULL, &required_size_voltage);
        char *voltage = malloc(required_size_voltage);
        err = nvs_get_str(nvs_handle, "voltage", voltage, &required_size_voltage);

        switch (err) {
            case ESP_OK:
                printf("Get in nvs voltage = %s\n", voltage);
                data -> voltage = atof(voltage);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The voltage is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        printf("Reading current from NVS ... \n");
        size_t required_size_current;
        nvs_get_str(nvs_handle, "current", NULL, &required_size_current);
        char *current = malloc(required_size_current);
        err = nvs_get_str(nvs_handle, "current", current, &required_size_current);

        switch (err) {
            case ESP_OK:
                printf("Get in nvs current = %s\n", current);
                data -> current = atof(current);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The current is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        printf("Reading power from NVS ... \n");
        size_t required_size_power;
        nvs_get_str(nvs_handle, "power", NULL, &required_size_power);
        char *power = malloc(required_size_power);
        err = nvs_get_str(nvs_handle, "power", power, &required_size_power);

        switch (err) {
            case ESP_OK:
                printf("Get in nvs power = %s\n", power);
                data -> power = atof(power);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The power is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        printf("Reading energy from NVS ... \n");
        size_t required_size_energy;
        nvs_get_str(nvs_handle, "energy", NULL, &required_size_energy);
        char *energy = malloc(required_size_energy);
        err = nvs_get_str(nvs_handle, "energy", energy, &required_size_energy);

        switch (err) {
            case ESP_OK:
                printf("Get in nvs energy = %s\n", energy);
                data -> energy = atof(energy);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The energy is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        nvs_close(nvs_handle);
    }
}
void set_init_nvs(com_module *data){
    esp_err_t err = nvs_flash_init();
    nvs_handle_t nvs_handle;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);

    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        // Write voltage
        printf("Updating voltage in NVS ... ");
        char voltage[50];
        sprintf(voltage, "%f", data->voltage);
        err = nvs_set_str(nvs_handle, "voltage", voltage);

        printf((err != ESP_OK) ? "Voltage no update nvs!\n" : "Voltage update nvs.\n");

        // Write current
        printf("Updating current in NVS ... ");
        char current[50];
        sprintf(current, "%f", data->current);
        err = nvs_set_str(nvs_handle, "current", current);

        printf((err != ESP_OK) ? "Current no update nvs!\n" : "Current update nvs.\n");

        // Write power
        printf("Updating power in NVS ... ");
        char power[50];
        sprintf(power, "%f", data->power);
        err = nvs_set_str(nvs_handle, "power", power);

        printf((err != ESP_OK) ? "Power no update nvs!\n" : "Power update nvs.\n");

        // Write energy
        printf("Updating energy in NVS ... ");
        char energy[50];
        sprintf(energy, "%f", data->energy);
        err = nvs_set_str(nvs_handle, "energy", energy);

        printf((err != ESP_OK) ? "Energy no update nvs!\n" : "Energy update nvs.\n");

        printf("Committing updates in NVS ... ");
        err = nvs_commit(nvs_handle);
        printf((err != ESP_OK) ? "No commit nvs!!\n" : "Commit nvs!\n");

        // Close
        nvs_close(nvs_handle);
    }
};
