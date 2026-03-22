#!/bin/bash

echo "==================================================="
echo "   Iniciando compilación unificada del Proyecto"
echo "==================================================="

# 1. Compilar los procesos a código objeto (.o) sin enlazarlos
echo "-> Compilando Proceso 1..."
arm-none-eabi-gcc -c Proceso1/proceso1.c -o Proceso1/proceso1.o -I OS/

echo "-> Compilando Proceso 2..."
arm-none-eabi-gcc -c Proceso2/proceso2.c -o Proceso2/proceso2.o -I OS/

# 2. Compilar el OS y unir todo
echo "-> Compilando Sistema Operativo y unificando..."
cd OS

# Ensamblar startup
arm-none-eabi-as startup.s -o startup.o

# Compilar código fuente del OS
arm-none-eabi-gcc -c os.c -o os.o
arm-none-eabi-gcc -c stdio.c -o stdio.o
arm-none-eabi-gcc -c string.c -o string.o
arm-none-eabi-gcc -c uart.c -o uart.o

# --- LO NUEVO EMPIEZA AQUÍ ---

# Buscar la librería de GCC para las matemáticas con decimales
LIBGCC=$(arm-none-eabi-gcc -print-libgcc-file-name)

# ENLAZAR TODO JUNTO agregando $LIBGCC al final del comando
arm-none-eabi-ld -T memmap.ld startup.o os.o stdio.o string.o uart.o ../Proceso1/proceso1.o ../Proceso2/proceso2.o $LIBGCC -o os.elf

# --- LO NUEVO TERMINA AQUÍ ---

# Convertir a binario puro
arm-none-eabi-objcopy -O binary os.elf os.bin

cd ..
echo ""
echo "==================================================="
echo " ¡Compilación exitosa! Archivo generado: OS/os.bin "
echo "==================================================="