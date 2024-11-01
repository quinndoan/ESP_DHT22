#ifndef UART_H_
#define UART_H_

#include "library.h"
#include "DHT22.h"
#include "driver/uart.h"
#include <stdlib.h>
#include <string.h>
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define UART UART_NUM_2

void initialize_uart();
int sendData(const char* logName, const char* data);
void tx_task(void *arg);
// void rx_task(void *arg);
void send_environment_data();
#endif