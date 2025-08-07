#include "bsp_uart.h"
#include "main.h"
#include <stdarg.h>
#include <stdio.h>


#define TX_BUF_SIZE 1024
char tx_buf[TX_BUF_SIZE];


void BSP_UART_TransmitFloat(UART_HandleTypeDef *huartx, int num, ...) {
    Float_Data float_data[10];
    va_list arg;
    va_start(arg, num);
    for (int i = 0; i < num; ++i) {
        float_data[i].data = va_arg(arg, double);
    }
    va_end(arg);
    *(uint32_t *)(float_data + num) = 0x7F800000;
    HAL_UART_Transmit(huartx, (uint8_t *)float_data, 4 * (num + 1),100);
}

void BSP_UART_Transmit(UART_HandleTypeDef *huartx, const char *format, ...) {
    va_list arg;
    uint16_t len;
    va_start(arg, format);
    len = vsnprintf(tx_buf, TX_BUF_SIZE, format, arg);
    va_end(arg);
    HAL_UART_Transmit(huartx, (uint8_t *) tx_buf, len, 100);
}
