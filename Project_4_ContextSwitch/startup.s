/* Startup file for Cortex-M3 */
.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.global Reset_Handler

/* Vector Table */
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word _estack             /* Top of Stack */
  .word Reset_Handler       /* Reset Handler */
  .word 0                   /* NMI Handler */
  .word 0                   /* Hard Fault Handler */
  .word 0                   /* MPU Fault Handler */
  .word 0                   /* Bus Fault Handler */
  .word 0                   /* Usage Fault Handler */
  .word 0                   /* Reserved */
  .word 0                   /* Reserved */
  .word 0                   /* Reserved */
  .word 0                   /* Reserved */
  .word 0                   /* SVCall Handler */
  .word 0                   /* Debug Monitor Handler */
  .word 0                   /* Reserved */
  .word PendSV_Handler      /* PendSV Handler */
  .word SysTick_Handler     /* SysTick Handler */

/* Reset Handler */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
.weak SysTick_Handler
.type SysTick_Handler, %function
SysTick_Handler:
  b .

.type PendSV_Handler, %function
.global PendSV_Handler
PendSV_Handler:
  /* 1. Disable Interrupts */
  cpsid i

  /* 2. Save Context of Current Task */
  /* Hardware already saved R0-R3, R12, LR, PC, xPSR */
  /* We must save R4-R11 */
  mrs r0, psp               /* Get current Process Stack Pointer */
  stmdb r0!, {r4-r11}       /* Save R4-R11 to stack */

  /* Save SP to current_task->sp */
  ldr r1, =current_task     /* Load address of current_task pointer */
  ldr r2, [r1]              /* Load value of current_task (pointer to TCB) */
  str r0, [r2]              /* Store SP into TCB (first member) */

  /* 3. Load Context of Next Task */
  ldr r3, =next_task        /* Load address of next_task pointer */
  ldr r2, [r3]              /* Load value of next_task (pointer to TCB) */
  str r2, [r1]              /* current_task = next_task */
  
  ldr r0, [r2]              /* Load SP from TCB (first member) */
  ldmia r0!, {r4-r11}       /* Restore R4-R11 from stack */
  msr psp, r0               /* Set PSP */

  /* 4. Enable Interrupts and Return */
  cpsie i
  bx lr                     /* Return using LR (EXC_RETURN) */
Reset_Handler:
  /* 1. Copy .data segment from Flash to RAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

  /* 2. Zero fill the .bss segment */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

  /* 3. Call the application's entry point. */
  bl main
  
  /* Infinite loop if main returns */
LoopForever:
    b LoopForever
.size Reset_Handler, .-Reset_Handler
