#include "uart.h"

int main(void) {
    // Initialize UART
    uart_init();
    
    // Say Hello!
    uart_print("Hello World from Bare Metal ARM!\n");
    
    while (1) {
        // Echo loop (Optional: if we implemented read)
    }
    
    return 0;
}
