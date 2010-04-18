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

using namespace std;

#define BUFSIZE 1024

void usage(void) {
   printf("usage: ftable [ -v ] [ -s num ] [-p num ] [ infile [ outfile ] ]\n");
   exit(1);
}

int main(int argc, char **argv) {
   bool verbose = false;
   int skip = 0;
   int period = 1;
   int inFd = STDIN_FILENO, outFd = STDOUT_FILENO;

   // Cmdline parsing
   for (int i=1; i < argc; i++) {
      string arg(argv[i]);
      printf("%s\n", arg.c_str());
      if (!arg.compare("-v")) {
         verbose = true;
      }
      else if (!arg.compare("-s")) {
         char *temp;
         skip = strtol(argv[++i], &temp, 10);
         // Check for a bad strtol. Probably a more efficient way to do this
         if (temp != argv[i] + string(argv[i]).length()) {
            usage();
         }
         // verify valid skip
         if (skip < 0) {
            usage();
         }
      }
      else if (!arg.compare("-p")) {
         char *temp;
         period = strtol(argv[++i], &temp, 10); 
         // Check for a bad strtol. Probably a more efficient way to do this
         if (temp != argv[i] + string(argv[i]).length()) {
            usage();
         }
         // verify that period is 1 or greater
         if (period < 1) {
            usage();
         }
      }
      else if (arg[0] == '-') {
         usage();
      }
      else if (inFd == STDIN_FILENO) {
         printf("infile: %s\n", arg.c_str());
         inFd = open(arg.c_str(), O_RDONLY);
         if (-1 == inFd) {
            perror("input file");
            usage();
         }
      }
      else if (outFd == STDOUT_FILENO) {
         outFd = open(arg.c_str(), 
                      O_WRONLY | O_TRUNC | O_CREAT, 
                      S_IRUSR | S_IWUSR );
         if (-1 == outFd) {
            perror("output file");
            usage();
         }
      }
      else {
         printf("Invalid arguments\n");
         usage();
      }
   }

   // main loop
   int validChars = 0;
   int curPeriod = 1;
   int table[26];
   char *inBuf = (char *) malloc(BUFSIZE);
   memset(&table, 0, sizeof(int) * 26);
   int bytesRead = 0;

   while (0 != (bytesRead = read(inFd, inBuf, BUFSIZE))) {
      for (int i = 0; i < bytesRead; i++) {
         // eat letters until skip is done
         if (skip && isalpha(inBuf[i])) {
            --skip;
         }
         // valid letter and time to count a letter
         else if (curPeriod == 1 && isalpha(inBuf[i])) {
            table[toupper(inBuf[i]) - 'A']++;
            printf("Found a %c\n", toupper(inBuf[i]));
            ++validChars;
            curPeriod = period;
         }
         // valid char, need to skip though
         else if (isalpha(inBuf[i])) {
            --curPeriod;
         }
      }
   }

   for (int i = 0; i < 26; i++) {
      string hist("");
      for (int j = 0; 
            j < (int) (table[i] * 100.0 / validChars); j++) {
         hist += '*';
      }
      printf("%c: %9d ( %05.2f%%) %s\n", 'A' + i, table[i], 
                                         table[i] * 100.0 / validChars,
                                         hist.c_str());
   }

   if (inFd != STDIN_FILENO) {
      close(inFd);
      if (outFd != STDOUT_FILENO) {
         close(outFd);
      }
   }
}
