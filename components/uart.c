#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern QueueHandle_t module_queue;

static void split_type_data(com_module *module) {
    split_tag_module(module);

    ESP_LOGI("split_type_data", "current=[%.4f] | voltage=[%.4f] | power=[%.4f] | energy=[%.4f] | update=[%d] \n",
             module -> current,
             module -> voltage,
             module -> power,
             module -> energy,
             module -> update);
};

void initialization_uart(void)
{
    const uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(
            UART,
            RX_BUF_SIZE * 2,
            0,
            0,
            NULL,
            0
            );

    uart_param_config(
            UART,
            &uart_config
            );

    uart_set_pin(
            UART,
            TXD_PIN,
            RXD_PIN,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
            );
}

void receive_message_uart(void *pvParameters)
{
    com_module module = *(com_module *) pvParameters;

    static const char *RX_TASK_TAG = "UART_RX | ";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);

    uint8_t* bytes_received_rx = (uint8_t*) malloc(RX_BUF_SIZE);

    printf("\nUART | initialization! \n");

    while(1) {
        const int len_bytes_received_rx = uart_read_bytes(
                UART,
                bytes_received_rx,
                (RX_BUF_SIZE - 1),
                20 / portTICK_PERIOD_MS);

        if (len_bytes_received_rx) {
            bytes_received_rx[len_bytes_received_rx] = '\0';
            char* value_received = (char *) bytes_received_rx;

            if(strcmp(value_received, "") != 0) {
                ESP_LOGI(RX_TASK_TAG, "Data received: %s\n", value_received);
                module.read_uart = value_received;
                split_type_data(&module);

                xQueueSend( module_queue, (void *)&module, 10 );
            }
        }
    }

    free(bytes_received_rx);
}

void send_message_uart(int8_t select_relay, int8_t value)
{
    // Dynamically allocate a single large block of memory with the specified size of data sended uart
    char* bytes_sended_tx = (char*) malloc(100);

    // sends formatted output to a string pointed to, by bytes_sended_tx.
    if (select_relay == 0) {
        sprintf(bytes_sended_tx, "r0%d\r\n", value);
    }

    if (select_relay == 1) {
        sprintf(bytes_sended_tx, "r%d0\r\n", value);
    }

    // Send data to the UART port from a given buffer and length,.
    uart_write_bytes(
            UART,
            bytes_sended_tx,
            strlen(bytes_sended_tx)
    );
}