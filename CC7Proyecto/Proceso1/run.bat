@echo off
echo ==============================
echo   BUILDING ARM PROGRAM (OS)
echo ==============================

REM Crear carpetas si no existen
if not exist bin mkdir bin
if not exist otros mkdir otros

echo.
echo Cleaning previous builds...
del /Q otros*.o 2>nul
del /Q otros*.elf 2>nul
del /Q otros*.list 2>nul
del /Q bin*.bin 2>nul

echo.
echo 1. Assembling startup.s
arm-none-eabi-as --warn --fatal-warnings startup.s -o otros\startup.o

echo.
echo 2. Compiling proceso1.c
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 ^
-nostdlib -nostartfiles -ffreestanding proceso1.c -o otros\proceso1.o

echo.
echo 3. Linking object files
arm-none-eabi-ld -T memmap.ld otros\startup.o otros\proceso1.o -o otros\proceso1.elf

echo.
echo 4. Generating binary
arm-none-eabi-objcopy otros\proceso1.elf -O binary bin\proceso1.bin

echo.
echo 5. Disassembling ELF (creating list file)
arm-none-eabi-objdump -D otros\proceso1.elf > otros\proceso1.list

echo.
echo =================================
echo BUILD FINISHED
echo Binary: bin\proceso1.bin
echo Other files: carpeta "otros"
echo =================================

pause
