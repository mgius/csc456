/* Mark Gius
 * CSC456
 * vig - vigenere enciphering/deciphering
 * unmodified from program1
 */
#include <ctype.h> // isalpha
#include <fcntl.h> // open,
#include <stdio.h> // printf, et al
#include <stdlib.h> // malloc
#include <string> // c++ string

#define BUFSIZE 1024

using namespace std;

void usage() {
   printf("usage: vig [ -v ] [ -d ] key [infile [ outfile ] ]\n");
   exit(1);
}

// Global variables, unfortunate evil for key and decode flag
// 1 if encoding, -1 if decoding
int decode = 1;
string key("");

// Function to keep track of conversion internally.
inline char convert(char c) {
   static unsigned int count = 0;
   c += decode * (key[count] - 'A');
   if (c > 'Z') {
      c -= 26;
   }
   if (c < 'A') {
      c += 26;
   }
   count++;
   if (count >= key.length()) {
      count = 0;
   }
   return c;
}

int main(int argc, char **argv) {
   bool verbose = false;
   int inFd = STDIN_FILENO, outFd = STDOUT_FILENO;

   // Cmdline parsing
   for (int i=1; i < argc; i++) {
      string arg(argv[i]);
      // Verbose doesn't do anything, but here to comply with spec
      if (!arg.compare("-v")) {
         verbose = true;
      }
      // Decrypt flag
      else if (!arg.compare("-d")) {
         decode = -1;
      }
      // all other flags invalid
      else if (arg[0] == '-') {
         usage();
      }
      // first non-flag token is the key
      else if (!key.compare("")) {
         for (unsigned int i = 0; i < arg.length(); i++) {
            if (!isalpha(arg[i])) {
               usage();
            }
            key += toupper(arg[i]);
         }
      }
      // second non-flag token in the input file
      else if (inFd == STDIN_FILENO) {
         inFd = open(arg.c_str(), O_RDONLY);
         if (-1 == inFd) {
            perror("input file");
            usage();
         }
      }
      // third non-flag token is the output file
      else if (outFd == STDOUT_FILENO) {
         outFd = open(arg.c_str(), 
                      O_WRONLY | O_TRUNC | O_CREAT, 
                      S_IRUSR | S_IWUSR );
         if (-1 == outFd) {
            perror("output file");
            usage();
         }
      }
      // fourth non-flag token is an error
      else {
         printf("Invalid arguments\n");
         usage();
      }
   }

   char *inBuf = (char*) malloc(BUFSIZE);
   int bytesRead = 0;
   // I don't have any guarantee to read BUFSIZE, so loop internally on
   // the number of bytes actually read
   while (0 != (bytesRead = read(inFd, inBuf, BUFSIZE))) {

      for (int i = 0; i < bytesRead; i++) {
         if (isalpha(inBuf[i])) {
            inBuf[i] = convert(toupper(inBuf[i]));
         }
      }
      write(outFd, inBuf, bytesRead);
   }

   // clean up file descriptors
   if (inFd != STDIN_FILENO) {
      close(inFd);
      if (outFd != STDOUT_FILENO) {
         close(outFd);
      }
   }
}
