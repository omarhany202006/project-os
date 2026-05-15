# Deployment Guide: Windows to Ubuntu/xv6

This guide explains how to push your Kernel Panic Logger project from Windows to GitHub, and then pull or clone it in Ubuntu/xv6.

## Part 1: Push to GitHub (Windows)

### 1.1 Create a New Repository on GitHub

1. Go to [GitHub](https://github.com/new)
2. Sign in to your account
3. Repository name: `kernel-panic-logger`
4. Description: "Kernel panic logging with log buffers and crash context information"
5. Choose visibility (Public or Private)
6. **Do NOT** initialize with README (we already have one)
7. Click "Create repository"

### 1.2 Add Remote and Push

```powershell
# Navigate to your project
cd "b:\project os"

# Add GitHub as remote origin
git remote add origin https://github.com/YOUR_USERNAME/kernel-panic-logger.git

# Verify remote
git remote -v

# Push to GitHub (main branch)
git branch -M main
git push -u origin main
```

**Note:** Replace `YOUR_USERNAME` with your actual GitHub username.

### 1.3 Verify Push

Visit `https://github.com/YOUR_USERNAME/kernel-panic-logger` to verify your code is on GitHub.

## Part 2: Clone in Ubuntu

### 2.1 Set Up Ubuntu Terminal

If using WSL (Windows Subsystem for Linux) on Windows:
```powershell
# PowerShell on Windows
wsl

# Now you're in Ubuntu terminal
```

If using a separate Ubuntu machine, open Terminal.

### 2.2 Clone the Repository

```bash
# Navigate to where you want the project
cd ~

# Clone your repository
git clone https://github.com/YOUR_USERNAME/kernel-panic-logger.git

# Navigate into the project
cd kernel-panic-logger

# Verify files
ls -la
```

### 2.3 Build on Ubuntu

```bash
# Make the build script executable
chmod +x build.sh

# Build the project
./build.sh

# Run the demonstration
./bin/kernel_panic_logger
```

## Part 3: Integration with xv6 (Ubuntu)

### 3.1 Install xv6 Build Requirements

```bash
# Update package manager
sudo apt-get update

# Install required tools for RISC-V xv6
sudo apt-get install -y build-essential git qemu-system-riscv gcc-riscv64-linux-gnu gdb-multiarch

# Verify the RISC-V toolchain
qemu-system-riscv64 --version
riscv64-linux-gnu-gcc --version

# Clone the RISC-V xv6 labs repository
cd ~
git clone git://g.csail.mit.edu/xv6-labs-2025
cd xv6-labs-2025
```

### 3.2 Integrate Kernel Panic Logger with xv6

```bash
# Copy the kernel panic logger to xv6
cp -r ~/kernel-panic-logger/include ~/xv6-labs-2025/
cp -r ~/kernel-panic-logger/src ~/xv6-labs-2025/kernel-panic-logger/

# Or link the directories
ln -s ~/kernel-panic-logger ~/xv6-labs-2025/panic-logger
```

### 3.3 Modify xv6 Build System

Edit `xv6-labs-2025/Makefile` to include panic logger compilation:

```makefile
# In the OBJS section, add:
_panic_logger_o = \
	kernel-panic-logger/log_buffer.o \
	kernel-panic-logger/crash_context.o

OBJS = ... $(_panic_logger_o) ...

# Add compilation rules:
kernel-panic-logger/%.o: kernel-panic-logger/%.c
	$(CC) -c $< -o $@
```

### 3.4 Update xv6 trap handler

In `xv6-labs-2025/trap.c`, integrate crash context capture:

```c
#include "kernel-panic-logger/log_buffer.h"
#include "kernel-panic-logger/crash_context.h"

void trap(struct trapframe *tf) {
    // ... existing code ...
    
    // On fatal exception
    if (fatal_exception) {
        log_buffer_t *logs = log_buffer_get_global();
        crash_context_t ctx = crash_context_panic(
            tf->trapno,
            crash_context_get_exception_name(tf->trapno),
            "Kernel panic in trap handler",
            logs
        );
        crash_context_print(&ctx);
        log_buffer_dump(logs);
        // Halt the system
        panic("System halted due to exception");
    }
}
```

### 3.5 Build xv6 with Kernel Panic Logger

```bash
cd ~/xv6-labs-2025
make clean
make
make qemu
```

### 3.6 Run xv6 and Test Panic Logging

In xv6:
```bash
# Trigger a panic from user space (if tests are available)
# The panic logs should be displayed with full crash context
```

## Part 4: Maintaining Your Repository

### 4.1 Pull Latest Changes from GitHub

```bash
# In Ubuntu
cd ~/kernel-panic-logger
git pull origin main
```

### 4.2 Push New Changes

```bash
# Make changes, then commit
git add .
git commit -m "Description of changes"
git push origin main
```

### 4.3 Sync Between Windows and Ubuntu

```bash
# On Windows (PowerShell)
cd "b:\project os"
git pull origin main

# On Ubuntu (Bash)
cd ~/kernel-panic-logger
git pull origin main
```

## Troubleshooting

### Git Authentication Issues

**Problem:** "Permission denied" when pushing

**Solution:**
```bash
# Use SSH instead of HTTPS
git remote set-url origin git@github.com:YOUR_USERNAME/kernel-panic-logger.git

# Or use personal access token (GitHub)
git remote set-url origin https://<token>@github.com/YOUR_USERNAME/kernel-panic-logger.git
```

### Line Ending Issues on Ubuntu

```bash
# After cloning
git config core.safecrlf false

# Or configure globally
git config --global core.safecrlf false
```

### Build Issues on Ubuntu

```bash
# If build fails, ensure 32-bit support (for some systems)
sudo apt-get install -y gcc-multilib

# Rebuild
./build.sh
```

### xv6 Integration Issues

```bash
# Clean xv6 build
cd ~/xv6-labs-2025
make clean
make veryclean
make

# Check for compilation errors related to panic logger
# Adjust #include paths in xv6 trap.c if needed
```

## Quick Reference: Common Commands

```bash
# Clone the project
git clone https://github.com/YOUR_USERNAME/kernel-panic-logger.git
cd kernel-panic-logger

# Build
./build.sh

# Run demo
./bin/kernel_panic_logger

# View git log
git log --oneline

# Push changes
git add .
git commit -m "Your message"
git push origin main

# Pull latest
git pull origin main
```

## Next Steps

1. ✓ Push to GitHub (complete on Windows)
2. ✓ Clone on Ubuntu (complete above)
3. → Integrate with xv6 kernel
4. → Test panic handling with xv6
5. → Create additional test cases
6. → Document xv6 integration results
7. → Create release on GitHub

---

**Note:** For any issues during the process, refer to:
- [Git Documentation](https://git-scm.com/doc)
- [GitHub Docs](https://docs.github.com)
- [xv6 Labs 2025 Documentation](https://github.com/mit-pdos/xv6-labs-2025)
