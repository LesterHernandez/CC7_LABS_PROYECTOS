@echo off
REM Compilar los programas
cl /Feparent.exe parent.c
cl /Fechild.exe child.c

echo.
echo ================================
echo   Ejecutar ejemplos de tareas
echo ================================
echo.

echo Task 1: Crear proceso simple
parent.exe task1
echo.

echo Task 2: Sincronizar padre e hijo
parent.exe task2
echo.

echo Task 3: Comunicación con pipe
parent.exe task3
echo.

echo Task 4: Múltiples hijos
parent.exe task4
echo.

echo Task 5: Memoria compartida
parent.exe task5
echo.

pause
