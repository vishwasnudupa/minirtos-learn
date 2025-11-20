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
  .word 0                   /* PendSV Handler */
  .word SysTick_Handler     /* SysTick Handler */

/* Reset Handler */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
.weak SysTick_Handler
.type SysTick_Handler, %function
SysTick_Handler:  /* Default handler if not defined in C */
  b .             /* Spin forever */
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
