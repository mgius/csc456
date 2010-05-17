/* Mark Gius
 * Program 2: kasiski
 * CSC456
 * Kasiski attack on vigenere cycle
 */

#include <ctype.h> // isAlpha
#include <fcntl.h>
#include <math.h> // ceil
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ic.h"
using namespace std;

// 1K buffer is reasonable for all but the most restrictive embedded systems
// and if you're cracking ciphertexts on embedded systems why are you using
// my code anyway?
#define BUFSIZE 1024

void usage(void) {
   printf("usage: kasiski [ -v ] [-m length] [ infile [ outfile ] ]\n");
   exit(1);
}

int main(int argc, char **argv) {
   bool verbose = false;
   int minLength = 3;
   // Ugh, I like using read(2), but it looks like I need to bounce through
   // a stream, and you can lseek through stdin
   FILE *inFile = stdin, *outFile = stdout;

   // Cmdline parsing
   for (int i=1; i < argc; i++) {
      string arg(argv[i]);
      // Verbose does nothing, but it's here to conform with spec
      if (!arg.compare("-v")) {
         verbose = true;
      }
      else if (!arg.compare("-m")) {
         char *temp;
         minLength = strtol(argv[++i], &temp, 10);
         // Check for a bad strtol. Probably a more efficient way to do this
         if (temp != argv[i] + string(argv[i]).length()) {
            usage();
         }
         // verify valid length
         if (minLength < 1) {
            usage();
         }
      }
      // All other flags are bad
      else if (arg[0] == '-') {
         usage();
      }
      // First non-flag argument is input
      else if (inFile == stdin) {
         inFile = fopen(arg.c_str(), "r");
         if (NULL == inFile) {
            perror("input file");
            usage();
         }
      }
      // second non-flag argument is output
      else if (outFile == stdout) {
         outFile = fopen(arg.c_str(), "w");
         if (NULL == outFile) {
            perror("output file");
            usage();
         }
      }
      // third non-flag argument is error
      else {
         printf("Invalid arguments\n");
         usage();
      }
   }

   if (inFile != stdin) {
      fclose(inFile);
      if (outFile != stdout) {
         fclose(outFile);
      }
   }
}
