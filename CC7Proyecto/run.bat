@echo off

REM ====== CREATE FOLDERS ======
if not exist otros mkdir otros
if not exist bin mkdir bin

REM ====== INCLUDE PATHS ======
SET INCLUDES=-I OS -I OS\irq -I OS\kernel -I OS\Librerias -I OS\uart -I OS\uart\uart_conv -I OS\uart\uart_hw -I OS\uart\uart_out -I OS\uart\uart_in -I OS\pcb -I OS\scheduler -I OS\process 

REM ====== BUILD PROCESSES ======
arm-none-eabi-gcc -c Proceso1\proceso1.c -o otros\proceso1.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c Proceso2\proceso2.c -o otros\proceso2.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error


REM ====== BUILD STARTUP ======
arm-none-eabi-as OS\startup\stack.s -o otros\stack.o
IF %ERRORLEVEL% NEQ 0 GOTO error
arm-none-eabi-as OS\startup\vector.s -o otros\vector.o
IF %ERRORLEVEL% NEQ 0 GOTO error
arm-none-eabi-as OS\startup\reset.s -o otros\reset.o
IF %ERRORLEVEL% NEQ 0 GOTO error
arm-none-eabi-as OS\startup\exceptions.s -o otros\exceptions.o
IF %ERRORLEVEL% NEQ 0 GOTO error
arm-none-eabi-as OS\process\process_start.s -o otros\process_start.o
IF %ERRORLEVEL% NEQ 0 GOTO error
arm-none-eabi-as OS\irq\irq_h.s -o otros\irq_h.o
IF %ERRORLEVEL% NEQ 0 GOTO error
arm-none-eabi-as OS\irq\low_level.s -o otros\low_level.o
IF %ERRORLEVEL% NEQ 0 GOTO error

REM ====== BUILD IRQ ======
arm-none-eabi-gcc -c OS\irq\irq.c -o otros\irq.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error


REM ====== BUILD KERNEL ======
arm-none-eabi-gcc -c OS\kernel\kernel.c -o otros\kernel.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\process\process.c -o otros\process.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\scheduler\scheduler.c -o otros\scheduler.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error


REM ====== BUILD LIBRARIES ======
arm-none-eabi-gcc -c OS\Librerias\print.c -o otros\print.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\Librerias\read.c -o otros\read.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\Librerias\string.c -o otros\string.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error


REM ====== BUILD UART ======
arm-none-eabi-gcc -c OS\uart\uart_hw\uart_hw.c -o otros\uart_hw.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\uart\uart_in\uart_in.c -o otros\uart_in.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\uart\uart_out\uart_out.c -o otros\uart_out.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error

arm-none-eabi-gcc -c OS\uart\uart_conv\uart_conv.c -o otros\uart_conv.o %INCLUDES%
IF %ERRORLEVEL% NEQ 0 GOTO error


REM ====== GET LIBGCC ======
FOR /F "delims=" %%i IN ('arm-none-eabi-gcc -print-libgcc-file-name') DO SET LIBGCC=%%i


REM ====== LINK ======
arm-none-eabi-ld -T OS\memmap.ld otros\irq_h.o otros\low_level.o otros\process_start.o otros\exceptions.o otros\reset.o otros\stack.o otros\vector.o otros\irq.o otros\kernel.o otros\process.o otros\scheduler.o otros\print.o otros\read.o otros\string.o otros\uart_hw.o otros\uart_in.o otros\uart_out.o otros\uart_conv.o otros\proceso1.o otros\proceso2.o "%LIBGCC%" -o otros\os.elf

IF %ERRORLEVEL% NEQ 0 GOTO error


REM ====== GENERATE BINARY ======
arm-none-eabi-objcopy -O binary otros\os.elf bin\os.bin
IF %ERRORLEVEL% NEQ 0 GOTO error

echo Build successful
exit /b


:error
echo Build failed
pause
exit /b