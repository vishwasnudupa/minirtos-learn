# Makefile for Bare Metal ARM

# Toolchain
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Architecture
CFLAGS = -mcpu=cortex-m3 -mthumb -g -Wall
LDFLAGS = -T linker.ld -nostdlib

# Files
SRCS = main.c startup.s uart.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
ELF = firmware.elf
BIN = firmware.bin

all: $(ELF)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.bin

# Run in QEMU (LM3S6965EVB)
run: $(ELF)
	qemu-system-arm -M lm3s6965evb -kernel $(ELF) -nographic

# Run QEMU with GDB server (port 1234)
debug: $(ELF)
	qemu-system-arm -M lm3s6965evb -kernel $(ELF) -nographic -s -S
