#!/bin/bash

# Ensamblar startup.s
arm-none-eabi-as --warn --fatal-warnings startup.s -o startup.o

# Compilar fuentes C
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding os.c -o os.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding stdio.c -o stdio.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding string.c -o string.o
arm-none-eabi-gcc -c -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=hard -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding uart.c -o uart.o

# Enlazar con el script memmap
arm-none-eabi-ld -T memmap.ld startup.o os.o stdio.o string.o uart.o ../Proceso1/proceso1.o ../Proceso2/proceso2.o -o os.elf

# Generar archivo binario en carpeta bin
mkdir -p bin
arm-none-eabi-objcopy os.elf -O binary bin/os.bin

# Generar listado y mover archivos a carpeta otros
arm-none-eabi-objdump -D os.elf > os.list
mkdir -p otros
mv os.elf otros/
mv *.o otros/
mv os.list otros/

echo "Compilacion de OS y organizacion completadas."