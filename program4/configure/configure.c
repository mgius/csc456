#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char* argv[]) {
	unsigned int writeCount, uncalledAddress;
	char standard[12] = "abcdabcdabcd";

   if (argc < 3) {
      perror("Usage: configure BaseAsHex UncalledAsHex");
      exit(1);
   }
   if (1 != sscanf(argv[2], "%X", &uncalledAddress)) {
      perror("Couldn't scan second argument as hexadecimal");
      exit(1);
   }
	if (12 != (writeCount = write(STDOUT_FILENO, standard,12))) {
		error(1, errno, 
            "ERROR: Expected to prime buffer with 12 characters, wrote %d\n", 
            writeCount);
	}
	if (4 != (writeCount = write(STDOUT_FILENO, &uncalledAddress, 4))) {
		error(1, errno, 
		      "ERROR: Expected to write address as 4 bytes, wrote %d\n", 
            writeCount);
   }
}
