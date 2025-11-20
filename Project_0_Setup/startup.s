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
  .word 0                   /* SysTick Handler */

/* Reset Handler */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  /* 
     Normally we would copy .data from Flash to RAM 
     and zero out .bss here. For this minimal example,
     we will skip it to keep things simple.
     But we MUST set the stack pointer (hardware does it, but good practice).
  */
  
  /* Call the application's entry point. */
  bl main
  
  /* Infinite loop if main returns */
LoopForever:
    b LoopForever
.size Reset_Handler, .-Reset_Handler
