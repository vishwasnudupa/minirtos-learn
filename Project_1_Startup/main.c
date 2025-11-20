/* Minimal Main */

// Initialized variable - should be in .data (RAM) and have value 0xDEADBEEF
int my_initialized_var = 0xDEADBEEF;

// Uninitialized variable - should be in .bss (RAM) and have value 0
int my_zero_var;

int main(void) {
    // We made it to C code!
    
    // Increment variables to prove we can write to RAM
    my_initialized_var++;
    my_zero_var++;
    
    while (1) {
        // Do nothing forever
    }
    
    return 0;
}
