# 🎓 Mini-RTOS: The Deep Dive

You have built a **Real-Time Operating System (RTOS)**. This is a massive achievement.
This guide explains *what* you built, *why* it works, and *how* to take it further.

## 1. What is an RTOS?
An **Operating System (OS)** manages hardware resources (CPU, RAM, Peripherals) and provides services to applications.
- **GPOS (General Purpose OS)**: Windows, Linux, macOS. Optimized for throughput and user experience. Fairness is key.
- **RTOS (Real-Time OS)**: FreeRTOS, Zephyr, VxWorks. Optimized for **Determinism**. Timing is key.

**Your Mini-RTOS** is a "Cooperative" (if you yield) or "Preemptive" (because of SysTick) scheduler. Since we use SysTick to force switches, it is **Preemptive**.

## 2. The Core Components

### A. The TCB (Task Control Block)
Every task needs a "backpack" to carry its stuff when it's not running.
```c
typedef struct {
    uint32_t *sp; // Stack Pointer
} TCB;
```
In a real RTOS, this would also contain:
- `priority`: For priority scheduling.
- `state`: READY, BLOCKED, SUSPENDED.
- `name`: For debugging ("Task 1").

### B. The Stack Frame (The "Fake" Context)
When you create a thread (`os_thread_create`), the thread hasn't run yet. But the CPU *thinks* it was interrupted.
We manually push values to the stack so that when the CPU "returns" from the first interrupt, it "returns" into the start of your function.
- **PC (Program Counter)**: Set to `task_function`.
- **LR (Link Register)**: Set to `0xFFFFFFFD` (Return to Thread Mode using PSP).

### C. The Scheduler (Round Robin)
Your scheduler is simple:
```c
current_task_idx++;
if (current_task_idx >= task_count) current_task_idx = 0;
```
It just goes in a circle. Task 1 -> Task 2 -> Task 3 -> Task 1.
**Pros**: Simple, Fair (everyone gets equal time).
**Cons**: Urgent tasks have to wait their turn.

### D. The Context Switch (PendSV)
We use **PendSV** (Pendable Service Call) because it has the *lowest* priority.
Why?
Imagine a high-priority interrupt (like a Motor Controller) fires. If we context switch *inside* that interrupt, we might delay the motor control logic.
By using PendSV, we ensure the context switch only happens when **no other interrupts are running**.

## 3. End-to-End Flow

1.  **Startup**: `main()` calls `os_kernel_init()` and `os_thread_create()`.
2.  **Launch**: `os_kernel_launch()` sets up SysTick and jumps to Task 1.
3.  **Execution**: Task 1 runs for 10ms (or whatever the quanta is).
4.  **Interrupt**: SysTick fires!
    - Hardware saves Task 1's registers (R0-R3, PC, etc.).
    - `SysTick_Handler` runs. It updates `next_task` to Task 2 and triggers PendSV.
5.  **Switch**: PendSV fires!
    - Saves Task 1's R4-R11.
    - Loads Task 2's SP.
    - Restores Task 2's R4-R11.
6.  **Resume**: PendSV returns. Hardware pops Task 2's registers.
7.  **Task 2 Runs**.

## 4. Next Steps (How to make it "Real")

### A. Sleeping (`os_delay`)
Right now, your tasks use `for(i=0; i<100000; i++);`. This burns battery!
**Better**:
```c
void os_delay(uint32_t ticks) {
    current_task->state = BLOCKED;
    current_task->delay = ticks;
    trigger_context_switch();
}
```
The scheduler would skip BLOCKED tasks until their delay is 0.

### B. Synchronization (Mutexes)
If Task 1 and Task 2 both try to print to UART at the same time, output gets mixed: `HeTalsklo 1...`.
**Solution**:
```c
os_mutex_lock(&uart_mutex);
uart_print("Hello");
os_mutex_unlock(&uart_mutex);
```

### C. Dynamic Memory
We used static arrays (`stack1`, `stack2`). A real OS uses `malloc` to allocate stacks dynamically.

## Summary
You have built the **Kernel**. The engine is running. Now you can build the car (drivers, filesystem, network stack) around it.
