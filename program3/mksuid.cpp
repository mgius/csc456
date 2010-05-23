#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/* I picked group 50 because it's defined as "staff" on my system */
#define STUDENTUID 1000
#define TARGETUSER 0
#define TARGETGRP 50

/* Gets the current state of the STDIN FD 
   Caller is responsible for freeing returned pointer 
 */
struct termios *getTerm() {
	struct termios *current = (struct termios*) malloc(sizeof(struct termios));
	if (0 != tcgetattr(STDIN_FILENO, current)) {
		perror("getTerm - tcgettattr");
		exit(1);
	}
	return current;
}

/* sets the terminal to the passed termios.  
	Caller is responsible for freeing passed pointer
 */
void setTerm(struct termios *newTermTerm) {
	if (0 != tcsetattr(STDIN_FILENO, TCSADRAIN, newTermTerm)) {
		perror("setTerm - tcsetattr");
		exit(1);
	}
}

int main(void) {
	struct termios *old = NULL, *newTerm = NULL;
	struct spwd *passEnt = NULL;
	char *password = (char *) malloc(80);
	size_t passLength = 0;

	if (STUDENTUID != getuid()) {
		printf("Sorry, you are not authorized to run this program\n");
		exit(1);
	}
	
	/* disable echo for password entry */
	old = getTerm(), newTerm = getTerm();
	newTerm->c_iflag &= ~ECHO;
	setTerm(newTerm);

	printf("Enter your password: ");

	/* getline is POSIX standard.  It's just part of a relatively new POSIX */
	if (0 > getline(&password, &passLength, stdin)) {
		printf("Error while reading password");
		/* make an attempt to restore the terminal */
		setTerm(old);
		exit(1);
	}

	passEnt = getspnam("mgius");
	printf("%s\n", passEnt->sp_pwdp);

	free(old), free(newTerm);
}
