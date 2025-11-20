#include "uart.h"
#include <stdint.h>

// --- OS Kernel Definitions ---

// Task Control Block
typedef struct {
    uint32_t *sp; // Stack Pointer (Must be first member for assembly)
} TCB;

TCB tcb1, tcb2;
TCB *current_task;
TCB *next_task;

// Stack Size
#define STACK_SIZE 1024
uint32_t stack1[STACK_SIZE];
uint32_t stack2[STACK_SIZE];

// SysTick Registers
#define SYSTICK_CSR   (*((volatile uint32_t *)0xE000E010))
#define SYSTICK_RVR   (*((volatile uint32_t *)0xE000E014))
#define SYSTICK_CVR   (*((volatile uint32_t *)0xE000E018))
#define SYSTICK_ENABLE    (1 << 0)
#define SYSTICK_TICKINT   (1 << 1)
#define SYSTICK_CLKSOURCE (1 << 2)

// Interrupt Control State Register (for triggering PendSV)
#define ICSR (*((volatile uint32_t *)0xE000ED04))
#define ICSR_PENDSVSET (1 << 28)

// --- Task Functions ---

void task1_func(void) {
    while (1) {
        uart_print("Task 1: Running...\n");
        // Busy wait to slow it down
        for (volatile int i = 0; i < 100000; i++);
    }
}

void task2_func(void) {
    while (1) {
        uart_print("Task 2: Running...\n");
        // Busy wait to slow it down
        for (volatile int i = 0; i < 100000; i++);
    }
}

// --- Kernel Functions ---

void task_init(TCB *tcb, uint32_t *stack, void (*func)(void)) {
    // Stack grows down. Start at the top.
    uint32_t *sp = stack + STACK_SIZE;
    
    // Forge the Exception Frame (Hardware saved context)
    // xPSR, PC, LR, R12, R3, R2, R1, R0
    sp--; *sp = 0x01000000; // xPSR (Thumb bit set)
    sp--; *sp = (uint32_t)func; // PC (Return Address)
    sp--; *sp = 0xFFFFFFFD; // LR (Return to Thread Mode, PSP)
    sp--; *sp = 0; // R12
    sp--; *sp = 0; // R3
    sp--; *sp = 0; // R2
    sp--; *sp = 0; // R1
    sp--; *sp = 0; // R0
    
    // Forge the Software Saved Context (R4-R11)
    for (int i = 0; i < 8; i++) {
        sp--; *sp = 0;
    }
    
    // Save the stack pointer to the TCB
    tcb->sp = sp;
}

void SysTick_Handler(void) {
    // Simple Round-Robin Scheduler
    if (current_task == &tcb1) {
        next_task = &tcb2;
    } else {
        next_task = &tcb1;
    }
    
    // Trigger PendSV to perform the switch
    ICSR |= ICSR_PENDSVSET;
}

int main(void) {
    uart_init();
    uart_print("Initializing OS...\n");
    
    // Initialize Tasks
    task_init(&tcb1, stack1, task1_func);
    task_init(&tcb2, stack2, task2_func);
    
    // Start with Task 1
    current_task = &tcb1;
    
    // Configure SysTick (100ms for faster switching)
    SYSTICK_RVR = 0xB71B00 / 10; 
    SYSTICK_CVR = 0;
    SYSTICK_CSR = SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSOURCE;
    
    // Switch to Process Stack Pointer (PSP) for tasks
    // We are currently using MSP (Main Stack Pointer).
    // We need to switch to PSP before starting the first task.
    // But for simplicity, we will just let the first PendSV handle it.
    // However, PendSV expects us to be using PSP if we return to Thread Mode.
    
    // HACK: To start the first task, we will just manually load its SP and jump.
    // This is cleaner than handling the "first switch" edge case in PendSV.
    
    __asm volatile (
        "ldr r0, =tcb1 \n"       // Load address of tcb1
        "ldr r1, [r0] \n"        // Load SP from tcb1
        "msr psp, r1 \n"         // Set PSP
        "movs r0, #2 \n"         // Switch to PSP (Control Register bit 1)
        "msr control, r0 \n"
        "isb \n"
        "ldr r1, =task1_func \n" // Load address of task1
        "bx r1 \n"               // Jump to task1
    );
    
    while (1) {
        // Should never get here
    }
    
    return 0;
}
