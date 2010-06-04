#define GetSP(sp)  asm("movl  %%esp,%0": "=r" (sp) : )
#define SetSP(sp)  asm("movl  %0,%%esp":           : "r" (sp)  )
#define STACKSIZE 1024 * 1024
#include <stdio.h>
#include <stdlib.h>

void uncalled(void) {
   printf("Uncalled was called. Strange that.\n");
   exit(-1);
}

int realMain(void) {
   char buf[4];
   gets(buf);
   //puts(buf);
   return 0;
}

int oldSp = -1;
int main(void) {
   char *ourStack = malloc(STACKSIZE);
   sleep(10);
   GetSP(oldSp);
   SetSP(ourStack + STACKSIZE);
   realMain();
   SetSP(oldSp);
   return 0;
}

