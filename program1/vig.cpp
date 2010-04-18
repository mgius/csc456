#include <ctype.h> // isalpha
#include <fcntl.h> // open,
#include <stdio.h> // printf, et al
#include <stdlib.h> // malloc
#include <string> 

#define BUFSIZE 1024

using namespace std;

void usage() {
   printf("usage: vig [ -v ] [ -d ] key [infile [ outfile ] ]\n");
   exit(1);
}

// 1 if encoding, -1 if decoding
int decode = 1;
string key("");
unsigned int count = 0;

inline char convert(char c) {
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
      if (!arg.compare("-v")) {
         verbose = true;
      }
      else if (!arg.compare("-d")) {
         decode = -1;
      }
      else if (arg[0] == '-') {
         usage();
      }
      else if (!key.compare("")) {
         for (unsigned int i = 0; i < arg.length(); i++) {
            if (!isalpha(arg[i])) {
               usage();
            }
            key += toupper(arg[i]);
         }
      }
      else if (inFd == STDIN_FILENO) {
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

   char *inBuf = (char*) malloc(BUFSIZE);
   int bytesRead = 0;
   while (0 != (bytesRead = read(inFd, inBuf, BUFSIZE))) {
      for (int i = 0; i < bytesRead; i++) {
         if (isalpha(inBuf[i])) {
            inBuf[i] = convert(toupper(inBuf[i]));
         }
      }
      write(outFd, inBuf, bytesRead);
   }

   if (inFd != STDIN_FILENO) {
      close(inFd);
      if (outFd != STDOUT_FILENO) {
         close(outFd);
      }
   }
}
