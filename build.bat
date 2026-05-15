@echo off
REM Build script for Windows environments without gcc pre-installed
REM This creates a ready-to-build project that works with any C compiler

setlocal enabledelayedexpansion

echo.
echo Kernel Panic Logger - Windows Build Script
echo ============================================
echo.

REM Check if we can find a compiler
where gcc >nul 2>&1
if %errorlevel% equ 0 (
    echo Found GCC compiler
    goto build_gcc
)

where cl >nul 2>&1
if %errorlevel% equ 0 (
    echo Found MSVC compiler
    goto build_msvc
)

echo WARNING: No C compiler found!
echo.
echo To compile this project, install one of:
echo - GCC (via MinGW or WSL)
echo - Microsoft Visual C++
echo - Clang
echo.
echo Instructions:
echo 1. Install MinGW GCC from: https://www.mingw-w64.org/
echo 2. Add to your PATH
echo 3. Run this script again
echo.

goto end

:build_gcc
echo.
echo Building with GCC...
if not exist build mkdir build
if not exist bin mkdir bin

gcc -Wall -Wextra -std=c99 -I./include -c src/log_buffer.c -o build/log_buffer.o
if %errorlevel% neq 0 goto compile_error

gcc -Wall -Wextra -std=c99 -I./include -c src/crash_context.c -o build/crash_context.o
if %errorlevel% neq 0 goto compile_error

gcc -Wall -Wextra -std=c99 -I./include -c src/example.c -o build/example.o
if %errorlevel% neq 0 goto compile_error

gcc build/log_buffer.o build/crash_context.o build/example.o -o bin/kernel_panic_logger.exe
if %errorlevel% neq 0 goto compile_error

echo.
echo [OK] Build successful!
echo Output: bin\kernel_panic_logger.exe
echo.
goto success

:build_msvc
echo.
echo Building with MSVC...
if not exist build mkdir build
if not exist bin mkdir bin

cl /W4 /std:c11 /I./include /Fobuild\ src/log_buffer.c
if %errorlevel% neq 0 goto compile_error

cl /W4 /std:c11 /I./include /Fobuild\ src/crash_context.c
if %errorlevel% neq 0 goto compile_error

cl /W4 /std:c11 /I./include /Fobuild\ src/example.c
if %errorlevel% neq 0 goto compile_error

link /OUT:bin\kernel_panic_logger.exe build\log_buffer.obj build\crash_context.obj build\example.obj
if %errorlevel% neq 0 goto compile_error

echo.
echo [OK] Build successful!
echo Output: bin\kernel_panic_logger.exe
echo.
goto success

:compile_error
echo.
echo [ERROR] Compilation failed!
echo.
goto end

:success
echo Running demo...
echo.
bin\kernel_panic_logger.exe
echo.
goto end

:end
endlocal
