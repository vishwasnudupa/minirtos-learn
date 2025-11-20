# Project 4: Context Switching (The Magic Trick)

## 📖 Introduction
This is the most difficult and rewarding part of the course.
We are going to make a single CPU run two tasks "simultaneously".
This is how Windows runs Chrome and Spotify at the same time. It switches between them thousands of times per second.

## 🎯 The Goal
Manually switch the CPU from executing `task1()` to `task2()` and back.

## 🧠 Key Concepts

### 1. The "Save Game" Analogy
- **Task 1** is playing a game.
- **Interrupt** (Mom calls for dinner).
- **Save**: We must save Task 1's progress (Registers R0-R15) to memory (Task 1's Stack).
- **Switch**: We load Task 2's save file (Task 2's Stack).
- **Restore**: We put Task 2's values into the registers.
- **Resume**: The CPU continues Task 2 as if it never stopped.

### 2. PendSV (Pendable Service Call)
We use a special interrupt called **PendSV**.
Why not switch inside the SysTick interrupt?
Because SysTick might happen while *another* interrupt is running. Switching stacks in the middle of an interrupt handler is dangerous.
PendSV has the **lowest priority**. We trigger it, and the CPU waits until all other interrupts are done before running it. This is safe.

### 3. The Stack Frame
The CPU has 16 registers.
- **Hardware** saves 8 of them (`R0-R3`, `R12`, `LR`, `PC`, `xPSR`).
- **Software** (Us) must save the other 8 (`R4-R11`).

## 📂 File Breakdown

### `main.c` -> `task_init`
We have to "fake" the first save file.
When Task 1 starts, it has never run before. So we manually write values to its stack that look like a saved context.
We set the **PC (Program Counter)** to the start of the function.

### `startup.s` -> `PendSV_Handler`
This is pure assembly magic.
1.  `mrs r0, psp`: Get the current stack pointer.
2.  `stmdb r0!, {r4-r11}`: Save the "software" registers.
3.  `str r0, [current_task]`: Save the SP to the TCB.
4.  `ldr r0, [next_task]`: Load the SP of the next task.
5.  `ldmia r0!, {r4-r11}`: Restore the "software" registers.
6.  `msr psp, r0`: Update the CPU's stack pointer.
7.  `bx lr`: Return. The hardware restores the rest.

## 🚀 How it Works
You see `Task 1` and `Task 2` printing alternately.
The CPU is jumping between two completely different stacks, preserving the state of each function perfectly.
