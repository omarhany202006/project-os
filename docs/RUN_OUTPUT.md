# WSL Ubuntu Build Output

Command:

```bash
make run
```

Result:

```text
gcc -Wall -Wextra -Werror -std=c99 -I./include -c src/crash_context.c -o build/crash_context.o
gcc -Wall -Wextra -Werror -std=c99 -I./include -c src/example.c -o build/example.o
gcc -Wall -Wextra -Werror -std=c99 -I./include build/log_buffer.o build/crash_context.o build/example.o -o bin/kernel_panic_logger
[OK] Build successful: bin/kernel_panic_logger
Running kernel panic logger demo...
+================================================================+
|         KERNEL PANIC LOGGER - DEMONSTRATION PROGRAM            |
|                                                                |
|  Log Buffers & Crash Context Info for Panic Handling           |
|  Portable C99 implementation for Ubuntu/WSL                    |
+================================================================+

=== EXCEPTION NAMES REFERENCE ===

[ 0] Division by Zero
[ 1] Debug
[ 2] Non-Maskable Interrupt
[ 3] Breakpoint
[ 4] Overflow
[ 5] Bound Range Exceeded
[ 6] Invalid Opcode
[ 7] Device Not Available
[ 8] Double Fault
[ 9] Coprocessor Segment Overrun
[10] Invalid TSS
[11] Segment Not Present
[12] Stack-Segment Fault
[13] General Protection Fault
[14] Page Fault
[15] Reserved
[16] x87 Floating-Point Exception
[17] Alignment Check
[18] Machine Check
[19] SIMD Floating-Point Exception
[20] Virtualization Exception

=== LOG BUFFER DEMONSTRATION ===

========== KERNEL LOG BUFFER DUMP ==========
Total entries: 7
Overflow count: 0
==========================================

[1373] [CPU 0] [INFO] Kernel initialized
[1375] [CPU 0] [DEBUG] Loading device drivers
[1375] [CPU 1] [INFO] CPU 1 online
[1376] [CPU 0] [WARN] Low memory condition detected
[1377] [CPU 0] [ERROR] Failed to mount root filesystem
[1377] [CPU 0] [DEBUG] Attempting recovery...
[1378] [CPU 0] [INFO] System recovered

========= END OF LOG DUMP (7 entries) =========

Total entries written: 7
Buffer overflows: 0

=== CRASH CONTEXT DEMONSTRATION ===

+================================================================+
|                    KERNEL PANIC DETECTED                       |
+================================================================+

Exception: [14] Page Fault
Message: Attempted to access invalid memory address 0xdeadbeef
Timestamp: 1 ms
CPU ID: 0
In Interrupt: Yes

--- CPU REGISTERS ---
RAX: 0x0000000000000000  RBX: 0x0000000000000000
RCX: 0x0000000000000000  RDX: 0x0000000000000000
RSI: 0x0000000000000000  RDI: 0x0000000000000000
RBP: 0x0000000000000000  RSP: 0x00000000cafebabe
R8:  0x0000000000000000  R9:  0x0000000000000000
R10: 0x0000000000000000  R11: 0x0000000000000000
R12: 0x0000000000000000  R13: 0x0000000000000000
R14: 0x0000000000000000  R15: 0x0000000000000000
RIP: 0x00000000deadbeef  RFLAGS: 0x0000000000000000

--- CONTROL REGISTERS ---
CR0: 0x0000000000000000  CR2: 0x0000000000000000
CR3: 0x0000000000000000  CR4: 0x0000000000000000

--- MEMORY INFORMATION ---
Fault Address: 0x0000000000000000
Kernel Stack: 0xffff800000010000 - 0xffff800000020000 (Size: 65536 bytes)

--- STACK TRACE ---
[0] kernel_function_0 (0xffff800000000000) -> 0xffff800000001000
[1] kernel_function_1 (0xffff800000000100) -> 0xffff800000001100
[2] kernel_function_2 (0xffff800000000200) -> 0xffff800000001200
[3] kernel_function_3 (0xffff800000000300) -> 0xffff800000001300
[4] kernel_function_4 (0xffff800000000400) -> 0xffff800000001400

--- PROCESS INFORMATION ---
Process: kernel (PID: 0, TID: 0)

--- DUMP OUTPUT ---
KERNEL PANIC DUMP
Exception: [14] Page Fault
Message: Attempted to access invalid memory address 0xdeadbeef
Timestamp: 1 ms
CPU ID: 0
Instruction Pointer: 0x00000000deadbeef
Stack Pointer: 0x00000000cafebabe
Kernel Stack: 0xffff800000010000 - 0xffff800000020000

--- END DUMP (262 bytes) ---

[OK] All demonstrations completed successfully!
Ready for deployment to Ubuntu/xv6 environment.
```
