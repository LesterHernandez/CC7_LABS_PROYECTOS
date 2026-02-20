#include <windows.h>
#include <stdio.h>
#include <string.h>

// Nombre de la memoria compartida para el Task 5
#define SHARED_NAME "MySharedMemory"

// Funcion que ejecuta cada task segun el numero recibido
void run_task(int task) {

    // Estructuras necesarias para CreateProcess
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Inicializar estructuras en cero
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Variables para el pipe del Task 3
    HANDLE hRead = NULL, hWrite = NULL;
    SECURITY_ATTRIBUTES sa;

    // Buffer para construir el comando del child
    char command[100];

    // ================= TASK 5 =================
    // Comunicacion usando memoria compartida
    if (task == 5) {

        // Crear una region de memoria compartida en RAM
        HANDLE hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,   // No se usa archivo fisico
            NULL,                   // Seguridad por defecto
            PAGE_READWRITE,         // Permiso lectura y escritura
            0,
            1024,                   // Tamaño 1024 bytes
            SHARED_NAME             // Nombre de la memoria compartida
        );

        // Verificar si la memoria fue creada correctamente
        if (hMapFile == NULL) {
            printf("CreateFileMapping failed (%lu)\n", GetLastError());
            return;
        }

        // Mapear la memoria compartida al espacio del proceso
        char* pBuf = (char*) MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            1024
        );

        // Mensaje que se escribira en memoria compartida
        const char* message = "Shared Memory Example";

        // Copiar mensaje a la memoria compartida
        strcpy(pBuf, message);

        printf("Parent Process: Writing \"%s\"\n", message);

        // Crear el child con el Task 5
        sprintf(command, "child.exe 5");

        CreateProcess(NULL, command,
                      NULL, NULL, FALSE, 0,
                      NULL, NULL, &si, &pi);

        // Esperar que el child termine
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Liberar memoria compartida
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return;
    }

    // ================= TASK 4 =================
    // Crear multiples procesos hijos
    if (task == 4) {

        printf("Parent Process: PID=%lu\n",
               GetCurrentProcessId());

        // Crear tres procesos hijos usando un ciclo
        for (int i = 1; i <= 3; i++) {

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            // Pasar numero del hijo y PID del padre como argumentos
            sprintf(command, "child.exe 4 %d %lu",
                    i,
                    GetCurrentProcessId());

            CreateProcess(NULL, command,
                          NULL, NULL, FALSE, 0,
                          NULL, NULL, &si, &pi);

            // Esperar que cada hijo termine antes de crear el siguiente
            WaitForSingleObject(pi.hProcess, INFINITE);

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        return;
    }

    // ================= TASK 1–3 =================

    // Construir comando para ejecutar el child
    sprintf(command, "child.exe %d", task);

    // ================= TASK 3 =================
    // Comunicacion usando pipe
    if (task == 3) {

        // Configurar atributos de seguridad para heredar handles
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        // Crear pipe anonimo
        CreatePipe(&hRead, &hWrite, &sa, 0);

        // Evitar que el hijo herede el extremo de escritura
        SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0);

        // Indicar que se usaran handles personalizados
        si.dwFlags = STARTF_USESTDHANDLES;

        // Redirigir la entrada estandar del hijo al pipe
        si.hStdInput  = hRead;
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    }

    // Crear el proceso hijo
    CreateProcess(NULL, command,
                  NULL, NULL, TRUE, 0,
                  NULL, NULL, &si, &pi);

    // ================= TASK 1 =================
    // Solo crear proceso y mostrar PID del padre
    if (task == 1) {
        printf("Parent Process: PID=%lu\n",
               GetCurrentProcessId());
    }

    // ================= TASK 2 =================
    // Esperar que el hijo termine
    else if (task == 2) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        printf("Parent Process: Child has finished execution.\n");
    }

    // ================= TASK 3 =================
    // Escribir mensaje en el pipe
    else if (task == 3) {

        CloseHandle(hRead);

        const char* msg = "Hello from Parent";
        DWORD written;

        WriteFile(hWrite, msg,
                  (DWORD)strlen(msg) + 1,
                  &written, NULL);

        printf("Parent Process: Writing \"%s\"\n", msg);

        CloseHandle(hWrite);
    }

    // Esperar que el hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Liberar handles del proceso hijo
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// Funcion principal
int main() {

    printf("\n=========== TASK 1 ===========\n");
    run_task(1);

    printf("\n=========== TASK 2 ===========\n");
    run_task(2);

    printf("\n=========== TASK 3 ===========\n");
    run_task(3);

    printf("\n=========== TASK 4 ===========\n");
    run_task(4);

    printf("\n=========== TASK 5 ===========\n");
    run_task(5);

    return 0;
}