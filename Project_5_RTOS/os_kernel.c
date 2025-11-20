#include "os_kernel.h"
#include "uart.h"

// SysTick Registers
#define SYSTICK_CSR   (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_RVR   (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_CVR   (*((volatile uint32_t *)0xE000E018))
#define SYSTICK_ENABLE    (1 << 0)
#define SYSTICK_TICKINT   (1 << 1)
#define SYSTICK_CLKSOURCE (1 << 2)

// Interrupt Control State Register
#define ICSR (*((volatile uint32_t *)0xE000ED04))
#define ICSR_PENDSVSET (1 << 28)

// Max Tasks
#define MAX_TASKS 5
#define STACK_SIZE 1024

TCB tcbs[MAX_TASKS];
TCB *current_task;
TCB *next_task;
uint8_t task_count = 0;
uint8_t current_task_idx = 0;

void os_kernel_init(void) {
    task_count = 0;
    current_task_idx = 0;
}

uint8_t os_thread_create(void (*func)(void), uint32_t *stack_start) {
    if (task_count >= MAX_TASKS) return 0;
    
    TCB *tcb = &tcbs[task_count];
    
    // Stack grows down. Start at the top.
    uint32_t *sp = stack_start + STACK_SIZE;
    
    // Forge the Exception Frame
    sp--; *sp = 0x01000000; // xPSR
    sp--; *sp = (uint32_t)func; // PC
    sp--; *sp = 0xFFFFFFFD; // LR
    sp--; *sp = 0; // R12
    sp--; *sp = 0; // R3
    sp--; *sp = 0; // R2
    sp--; *sp = 0; // R1
    sp--; *sp = 0; // R0
    
    // Forge R4-R11
    for (int i = 0; i < 8; i++) {
        sp--; *sp = 0;
    }
    
    tcb->sp = sp;
    task_count++;
    return 1;
}

void os_kernel_launch(uint32_t quanta) {
    // Configure SysTick
    SYSTICK_RVR = quanta;
    SYSTICK_CVR = 0;
    SYSTICK_CSR = SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSOURCE;
    
    // Start First Task
    current_task = &tcbs[0];
    
    // HACK: Jump to first task
    __asm volatile (
        "ldr r0, =current_task \n"
        "ldr r1, [r0] \n"
        "ldr r2, [r1] \n"        // Load SP
        "msr psp, r2 \n"         // Set PSP
        "movs r0, #2 \n"
        "msr control, r0 \n"     // Switch to PSP
        "isb \n"
        "pop {r4-r11} \n"        // Restore R4-R11 (Manual Load)
        "pop {r0-r3} \n"         // Restore R0-R3 (Fake Exception Return)
        "pop {r12} \n"
        "add sp, sp, #4 \n"      // Skip LR
        "pop {lr} \n"            // Pop PC into LR
        "add sp, sp, #4 \n"      // Skip xPSR
        "bx lr \n"               // Jump to task
    );
}

void SysTick_Handler(void) {
    // Round Robin Scheduler
    current_task = &tcbs[current_task_idx];
    
    current_task_idx++;
    if (current_task_idx >= task_count) {
        current_task_idx = 0;
    }
    
    next_task = &tcbs[current_task_idx];
    
    // Trigger PendSV
    ICSR |= ICSR_PENDSVSET;
}
