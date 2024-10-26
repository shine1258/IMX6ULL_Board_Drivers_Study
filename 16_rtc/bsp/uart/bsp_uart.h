#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "imx6u.h"

void uart1_init(void);
void uart_setbaudrate(UART_Type* base, unsigned int baudrate, unsigned int srcclock_hz);
void putc(unsigned char c);
unsigned char getc(void);
void puts(char* str);

#endif // !__BSP_UART_H