/* Mark Gius
 * Program 2: kasiski
 * CSC456
 * Kasiski attack on vigenere cycle
 */
#include <iostream>
#include <list>
#include <map>
#include <string>

#include <ctype.h> // isAlpha, toupper
#include <fcntl.h> // open
#include <stdio.h> // printf, fread, fwrite
#include <string.h>
#include <stdlib.h> // malloc
#include <sys/stat.h> // open
#include <sys/types.h> // open
#include <unistd.h> // read, write

#include "ic.h"
using namespace std;

// map from string to list of locations, which contains all the
// information I need to keep track of, but I can't sort it properly!
map<string, list<int> > matches;

// Enter the matchRecord, which I can sort properly
struct matchRecord {
	string str;
	list<int> locations;

	bool operator<(matchRecord &other) {
		if (str.length() != other.str.length()) {
			return str.length() < other.str.length();
		}
		if (locations.size() != other.locations.size()) {
			return locations.size() < other.locations.size();
		}
		return str < other.str;
	}
};

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
int readInput(int inFd, char **sanitized) {
   char *buf = (char *) malloc(BUFSIZE);
   *sanitized = (char *) calloc(1, BUFSIZE * 4);
   int validChars = 0, bytesRead = 0, saniLen = BUFSIZE * 4;

   while (0 < (bytesRead = read(inFd, buf, BUFSIZE))) {
	   char cur = buf[0];
      for (int i = 0; i < bytesRead; i++, cur = buf[i]) {
         if (isalpha(cur)) {
				if (validChars <= saniLen) {
					saniLen *= 2;
					*sanitized = (char *) realloc(*sanitized, saniLen);
					if (NULL == *sanitized) {
						perror("readInput");
						exit(1);
					}
				}
				*(*sanitized + validChars) = toupper(cur);
				++validChars;
         }
      }
   }
	return validChars;
}

int main(int argc, char **argv) {
   bool verbose = false;
   int minLength = 3;
   // Ugh, I like using read(2), but it looks like I need to bounce through
   // a stream, and you can lseek through stdin
   int inFd = STDIN_FILENO, outFd = STDOUT_FILENO;
	char *sanitizedInput = NULL;
	int charCount = 0;

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
      else if (inFd == STDIN_FILENO) {
         inFd = open(arg.c_str(), O_RDONLY);
         if (-1 == inFd) {
            perror("input file");
            usage();
         }
      }
      // second non-flag argument is output
      else if (outFd == STDOUT_FILENO) {
         outFd = open(arg.c_str(), 
                      O_WRONLY | O_TRUNC | O_CREAT, 
                      S_IRUSR | S_IWUSR );
         if (-1 == outFd) {
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

	// Read in the chiphertext, stripping out invalid chars and 
	// mapping to uppercase
	charCount = readInput(inFd, &sanitizedInput);

   // clean up file descriptors
   if (inFd != STDIN_FILENO) {
      close(inFd);
      if (outFd != STDOUT_FILENO) {
         close(outFd);
      }
   }
}
