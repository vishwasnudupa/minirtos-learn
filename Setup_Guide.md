# 🛠️ Setup Guide: Getting Ready for Embedded Dev

It looks like your computer is missing the necessary tools. Since you want to use **Linux** and **QEMU**, we need to set up the **Windows Subsystem for Linux (WSL)**.

## Step 1: Install WSL (Windows Subsystem for Linux)
1.  Open **PowerShell** as Administrator (Right-click Start -> Terminal (Admin) or PowerShell (Admin)).
2.  Run this command:
    ```powershell
    wsl --install
    ```
3.  **Restart your computer**.
4.  After restart, a window should pop up to finish installing Ubuntu. Create a username and password.

## Step 2: Install the Toolchain (Inside WSL)
Once you have your Ubuntu terminal open (it looks like a command prompt but says "Ubuntu"), run these commands to get the compiler and emulator:

```bash
# Update the package list
sudo apt update

# Install the ARM compiler, QEMU, Make, and GDB
sudo apt install gcc-arm-none-eabi qemu-system-arm make gdb-multiarch
```

## Step 3: Verify Installation
Run these commands in your **Ubuntu terminal** to check if it worked:

```bash
arm-none-eabi-gcc --version
qemu-system-arm --version
```

If you see version numbers, you are ready!

## Alternative: Windows Native (If you can't use WSL)
If you cannot use WSL, you must install these tools for Windows manually:
1.  **ARM GCC**: Download from [Arm Developer](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
2.  **QEMU**: Download from [qemu.org](https://www.qemu.org/download/#windows).
3.  **Make**: Install via `choco install make` or download a binary.
4.  **Add to PATH**: You must add the `bin` folders of these tools to your Windows System PATH.
