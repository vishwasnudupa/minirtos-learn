# Project 5: Mini-RTOS (The Grand Finale)

## 📖 Introduction
In Project 4, we hardcoded the switch (`if task1 then task2`).
In the real world, we want to create tasks dynamically: `create_task(my_function)`.
This is what an **RTOS (Real-Time Operating System)** does.

## 🎯 The Goal
Build a reusable **Kernel** that can manage any number of tasks.
The user code (`main.c`) should be clean and simple. The ugly assembly stuff should be hidden in the Kernel.

## 🧠 Key Concepts

### 1. Abstraction
The most important concept in software engineering.
- **Low Level**: `LDR R0, [R1]` (Assembly)
- **Mid Level**: `PendSV_Handler` (Context Switch)
- **High Level**: `os_thread_create` (API)
We want the user to live in the High Level.

### 2. The TCB (Task Control Block)
Every task needs a "backpack" to carry its state.
```c
typedef struct {
    uint32_t *sp; // The Stack Pointer
} TCB;
```
When a task is not running, its Stack Pointer is saved here.

### 3. The Scheduler
This is the brain. It decides *who runs next*.
We implemented a **Round Robin** scheduler.
- Task 1 -> Task 2 -> Task 3 -> Task 1...
It is simple and fair. Everyone gets an equal slice of time (Quanta).

## 📂 File Breakdown

### `os_kernel.c`
- **`os_thread_create`**: Allocates a TCB and sets up the "fake" stack frame (from Project 4).
- **`os_kernel_launch`**: Configures SysTick and jumps to the first task.
- **`SysTick_Handler`**:
    1.  Saves `current_task`.
    2.  Updates `current_task` to the next one in the array.
    3.  Triggers PendSV.

### `main.c`
Look how clean it is!
```c
os_thread_create(task1, stack1);
os_thread_create(task2, stack2);
os_kernel_launch(100);
```
No assembly. No register manipulation. Just pure C.

## 🚀 Conclusion
You have built a working Operating System from scratch.
You understand:
- How the CPU boots (`startup.s`).
- How to talk to hardware (`uart.c`).
- How interrupts work (`SysTick`).
- How multitasking works (`PendSV`).

You are now an **Embedded Firmware Engineer**.
