#include <stdio.h> // printf et al
#include <stdlib.h> // exit, strtol
#include <string.h> // strlen
#include "ic.h"

void icUsage() {
   printf("usage: ic N # [ # [...] ]\n");
   exit(0);
}

// Parses a number out of a string, catching errors
int parseInt(char *arg) {
   char *strtolPtr = NULL;

   int result = strtol(arg, &strtolPtr, 10);
   if (strtolPtr != arg + strlen(arg)) {
      icUsage();
   }
   return result;
}

int main( int argc, char **argv) {
   int mesgLen = 0;
   if (argc < 3) {
      // need to be at least ic N #
      icUsage();
   }

   mesgLen = parseInt(argv[1]);

   printf("Key  Expected IC (N=%d)\n", mesgLen);
   printf("---- ------------------\n");
   for (int i = 2; i < argc; i++) {
      int keyLength = parseInt(argv[i]);
      printf("%4d %05.4f\n", keyLength, expectedIC(mesgLen, keyLength));
   }
}
