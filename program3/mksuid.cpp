#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/* I picked group 50 because it's defined as "staff" on my system */
#define STUDENTUID 1000
#define STUDENTNAME "mgius"
#define TARGETUSER 0
#define TARGETGRP 50

#define SALT_MAX 17

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
void setTerm(struct termios *newTerm) {
	if (0 != tcsetattr(STDIN_FILENO, TCSADRAIN, newTerm)) {
		perror("setTerm - tcsetattr");
		exit(1);
	}
}

void getPassHash(char **pass, char **salt) {

}

int main(void) {
	struct termios *oldTerm = NULL, *newTerm = NULL;
	struct spwd *passEnt = NULL;
	char *password = (char *) malloc(80);
   char *hash = NULL;
   char *salt = (char *)calloc(SALT_MAX,1);
	size_t passLength = 0;

	if (STUDENTUID != getuid()) {
		printf("Sorry, you are not authorized to run this program\n");
		exit(1);
	}
	
	/* disable echo for password entry */
	oldTerm = getTerm(), newTerm = getTerm();
	newTerm->c_lflag &= ~ECHO;
	setTerm(newTerm);

	printf("Enter your password: ");

	/* getline is POSIX standard.  It's just part of a relatively new POSIX */
	if (0 > getline(&password, &passLength, stdin)) {
		printf("Error while reading password");
		/* make an attempt to restore the terminal */
		setTerm(oldTerm);
		exit(1);
	}
   printf("\n");
   /* put the terminal back*/
   setTerm(oldTerm);

   /* The fact that you can't specify a UID annoys me */
	if (0 == (passEnt = getspnam(STUDENTNAME))) {
      printf("Couldn't get ahold of your stored password\n"
             "Program likely not running as root\n");
      exit(1);
   }
         
   /* Gets me the position of the beginning of the actual hash */

   if (passEnt->sp_pwdp[0] == '$') {
      hash = rindex(passEnt->sp_pwdp, '$') + 1;
      strncpy(salt, passEnt->sp_pwdp, (int)(hash - passEnt->sp_pwdp));
   }
   else {
      // using DES
      hash = passEnt->sp_pwdp + 2;
      strncpy(salt, passEnt->sp_pwdp, 2);
   }

	free(oldTerm), free(newTerm);
   free(password);
}
