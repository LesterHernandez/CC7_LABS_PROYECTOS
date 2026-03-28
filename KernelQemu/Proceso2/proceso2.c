#include "print.h"

void proceso2(void) {
    int n = 0;
    while(1) {
        PRINT("%d\n", n);
        n++;
        if(n > 9) n = 0;
        
        for(volatile int i = 0; i < 10000000; i++); 
    }
}