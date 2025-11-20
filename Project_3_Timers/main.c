#include "uart.h"

// SysTick Registers (Cortex-M3 Standard)
#define SYSTICK_CSR   (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_RVR   (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_CVR   (*((volatile uint32_t *)0xE000E018))

#define SYSTICK_ENABLE    (1 << 0)
#define SYSTICK_TICKINT   (1 << 1)
#define SYSTICK_CLKSOURCE (1 << 2)

// Interrupt Handler - Called by Hardware!
void SysTick_Handler(void) {
    uart_print("Tick...\n");
}

int main(void) {
    // Initialize UART
    uart_init();
    uart_print("Setting up SysTick...\n");
    
    // Configure SysTick
    // 1. Set Reload Value. QEMU clock is virtual, but let's try a value.
    // If we assume 12MHz, 1 second = 12,000,000 cycles.
    // 0xB71B00 = 12,000,000
    SYSTICK_RVR = 0xB71B00; 
    
    // 2. Clear Current Value
    SYSTICK_CVR = 0;
    
    // 3. Enable SysTick: Enable + TickInt + ClkSource
    SYSTICK_CSR = SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSOURCE;
    
    uart_print("Timer started! Waiting for interrupts...\n");
    
    while (1) {
        // Do nothing. The CPU is free!
        // The interrupt will wake us up to print "Tick..."
    }
    
    return 0;
}
