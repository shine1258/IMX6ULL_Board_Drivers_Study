#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "imx6u.h"

void uart1_init(void);
void uart_setbaudrate(UART_Type* base, unsigned int baudrate, unsigned int srcclock_hz);
void uart1_putc(char c);
char uart1_getc(void);
void uart1_puts(char* str);

#endif // !__BSP_UART_H