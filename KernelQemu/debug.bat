@echo off

echo ============================================
echo        COMPILANDO Y EJECUTANDO QEMU
echo ============================================

REM ====== CREATE FOLDERS ======
if not exist bin mkdir bin
if not exist Otros mkdir Otros

REM ====== INCLUDE PATHS ======
SET INCLUDES=-I OS -I OS\irq -I OS\kernel -I OS\Librerias -I OS\uart

REM ====== BUILD PROCESSES ======
arm-none-eabi-gcc -c Proceso1\proceso1.c -o Otros\proceso1.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c Proceso2\proceso2.c -o Otros\proceso2.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== BUILD STARTUP ======
arm-none-eabi-as OS\startup\startup.s -o Otros\startup.o
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== BUILD IRQ ======
arm-none-eabi-gcc -c OS\irq\irq.c -o Otros\irq.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== BUILD KERNEL ======
arm-none-eabi-gcc -c OS\kernel\kernel.c -o Otros\kernel.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\kernel\process.c -o Otros\process.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\kernel\scheduler.c -o Otros\scheduler.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== LIBRARIES ======
arm-none-eabi-gcc -c OS\Librerias\print.c -o Otros\print.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\Librerias\read.c -o Otros\read.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\Librerias\string.c -o Otros\string.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== UART ======
arm-none-eabi-gcc -c OS\uart\uart_hw.c -o Otros\uart_hw.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\uart\uart_in.c -o Otros\uart_in.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\uart\uart_out.c -o Otros\uart_out.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\uart\uart_conv.c -o Otros\uart_conv.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== LIBGCC ======
FOR /F "delims=" %%i IN ('arm-none-eabi-gcc -print-libgcc-file-name') DO SET LIBGCC=%%i

REM ====== LINK ======
arm-none-eabi-ld -T OS\memmap.ld ^
Otros\startup.o Otros\irq.o Otros\kernel.o Otros\process.o Otros\scheduler.o ^
Otros\print.o Otros\read.o Otros\string.o ^
Otros\uart_hw.o Otros\uart_in.o Otros\uart_out.o Otros\uart_conv.o ^
Otros\proceso1.o Otros\proceso2.o "%LIBGCC%" -o Otros\os.elf

IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== GENERATE BIN ======
arm-none-eabi-objcopy -O binary Otros\os.elf bin\os.bin
IF %ERRORLEVEL% NEQ 0 GOTO error

echo.
echo BUILD SUCCESSFUL
echo.

REM ====== RUN QEMU ======
echo Ejecutando QEMU...

start "QEMU OS" cmd /k qemu-system-arm ^
-M vexpress-a9 ^
-kernel bin\os.bin ^
-nographic ^
-monitor none ^
-serial stdio

exit /b

:error
echo BUILD FAILED
pause
exit /b