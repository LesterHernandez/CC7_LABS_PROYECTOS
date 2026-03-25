#!/bin/bash

echo "==================================================="
echo "   Iniciando compilación para QEMU (vexpress-a9)"
echo "==================================================="

echo "-> Ensamblando startup.s..."
arm-none-eabi-as -mcpu=cortex-a9 startup.s -o startup.o

echo "-> Compilando todos los archivos C..."
for file in *.c; do
    arm-none-eabi-gcc -mcpu=cortex-a9 -c "$file" -O0 -ffreestanding
done

# Buscar la librería de GCC
LIBGCC=$(arm-none-eabi-gcc -print-libgcc-file-name)

echo "-> Enlazando todo en os.elf..."
# CORRECCIÓN: Usamos solo *.o para evitar duplicar startup.o
arm-none-eabi-ld -T memmap.ld *.o $LIBGCC -o os.elf

# Validar si el linker falló
if [ $? -ne 0 ]; then
    echo ""
    echo " Error al enlazar los archivos. Revisa los mensajes arriba."
    exit 1
fi

echo ""
echo "==================================================="
echo " ¡Compilación exitosa! Archivo generado: os.elf "
echo "==================================================="

echo "-> Iniciando QEMU..."
echo "-> (Para salir presiona: Ctrl+A, suelta, y luego presiona X)"
echo "==================================================="

qemu-system-arm -M vexpress-a9 -m 128M -kernel os.elf -nographic