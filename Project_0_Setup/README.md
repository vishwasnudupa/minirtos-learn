# Project 0: Environment Setup & The Build System

## 📖 Introduction
Before we write any code, we need a way to translate our human-readable text into machine-readable binary. This is the job of the **Toolchain**.
In this project, we set up the "scaffolding" that will support all future projects.

## 🎯 The Goal
To create a working "Build System" that can:
1.  **Compile**: Translate C to Assembly.
2.  **Assemble**: Translate Assembly to Machine Code.
3.  **Link**: Arrange the Machine Code into the correct memory addresses.
4.  **Emulate**: Run the result on a fake chip (QEMU).

## 📂 File Breakdown

### 1. `Makefile` (The Recipe)
Think of `make` as a chef, and the `Makefile` as the recipe.
```makefile
CC = arm-none-eabi-gcc       # The Compiler
CFLAGS = -mcpu=cortex-m3     # Target the Cortex-M3 processor
         -mthumb             # Use Thumb-2 instruction set (efficient)
         -g                  # Add debug info (so GDB works)
```
- **`all:`**: The default target. It says "I want the `.elf` file".
- **`%.o: %.c`**: A rule that says "To make a `.o` file, take a `.c` file and run GCC".

### 2. `linker.ld` (The Map)
The compiler produces code, but it doesn't know *where* that code will live in the chip's memory. The **Linker Script** decides this.
```ld
MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K
}
```
- **FLASH**: Where code lives. It is "Read-Only" (rx) and starts at address 0.
- **RAM**: Where variables live. It is "Read-Write" (rwx) and starts at 0x20000000.
- **SECTIONS**:
    - `.text`: The code itself (goes to FLASH).
    - `.data`: Initialized variables (lives in FLASH, copies to RAM).
    - `.bss`: Zero-initialized variables (lives in RAM).

## 🧪 How it Works
1.  You type `make`.
2.  `make` runs `gcc` to create `main.o` and `startup.o`.
3.  `make` runs `ld` to combine them into `firmware.elf` using the map in `linker.ld`.
4.  `make` runs `objcopy` to strip out debug info and create the raw `firmware.bin`.

## 🚀 Running It
```bash
make run
```
This launches QEMU. Since `main.c` is just an infinite loop, it will appear to hang. This is good! It means the CPU is running our loop.
