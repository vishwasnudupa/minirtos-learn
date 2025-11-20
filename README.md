# Bare Metal ARM: From Scratch to RTOS

Welcome to the **Bare Metal ARM Learning Path**.
This repository contains a step-by-step course to learn Embedded Firmware Development, starting from an empty directory and ending with a fully functional **Real-Time Operating System (RTOS)**.

## 🎯 Goal
To demystify how computers work. No libraries, no HALs, no magic. Just you, the datasheet, and the hardware.

## 🛠 Environment
- **Architecture**: ARM Cortex-M3 (Emulated via QEMU)
- **Toolchain**: GCC (arm-none-eabi)
- **Platform**: Linux (WSL) or Native Linux

## 📚 Course Structure

### Phase 1: The Foundation
- **[Setup Guide](Setup_Guide.md)**: How to install the tools (WSL, GCC, QEMU).
- **[Project 0: Environment Setup](Project_0_Setup/README.md)**: Creating the Makefile and Linker Script.
- **[Project 1: Startup Code](Project_1_Startup/README.md)**: Writing the Assembly bootloader (`startup.s`) and C Runtime init.
    - *Theory*: **[What is an Assembly (.s) file?](Concepts_Assembly.md)**

### Phase 2: Peripherals
- **[Project 2: UART Driver](Project_2_UART/README.md)**: Memory Mapped I/O. Writing a driver to print "Hello World".
- **[Project 3: Interrupts](Project_3_Timers/README.md)**: Configuring the NVIC and SysTick timer.

### Phase 3: Operating Systems
- **[Project 4: Context Switching](Project_4_ContextSwitch/README.md)**: The magic of Multitasking. Manually switching CPU stacks.
- **[Project 5: Mini-RTOS](Project_5_RTOS/README.md)**: Building a reusable Kernel with a Round-Robin Scheduler.
    - *Theory*: **[Mini-RTOS Deep Dive](MiniRTOS_DeepDive.md)**

## 🚀 How to Run
1.  Enter a project directory: `cd Project_5_RTOS`
2.  Build: `make`
3.  Run: `make run`
4.  Debug: `make debug` (in one terminal) and `gdb-multiarch` (in another).

## 🧠 Key Concepts Learned
- **Linker Scripts**: Controlling memory layout.
- **Startup Assembly**: Vector Tables, Stack Pointers, Reset Handlers.
- **ABI**: Procedure Call Standard (AAPCS).
- **Memory Mapped I/O**: Controlling hardware via pointers.
- **Interrupts**: NVIC, SysTick, PendSV.
- **Context Switching**: Saving/Restoring register state.

## License
Open Source. Use this to learn, teach, and build amazing things.
