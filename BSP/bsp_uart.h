#ifndef VOFA_H
#define VOFA_H


#include "main.h"


typedef struct {
    float data;
} __attribute__ ((packed)) Float_Data;


void BSP_UART_TransmitFloat(UART_HandleTypeDef *huartx, int num, ...);
void BSP_UART_Transmit(UART_HandleTypeDef *huartx, const char *format, ...);


#endif
