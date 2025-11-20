# Project 1: Bare Metal Startup

## 📖 Introduction
When you write a C program on your laptop, the OS (Windows/Linux) sets up the stack, heap, and variables for you before `main()` starts.
On a microcontroller, **there is no OS**. You are the OS.
If you just jump to `main()`, the program will crash because the "Stack Pointer" is undefined.

## 🎯 The Goal
Write the **Bootloader** (Startup Code) in Assembly Language.
This code must:
1.  Set up the **Stack Pointer**.
2.  Copy **Initialized Data** (`.data`) from Flash to RAM.
3.  Zero out **Uninitialized Data** (`.bss`) in RAM.
4.  Jump to `main()`.

## 📂 File Breakdown

### 1. `startup.s` (The Bootloader)
This is Assembly Language. It talks directly to the CPU registers.

#### The Vector Table
The Cortex-M3 expects the *first thing* in Flash to be a table of pointers.
```asm
.word _estack        /* Address 0x00: Initial Stack Pointer */
.word Reset_Handler  /* Address 0x04: Reset Handler (Start Here!) */
```
- **`_estack`**: Calculated in `linker.ld` (End of RAM).
- **`Reset_Handler`**: The function the CPU runs when powered on.

#### The Reset Handler
```asm
Reset_Handler:
  /* Copy .data from Flash to RAM */
  ldr r0, =_sdata    /* Start of RAM data */
  ldr r1, =_edata    /* End of RAM data */
  ldr r2, =_sidata   /* Start of Flash data */
  /* ...loop to copy... */
  
  /* Zero .bss */
  ldr r0, =_sbss
  ldr r1, =_ebss
  /* ...loop to write 0... */
  
  bl main            /* Branch to C code */
```

### 2. `main.c` (The Application)
Now that the environment is ready, we can write normal C code.
```c
int x = 5;      // Goes to .data (Initialized)
int y;          // Goes to .bss (Zeroed)
```
If we didn't write `startup.s`, `x` would be garbage and `y` would be garbage.

## 🧪 How it Works
1.  **Power On**: CPU reads address 0x00, sets Stack Pointer.
2.  **Reset**: CPU reads address 0x04, jumps to `Reset_Handler`.
3.  **Housekeeping**: `Reset_Handler` copies data and clears BSS.
4.  **Handover**: `Reset_Handler` calls `main()`.
5.  **Application**: `main()` runs forever.

## 🚀 Verification
Run `make debug` and use GDB to inspect memory. You will see `x` at a RAM address (e.g., `0x20000000`) holding the value `5`.
