#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int myWrite(int fd, void *buf, unsigned int bytes) {
   unsigned int writeCount = -1;
   if (bytes != (writeCount = write(fd, buf, bytes))) {
		error(errno, errno, 
            "ERROR: Expected to wrtie %d bytes, wrote %d\n", 
            bytes, writeCount);
   }
   return writeCount;
}

int main(int argc, const char* argv[]) {
	unsigned int writeCount, baseAddress, uncalledAddress;
	char standard[12] = "abcdabcdabcd";

   if (argc < 3) {
      perror("Usage: configure BaseAsHex UncalledAsHex");
      exit(1);
   }
   if (1 != sscanf(argv[1], "%X", &baseAddress)) {
      perror("Couldn't scan first argument as hexadecimal");
      exit(1);
   }

   if (1 != sscanf(argv[2], "%X", &uncalledAddress)) {
      perror("Couldn't scan second argument as hexadecimal");
      exit(1);
   }
   myWrite(STDOUT_FILENO, standard, 12);
   myWrite(STDOUT_FILENO, &baseAddress, 4);
   myWrite(STDOUT_FILENO, &uncalledAddress, 4);
}
