#ifndef UART_H
#define UART_H

#include <stdint.h>

// UART0 Base Address for LM3S6965
#define UART0_BASE      0x4000C000

// Register Offsets
#define UART_DR_OFFSET  0x000 // Data Register
#define UART_FR_OFFSET  0x018 // Flag Register
#define UART_IBRD_OFFSET 0x024 // Integer Baud Rate Divisor
#define UART_FBRD_OFFSET 0x028 // Fractional Baud Rate Divisor
#define UART_LCRH_OFFSET 0x02C // Line Control
#define UART_CTL_OFFSET 0x030 // Control

// Register Pointers
#define UART0_DR    (*((volatile uint32_t *)(UART0_BASE + UART_DR_OFFSET)))
#define UART0_FR    (*((volatile uint32_t *)(UART0_BASE + UART_FR_OFFSET)))
#define UART0_IBRD  (*((volatile uint32_t *)(UART0_BASE + UART_IBRD_OFFSET)))
#define UART0_FBRD  (*((volatile uint32_t *)(UART0_BASE + UART_FBRD_OFFSET)))
#define UART0_LCRH  (*((volatile uint32_t *)(UART0_BASE + UART_LCRH_OFFSET)))
#define UART0_CTL   (*((volatile uint32_t *)(UART0_BASE + UART_CTL_OFFSET)))

// Flag Register Bits
#define UART_FR_TXFF    (1 << 5) // Transmit FIFO Full
#define UART_FR_RXFE    (1 << 4) // Receive FIFO Empty

// Function Prototypes
void uart_init(void);
void uart_putc(char c);
void uart_print(const char *str);

#endif
