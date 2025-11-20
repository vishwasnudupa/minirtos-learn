# Project 3: Timers & Interrupts (The Doorbell)

## 📖 Introduction
In Project 2, we used **Polling** ("Are you ready?"). This wastes CPU time.
Imagine waiting for a pizza.
- **Polling**: You stand at the door and stare at the street. You can't do anything else.
- **Interrupts**: You sit on the couch and watch TV. When the pizza arrives, the doorbell rings. You pause TV, get pizza, resume TV.

## 🎯 The Goal
Configure the **SysTick Timer** to ring the doorbell (Interrupt) every 1 second.
While waiting, the CPU can do other things (or sleep).

## 🧠 Key Concepts

### 1. The NVIC (Nested Vectored Interrupt Controller)
This is a special part of the Cortex-M core that manages interrupts.
It decides who gets to run. If a high-priority interrupt fires while a low-priority one is running, the NVIC pauses the low one and runs the high one (**Nesting**).

### 2. The Vector Table (Again)
Remember the table in `startup.s`?
```asm
.word SysTick_Handler     /* Entry 15 */
```
When the SysTick timer hits zero, the hardware looks at Entry 15. It jumps to whatever address is stored there.

### 3. Context Saving
When an interrupt happens, the CPU must remember what it was doing.
It automatically pushes registers `R0`, `R1`, `R2`, `R3`, `R12`, `LR`, `PC`, `xPSR` to the stack.
This is called the **Exception Frame**.
When the interrupt is done, it pops these values back, and the main program resumes exactly where it left off.

## 📂 File Breakdown

### `main.c`
```c
// Enable SysTick with Interrupts
SYSTICK_CSR = ENABLE | TICKINT | CLKSOURCE;
```
- **`TICKINT`**: This bit enables the doorbell. Without it, the timer counts but never interrupts.

### `startup.s`
We added `SysTick_Handler` to the vector table. If we didn't, the CPU would jump to a default handler (usually an infinite loop), and the system would crash.

## 🚀 How it Works
1.  `main()` sets up the timer and enters `while(1)`.
2.  1 second passes.
3.  Timer hits 0. Hardware pauses `main()`.
4.  Hardware saves context to stack.
5.  Hardware jumps to `SysTick_Handler`.
6.  We print "Tick...".
7.  Function returns. Hardware restores context.
8.  `main()` resumes.
