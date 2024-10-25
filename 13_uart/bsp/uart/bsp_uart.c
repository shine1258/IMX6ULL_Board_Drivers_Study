#include "bsp_uart.h"
#include "bsp_gpio.h"

static void uart1_io_init(void);
static void uart_enable(UART_Type* uart);
static void uart_disable(UART_Type* uart);
static void uart_soft_reset(UART_Type* uart);

void uart1_init(void)
{
    uart1_io_init();
    uart_disable(UART1);
    uart_soft_reset(UART1);

    /* 使能发送和接收, 设置数据位为8位, 停止位为1位, 无校验位, 忽略RTS引脚
       使能RXDMUXSEL, 设置波特率为115200 */
    UART1->UCR1 = 0;
    UART1->UCR2 = 0;
    UART1->UCR2 = (1 << 1) | (1 << 2) | (1 << 5) | (1 << 14);
    UART1->UCR3 |= (1 << 2);
    uart_setbaudrate(UART1, 115200, 80000000);
    uart_enable(UART1);
}

/*
 * @description 		: 波特率计算公式，
 *    			  	  	  可以用此函数计算出指定串口对应的UFCR，
 * 				          UBIR和UBMR这三个寄存器的值
 * @param - base		: 要计算的串口。
 * @param - baudrate	: 要使用的波特率。
 * @param - srcclock_hz	:串口时钟源频率，单位Hz
 * @return		: 无
 */
void uart_setbaudrate(UART_Type* base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u; // 分子
    uint32_t denominator = 0U; // 分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0) {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK)) {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator) {
            numerator = 1;
        }
        if (0 == denominator) {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider) {
    case 1:
        refFreqDiv = 0x05;
        break;
    case 2:
        refFreqDiv = 0x04;
        break;
    case 3:
        refFreqDiv = 0x03;
        break;
    case 4:
        refFreqDiv = 0x02;
        break;
    case 5:
        refFreqDiv = 0x01;
        break;
    case 6:
        refFreqDiv = 0x00;
        break;
    case 7:
        refFreqDiv = 0x06;
        break;
    default:
        refFreqDiv = 0x05;
        break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0) {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3) {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); // 要先写UBIR寄存器，然后在写UBMR寄存器，3592页
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}

void uart1_putc(char c)
{
    while (!(UART1->USR2 & (1 << 3))) { }
    UART1->UTXD = c;
}

char uart1_getc(void)
{
    while (!(UART1->USR2 & (1 << 0))) { }
    return UART1->URXD;
}

void uart1_puts(char* str)
{
    while (*str) {
        uart1_putc(*str++);
    }
}

static void uart1_io_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);
}

static void uart_enable(UART_Type* uart)
{
    uart->UCR1 |= (1 << 0);
}

static void uart_disable(UART_Type* uart)
{
    uart->UCR1 &= ~(1 << 0);
}

static void uart_soft_reset(UART_Type* uart)
{
    uart->UCR2 &= ~(1 << 0);
    while (!(uart->UCR2 & (1 << 0))) { }
}

/// @brief 防止编译器报错
/// @param sig_nr
void raise(int sig_nr) { }