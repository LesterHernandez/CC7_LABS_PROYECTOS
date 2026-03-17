
#include "stdio.h"

int main(void) {
  while (1) {
    for(int i=0;i<10;i++){
        PRINT("%d \n",i);
    }
    for(char c = 'a'; c <= 'z'; c++){
        PRINT("%c\n", c);
    }
    
  }
}