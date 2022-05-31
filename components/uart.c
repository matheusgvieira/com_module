#include "uart.h"
#include <string.h>
#include <stdlib.h>


void initUart()
{
    uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, RX_BUF_SIZE, 0, 0, NULL, 0);
}

char* readByte()
{
    memset(byte_message, 0, sizeof(byte_message));
    uart_read_bytes(UART_NUM_0, (uint8_t *)byte_message, RX_BUF_SIZE, pdMS_TO_TICKS(500));
    return byte_message;

}

int8_t writeByte(char* tag, char* value)
{
    strcat(tag,value);
    return uart_write_bytes(UART_NUM_0, tag, sizeof(tag));
}
