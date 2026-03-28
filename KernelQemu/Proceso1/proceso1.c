#include "print.h" // Asegúrate de incluir tus headers del OS

void proceso1(void) {
    char c = 'a';
    while(1) {
        PRINT("%c\n", c); // Usando tu función PRINT de stdio.c
        c++;
        if(c > 'z') c = 'a';
        
        // Un pequeño retardo manual (delay) puede ser útil aquí 
        // para no saturar la UART antes de que el timer actúe.
        for(volatile int i = 0; i < 10000000; i++); 
    }
}