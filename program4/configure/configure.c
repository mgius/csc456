#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
   unsigned int MyStack = 0;
   char Uncalled[4];
   if (argc < 3) {
      printf("Need more Arguments!\n");
      exit(-1);
   }
   /* we're currenetly doing nothing with this.  huh... */
   /* TODO: Do something with this? */
   if (1 != sscanf(argv[1], "%x", &MyStack)) {
      printf("Invalid hex number\n");
      exit(-1);
   }
   if(1 != sscanf(argv[2], "%x", (unsigned int *)Uncalled)) {
      printf("Invalid hex number\n");
      exit(-1);
   }

   printf("abcdabcdabcdabcd%c%c%c%c", Uncalled[3],
                                      Uncalled[2],
                                      Uncalled[1],
                                      Uncalled[0]);
                                                
}
