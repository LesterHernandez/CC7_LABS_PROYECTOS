@echo off

REM Ensamblar startup
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o

REM Compilar librerías (desde OS)
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding ..\OS\stdio.c -o stdio.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding ..\OS\string.c -o string.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding ..\OS\uart.c -o uart.o

REM Compilar proceso1 (incluyendo headers de OS)
arm-none-eabi-gcc -c -I ..\OS -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding proceso1.c -o proceso1.o

REM Link
arm-none-eabi-ld -T memmap.ld startup.o proceso1.o stdio.o string.o uart.o -o proceso1.elf

REM Binario
mkdir bin
arm-none-eabi-objcopy proceso1.elf -O binary bin\proceso1.bin

REM Debug
arm-none-eabi-objdump -D proceso1.elf > proceso1.list
mkdir otros
move proceso1.elf otros\
echo Proceso1 compilado correctamente
pause