# 🎓 Concept: What is a `.s` file?

In Projects 1 and 2, you saw a file named `startup.s`. This is an **Assembly Language** source file.

## 1. The "Translation" Hierarchy
Computers don't understand C. They only understand **Machine Code** (1s and 0s).
1.  **C Code (`.c`)**: Human-readable logic (`if`, `while`, `x = y + 1`).
2.  **Assembly (`.s`)**: Human-readable machine instructions (`LDR`, `MOV`, `ADD`).
3.  **Machine Code (`.bin`)**: Raw binary numbers (`0xE000E010`).

The compiler (`gcc`) translates C to Assembly. The assembler (`as`) translates Assembly to Machine Code.

## 2. Why did we write `.s` manually?
If the compiler generates Assembly, why did we write `startup.s` by hand?

**Because C needs a "Stage" to perform.**
C code assumes certain things exist:
1.  **A Stack**: To store local variables.
2.  **Global Variables**: Initialized to their correct values.
3.  **A Return Address**: Where to go when a function ends.

When the chip first wakes up (Reset), **none of this exists**.
- The RAM is random garbage.
- The Stack Pointer is undefined.
- The CPU is dumb and doesn't know where `main()` is.

**The `.s` file is the Stage Crew.**
It runs *before* the show (C code) begins.
1.  **Sets up the Stack Pointer**: "Here is the memory area for temporary storage."
2.  **Initializes Memory**: "Let me clean up the RAM and unpack the props (variables)."
3.  **Calls `main()`**: "The stage is ready. Action!"

## 3. Key Parts of `startup.s`
In Project 1, we wrote:

### A. The Vector Table
```asm
.word _estack        /* 1. Initial Stack Pointer */
.word Reset_Handler  /* 2. Start here! */
```
This is the very first thing the CPU reads. It tells the CPU: "Your stack is at the end of RAM, and your code starts at `Reset_Handler`."

### B. The Reset Handler
```asm
Reset_Handler:
  bl main
```
This is the first instruction executed. `bl` means "Branch with Link" (Call function). It jumps to your C `main` function.

## Summary
- **`.s` files** contain low-level Assembly instructions.
- We use them for **Startup Code** because we need to set up the hardware environment *before* C code can run.
- Without `startup.s`, your `main.c` would crash instantly because it would try to save variables to a stack that doesn't exist!
