#include <stdio.h> // printf et al
#include <stdlib.h> // exit, strtol
#include <string.h> // strlen

void usage() {
   printf("usage: ic N # [ # [...] ]\n");
   exit(0);
}

/* Computes the actual index of coincidence, given the cipherlength and 
 * counts of each letter in the ciphertext
 *
 * No error checking is done on charCount.  It must be an array of at least
 * size 26 or undefined behavior will occur
 */
double actualIC(double cipherLength, int *charCount) {
   int sum = 0;
   for (int i = 0; i < 26; i++) {
      sum += charCount[i] * (charCount[i] - 1);
   }
   return 1.0 / (cipherLength * (cipherLength - 1.0)) * sum;
}

/* Computes expected index of coincidence.  
 * invalid key or cipherlengths result in negative return
 */
double expectedIC(double cipherLength, double keyLength) {
   // I'm calling cipherlength < keylength an invalid input b/c if your 
   // key is longer than the ciphertext, you're only really using part 
   // of the key
   if (cipherLength < 1 || keyLength < 1 || cipherLength < keyLength) {
      return -1.0;
   }

   // "left" and "right" side of the equation as presented in asgn2
   double left =  ( 1 / keyLength) * 
                  ( cipherLength - keyLength) / ( cipherLength - 1) *
                  0.066;
   double right = ( (keyLength - 1) / (keyLength)) * 
                  cipherLength / (cipherLength - 1) *
                  0.038;
   return left + right;
}

// Parses a number out of a string, catching errors
int parseInt(char *arg) {
   char *strtolPtr = NULL;

   int result = strtol(arg, &strtolPtr, 10);
   if (strtolPtr != arg + strlen(arg)) {
      usage();
   }
   return result;
}

int main( int argc, char **argv) {
   int mesgLen = 0;
   if (argc < 3) {
      // need to be at least ic N #
      usage();
   }

   mesgLen = parseInt(argv[1]);

   printf("Key  Expected IC (N=%d)\n", mesgLen);
   printf("---- ------------------\n");
   for (int i = 2; i < argc; i++) {
      int keyLength = parseInt(argv[i]);
      printf("%4d %05.4f\n", keyLength, expectedIC(mesgLen, keyLength));
   }
}
