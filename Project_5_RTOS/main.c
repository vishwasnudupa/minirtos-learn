#include "os_kernel.h"
#include "uart.h"

#define STACK_SIZE 1024
uint32_t stack1[STACK_SIZE];
uint32_t stack2[STACK_SIZE];
uint32_t stack3[STACK_SIZE];

void task1(void) {
    while (1) {
        uart_print("Task 1: Running...\n");
        for (volatile int i = 0; i < 100000; i++);
    }
}

void task2(void) {
    while (1) {
        uart_print("Task 2: Running...\n");
        for (volatile int i = 0; i < 100000; i++);
    }
}

void task3(void) {
    while (1) {
        uart_print("Task 3: Running...\n");
        for (volatile int i = 0; i < 100000; i++);
    }
}

int main(void) {
    uart_init();
    uart_print("Initializing Mini-RTOS...\n");
    
    os_kernel_init();
    
    os_thread_create(task1, stack1);
    os_thread_create(task2, stack2);
    os_thread_create(task3, stack3);
    
    uart_print("Starting Scheduler...\n");
    os_kernel_launch(0xB71B00 / 10); // 100ms quanta
    
    while (1) {}
}
