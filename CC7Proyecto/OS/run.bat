@echo off

REM Ensamblar startup.s
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o

REM Compilar fuentes C
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding os.c -o os.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding stdio.c -o stdio.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding string.c -o string.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding uart.c -o uart.o

REM Enlazar con el script memmap
arm-none-eabi-ld -T memmap.ld startup.o os.o stdio.o string.o uart.o -o os.elf

REM Generar archivo binario en carpeta bin
mkdir bin
arm-none-eabi-objcopy os.elf -O binary bin\os.bin

REM Generar listado y mover archivos a carpeta otros
arm-none-eabi-objdump -D os.elf > os.list
mkdir otros
move os.elf otros\
move *.o otros\
move os.list otros\

echo Compilacion y organizacion completadas.
pause
