/* Mark Gius
 * Program 2: kasiski
 * CSC456
 * Kasiski attack on vigenere cycle
 */

#include <ctype.h> // isAlpha
#include <stdio.h> // printf, fread, fwrite
#include <string>
#include <string.h>
#include <stdlib.h> // malloc
#include <unistd.h>

#include "ic.h"
using namespace std;

void usage(void) {
   printf("usage: kasiski [ -v ] [ -m length] [ infile [ outfile ] ]\n");
   exit(1);
}


#define BUFSIZE 1024
/* reads and normalizes input 
 * returns count of valid characters in sanitized 
 *
 * sanitized pointer will be overwritten 
 * */
int readInput(FILE *inFile, char **sanitized) {
   char *buf = (char *) malloc(BUFSIZE);
   *sanitized = (char *) calloc(1, BUFSIZE * 4);
   int validChars = 0, bytesRead = 0, saniLen = BUFSIZE * 4;

   while (0 != (bytesRead = fread(buf, BUFSIZE, 1, inFile))) {
      for (int i = 0; i < bytesRead; i++) {
         if (isalpha(buf[i])) {
         }
      }
   }
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
