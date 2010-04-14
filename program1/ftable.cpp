#include <errno.h>
#include <fcntl.h>
#include <map>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

void usage(void) {
   printf("usage: ftable [ -v ] [ -s num ] [-p num ] [ infile [ outfile ] ]\n");
   exit(1);
}

int main(int argc, char **argv) {
   bool verbose = false;
   int skip = 0;
   int period = 0;
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
      }
      else if (!arg.compare("-p")) {
         char *temp;
         period = strtol(argv[++i], &temp, 10); 
         // Check for a bad strtol. Probably a more efficient way to do this
         if (temp != argv[i] + string(argv[i]).length()) {
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

   if (inFd != STDIN_FILENO) {
      close(inFd);
      if (outFd != STDOUT_FILENO) {
         close(outFd);
      }
   }
}
