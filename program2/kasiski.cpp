/* Mark Gius
 * Program 2: kasiski
 * CSC456
 * Kasiski attack on vigenere cycle
 */
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>

#include <ctype.h> // isAlpha, toupper
#include <fcntl.h> // open
#include <stdio.h> // printf, fread, fwrite
#include <string.h> // strncmp, strlen
#include <stdlib.h> // malloc
#include <sys/stat.h> // open
#include <sys/types.h> // open
#include <unistd.h> // read, write

#include "ic.h"
#include "FString.h"
using namespace std;

// map from string to list of locations, which contains all the
// information I need to keep track of, but I can't sort it properly!
map<string, set<int> > matches;

// Enter the matchRecord, which I can sort properly
struct matchRecord {
	string str;
	set<int> locations;

	matchRecord(string s, set<int> locs) : str(s), locations(locs) {}

	bool operator<(matchRecord &other) {
		if (str.length() != other.str.length()) {
			return str.length() > other.str.length();
		}
		if (locations.size() != other.locations.size()) {
			return locations.size() > other.locations.size();
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
				if (validChars >= saniLen) {
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
   free(buf);
	return validChars;
}

/* finds all matches of a particular keySize, returns
 * true if at least one match was found
 */
bool findMatchesOfLength(char *begin, int keySize) {
	bool result = false;
	char match[keySize + 1];
	match[keySize] = '\0';

	// loop from first char to two key lengths from the end
	for (int i = 0; i <= (int)strlen(begin) - keySize * 2; i++) {
		//printf("outer loop %d\n", i);
		bool foundOne = false;
		// loop til the end of the cipher minus one key length
		for (int j = i + keySize; j <= (int)strlen(begin) - keySize; j++) {
			if (0 == strncmp(begin + i, begin + j, keySize)) {
				result = true;
				//printf("Found a match at positions %d and %d\n", i, j);
				if(!foundOne) {
					// first time finding this string.  initialize.
					// copy out the substring we care about, and shove the 
					// initial location into the map
					strncpy(match, begin + i, keySize);

					//// need to add the first location
					matches[string(match)].insert(i);
					foundOne = true;
				}
				// add the found location
				matches[string(match)].insert(j);
            j += keySize - 1;
			}
		}
	}

	return result;
}

/* converts the map to the list of structs */
void convertMapToList(list<matchRecord> *matchList) {
	map<string, set<int> >::iterator it;
	for (it=matches.begin(); it != matches.end(); it++) {
		matchList->push_back(matchRecord((*it).first, (*it).second));
	}
	matchList->sort();
}

/* Some of this code is kind of ganky as a result of me using write(2)
 * instead of the fprintf and FILE*
 */
void printOutput(int outFd, list<matchRecord> &matchList) {
	string header("Length   Count  Word    Location (distance)\n");
	header +=     "======   =====  ====    =========\n";
	write(outFd, header.c_str(), header.length());
	for(list<matchRecord>::iterator it = matchList.begin();
		 it != matchList.end();
		 it++) {
		string line(FString("%6d   %5d  %s    ", (*it).str.length(),
															  (*it).locations.size(),
															  (*it).str.c_str()));
		set<int>::iterator setIt = (*it).locations.begin();
		int firstLoc = (*setIt);
		setIt++;
		int secondLoc = (*setIt);
		setIt++;
		line += FString("%d %d (%d) ", firstLoc, secondLoc,
												 secondLoc - firstLoc);
		

		while (setIt != (*it).locations.end()) {
			firstLoc = secondLoc;
			secondLoc = (*setIt);
			line += FString("%d (%d) ", secondLoc,
												 secondLoc - firstLoc);

			setIt++;
		}
		line += "\n";
		write(outFd, line.c_str(), line.length());
	}

}

int main(int argc, char **argv) {
   bool verbose = false;
   int minLength = 3;
   // Ugh, I like using read(2), but it looks like I need to bounce through
   // a stream, and you can lseek through stdin
   int inFd = STDIN_FILENO, outFd = STDOUT_FILENO;
	char *sanitizedInput = NULL;
	int charCount = 0;
	list<matchRecord> matchList;

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

	// This probably violates a coding style, but I like it
	for(int i = minLength; findMatchesOfLength(sanitizedInput, i); i++) {}

	// convert the map to the list
	convertMapToList(&matchList);

	printOutput(outFd, matchList);

   // clean up file descriptors
   if (inFd != STDIN_FILENO) {
      close(inFd);
      if (outFd != STDOUT_FILENO) {
         close(outFd);
      }
   }
}
