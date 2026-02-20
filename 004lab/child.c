#include <windows.h>      // Libreria principal de la API de Windows (procesos, memoria, handles, etc)
#include <stdio.h>        // Entrada y salida estandar (printf)
#include <stdlib.h>       // Funciones como atoi y strtoul
#include <string.h>       // Manejo de cadenas

// Nombre que identifica la memoria compartida.
// Debe coincidir exactamente con el nombre usado en el parent.
#define SHARED_NAME "MySharedMemory"

int main(int argc, char *argv[]) {

    // Verifica que se haya pasado al menos un argumento
    // El argumento indica que task debe ejecutar el child
    if (argc < 2) {
        printf("No task specified.\n");
        return 1;
    }

    // Convierte el argumento recibido a entero
    int task = atoi(argv[1]);

    // ================= TASK 1 =================
    // Solo muestra el PID del proceso hijo
    if (task == 1) {
        printf("Child Process: PID=%lu\n",
               GetCurrentProcessId());
    }

    // ================= TASK 2 =================
    // Muestra el PID y simula trabajo con Sleep
    else if (task == 2) {
        printf("Child Process: PID=%lu\n",
               GetCurrentProcessId());

        // Pausa de 2 segundos para simular ejecucion
        Sleep(2000);
    }

    // ================= TASK 3 =================
    // Lee datos desde la entrada estandar (pipe)
    else if (task == 3) {

        char buffer[100];   // Buffer donde se almacenara el mensaje
        DWORD read;         // Variable para cantidad de bytes leidos

        // Lee desde STD_INPUT_HANDLE (redirigido por el parent)
        if (ReadFile(GetStdHandle(STD_INPUT_HANDLE),
                     buffer,
                     sizeof(buffer),
                     &read,
                     NULL)) {

            // Si la lectura es exitosa, muestra el mensaje recibido
            printf("Child Process: Received \"%s\"\n", buffer);
        }
        else {
            // Si falla la lectura, muestra el codigo de error
            printf("Child Process: ReadFile failed (%lu)\n", GetLastError());
        }
    }

    // ================= TASK 4 =================
    // Recibe numero de hijo y PID del padre como argumentos
    else if (task == 4) {

        // argv[2] contiene el numero del hijo (1, 2 o 3)
        int childNumber = atoi(argv[2]);

        // argv[3] contiene el PID del padre
        unsigned long parentPID = strtoul(argv[3], NULL, 10);

        // Imprime numero de hijo, PID propio y PID del padre
        printf("Child %d: PID=%lu, Parent PID=%lu\n",
               childNumber,
               GetCurrentProcessId(),
               parentPID);
    }

    // ================= TASK 5 =================
    // Accede a memoria compartida creada por el parent
    else if (task == 5) {

        // Abre el objeto de memoria compartida existente
        HANDLE hMapFile = OpenFileMapping(
            FILE_MAP_READ,  // Solo lectura
            FALSE,
            SHARED_NAME
        );

        // Verifica si se abrio correctamente
        if (hMapFile == NULL) {
            printf("OpenFileMapping failed (%lu)\n", GetLastError());
            return 1;
        }

        // Mapea la vista de la memoria compartida al espacio del proceso
        char* pBuf = (char*) MapViewOfFile(
            hMapFile,
            FILE_MAP_READ,  // Solo lectura
            0,
            0,
            0               // Mapea todo el objeto
        );

        // Verifica si el mapeo fue exitoso
        if (pBuf == NULL) {
            printf("MapViewOfFile failed (%lu)\n", GetLastError());
            CloseHandle(hMapFile);
            return 1;
        }

        // Imprime el contenido almacenado por el parent
        printf("Child Process: Read \"%s\"\n", pBuf);

        // Libera la vista mapeada
        UnmapViewOfFile(pBuf);

        // Cierra el handle del objeto de memoria compartida
        CloseHandle(hMapFile);
    }

    return 0;
}