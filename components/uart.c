#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int num = 0;

/*
 * Initialization of uart:
 *      - Install driver
 *      - Params of configuration
 *      - Setting pins
 * */

void initializationUart()
{
    // UART configuration parameters for uart_param_config function.
    const uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
    };

    // Install UART driver
    uart_driver_install(
            UART,
            RX_BUF_SIZE * 2,
            0,
            0,
            NULL,
            0
            );

    // Set UART configuration parameters.
    uart_param_config(
            UART,
            &uart_config
            );

    // Set UART pins
    uart_set_pin(
            UART,
            TXD_PIN,
            RXD_PIN,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
            );
}

/*
 * Function for readbyte of UART
 */
void readByteUart(void *arg)
{
    // Params for default log ESP
    static const char *RX_TASK_TAG = "COM_MODULE_RX";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);

    // Dynamically allocate a single large block of memory with the specified size of data received uart
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);

    // Rotine of to read
    while (1) {
        // UART read bytes from UART buffers
        const int bytes_received_rx = uart_read_bytes(
                UART,
                data,
                RX_BUF_SIZE,
                500 / portTICK_PERIOD_MS);

        // Check and print value received
        if (bytes_received_rx > 0) { // The number of bytes read from UART FIFO
            data[bytes_received_rx] = 0;
            ESP_LOGI(RX_TASK_TAG, "BYTE_RECEIVED: '%s'", bytes_received_rx, data);
        }
    }
    free(data);
}

/*
 * Function for send CMOK
 */
void writeByteUart(void *arg)
{
    // Dynamically allocate a single large block of memory with the specified size of data sended uart
    char* bytes_sended_tx = (char*) malloc(100);

    // Rotine for to send
    while (1) {

        // sends formatted output to a string pointed to, by bytes_sended_tx.
        sprintf(bytes_sended_tx, "CMOK");

        // Send data to the UART port from a given buffer and length,.
        uart_write_bytes(
                UART,
                bytes_sended_tx,
                strlen(bytes_sended_tx)
        );
        // Delay of send
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

