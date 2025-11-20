#ifndef OS_KERNEL_H
#define OS_KERNEL_H

#include <stdint.h>

// Task Control Block
typedef struct {
    uint32_t *sp; // Stack Pointer (Must be first member)
} TCB;

// API
void os_kernel_init(void);
void os_kernel_launch(uint32_t quanta);
uint8_t os_thread_create(void (*func)(void), uint32_t *stack_start);

#endif
