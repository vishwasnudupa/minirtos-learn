#include "uart.h"

void uart_init(void) {
    // 1. Disable UART before configuration
    UART0_CTL = 0;
    
    // 2. Set Baud Rate (Assuming 16MHz clock, 115200 baud)
    // Divider = 16,000,000 / (16 * 115200) = 8.6805
    // Integer part = 8
    // Fractional part = 0.6805 * 64 + 0.5 = 44
    UART0_IBRD = 8;
    UART0_FBRD = 44;
    
    // 3. Configure Line Control (8 bits, no parity, 1 stop bit, FIFO enable)
    // Bit 4 (FEN) = 1 (FIFO Enable)
    // Bit 5,6 (WLEN) = 11 (8 bits)
    UART0_LCRH = (1 << 4) | (3 << 5);
    
    // 4. Enable UART
    // Bit 0 (UARTEN) = 1
    // Bit 8 (TXE) = 1
    // Bit 9 (RXE) = 1
    UART0_CTL = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_putc(char c) {
    // Wait until there is space in the Transmit FIFO
    // Check the TXFF (Transmit FIFO Full) bit
    while (UART0_FR & UART_FR_TXFF) {
        // Spin wait
    }
    
    // Write character to Data Register
    UART0_DR = c;
}

void uart_print(const char *str) {
    while (*str) {
        uart_putc(*str);
        str++;
    }
}
