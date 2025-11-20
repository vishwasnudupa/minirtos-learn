# Project 2: UART Driver (Talking to the World)

## 📖 Introduction
A microcontroller is useless if it can't talk to the outside world.
On a PC, you use `printf`. But `printf` is a massive function that eventually calls the OS.
We don't have an OS. We have to talk to the hardware directly.

## 🎯 The Goal
Write a driver for the **UART (Universal Asynchronous Receiver-Transmitter)**.
This peripheral sends data bit-by-bit over a wire (or in our case, to the QEMU console).

## 🧠 Key Concepts

### 1. Memory Mapped I/O
How do you talk to a hardware chip?
In ARM, hardware peripherals look like **Magic Memory Addresses**.
- If you write to address `0x20000000`, you save a variable in RAM.
- If you write to address `0x4000C000`, you send a character over UART.

### 2. The `volatile` Keyword
```c
#define UART_DR  (*((volatile uint32_t *)0x4000C000))
```
- **`uint32_t *`**: Treat this number as an address.
- **`*`**: Go to that address.
- **`volatile`**: Crucial! It tells the compiler: "Do NOT optimize this. The value at this address can change magically."
    - Without `volatile`, the compiler might see `while(FLAG == 1)` and think "FLAG never changes in this loop, so I'll just check it once."
    - But `FLAG` is a hardware register! It *will* change when the hardware is ready.

## 📂 File Breakdown

### `uart.c`
```c
void uart_putc(char c) {
    // 1. Wait until the Transmit FIFO is not full
    while (UART_FR & UART_FR_TXFF);
    
    // 2. Write the character to the Data Register
    UART_DR = c;
}
```
This is called **Polling**. We keep asking "Are you ready? Are you ready?" until the hardware says "Yes".

## 🚀 How it Works
1.  `main()` calls `uart_print("Hello")`.
2.  `uart_print` loops through the string.
3.  `uart_putc` waits for the hardware buffer to have space.
4.  `uart_putc` writes 'H' to `0x4000C000`.
5.  The UART hardware sees the write and shifts the bits out.
6.  QEMU captures these bits and shows 'H' on your terminal.
