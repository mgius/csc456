#include <fcntl.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* I picked group 27 because it's defined as "sudo" on my system */
#define STUDENTUID 1000
#define STUDENTNAME "mgius"
#define TARGETUID 0
#define TARGETGID 27

#define SALT_MAX 17
#define CRYPT_MAX 87

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

/* grabs the password hash and salt from the shadow file, and 
 * sets the two passed pointers to the right locations in that
 * I'm unsure if the entity returned by getspnam should be free'd
 */
void getSaltAndHash(char **pass, char **salt) {
	struct spwd *passEnt = NULL;
   /* The fact that you can't specify a UID annoys me */
	if (0 == (passEnt = getspnam(STUDENTNAME))) {
      printf("Couldn't get ahold of your stored password\n"
             "Program likely not running as root\n");
      exit(1);
   }
         
   /* Gets me the position of the beginning of the actual hash */

   if (passEnt->sp_pwdp[0] == '$') {
      //*pass = rindex(passEnt->sp_pwdp, '$') + 1;
      *pass = passEnt->sp_pwdp;
      //strncpy(*salt, passEnt->sp_pwdp, ((int)(*pass - passEnt->sp_pwdp));
      strncpy(*salt, passEnt->sp_pwdp,
              (int)(rindex(passEnt->sp_pwdp, '$') + 1 - passEnt->sp_pwdp));
   }
   else {
      // using DES
      //*pass = passEnt->sp_pwdp + 2;
      *pass = passEnt->sp_pwdp;
      strncpy(*salt, passEnt->sp_pwdp, 2);
   }

   //printf("pass: %s\nsalt: %s\n", *pass, *salt);

}

bool verifyFile(int sniffFd) {
   struct stat *sniffStat = (struct stat *) malloc(sizeof(struct stat));
   /* stat the file descriptor I just opened */
   if (0 > fstat(sniffFd, sniffStat)) {
      perror("fstat");
      exit(1);
   }
   /* check the various bits of the sniff program */
   if (sniffStat->st_uid != STUDENTUID) {
      printf("sniff is not owned by %s\n", STUDENTNAME);
      exit(1);
   }
   if (0 != (sniffStat->st_mode & (S_IRWXG | S_IRWXO))) {
      printf("Group and/or Other permissions incorrect\n");
      exit(1);
   }
   if (0 == (sniffStat->st_mode & S_IXUSR)) {
      printf("User execute permission not set\n");
      exit(1);
   }
   if (time(NULL) - 60 > sniffStat->st_mtime || 
       time(NULL) < sniffStat->st_mtime) {
      printf("sniff is too old, or is from the future!\n");
      exit(1);
   }
   free(sniffStat);
   return true;
}

int main(void) {
	struct termios *oldTerm = NULL, *newTerm = NULL;
	char *password = NULL;
   char *shadowHash = NULL;
   char *passwordHash = NULL;
   char *salt = (char *)calloc(SALT_MAX,1);
	size_t passLength = 0;


   int sniffFd = -1;

	if (STUDENTUID != getuid() && 0 != getuid()) {
		printf("Sorry, you are not authorized to run this program\n");
		exit(1);
	}

   /* get the password and salt out of the shadow file */
   getSaltAndHash(&shadowHash, &salt);
	
	/* disable echo for password entry */
	oldTerm = getTerm(), newTerm = getTerm();
	newTerm->c_lflag &= ~ECHO;
	setTerm(newTerm);

	printf("Enter your password: ");

	/* getline is POSIX standard.  It's just part of a relatively new POSIX */
	if (0 > getline(&password, &passLength, stdin)) {
		perror("Error while reading password");
		/* make an attempt to restore the terminal */
		setTerm(oldTerm);
		exit(1);
	}
   /* getline includes the newline */
   if (password[strlen(password) - 1] == '\n') {
      password[strlen(password) - 1] = '\0';
   }
   /* Immmediately crypt the password, then zero it */
   /* possible vulnerability: password gets swapped to disk */
   passwordHash = crypt(password, salt);
   memset(password, 0, passLength);
   /* Even worse:
    * http://msdn.microsoft.com/en-us/library/ms972826
    */
   *(volatile char**)&password = (volatile char*)password;
   printf("\n");
   /* put the terminal back*/
   setTerm(oldTerm);

   if (strcmp(passwordHash, shadowHash)) {
      printf("Incorrect Password\n");
      exit(1);
   }

   /* attempt to open the file "sniff" in the current directory */
   if (-1 == (sniffFd = open("./sniff", O_RDONLY | O_NOFOLLOW))) {
      perror("Unable to open sniff, or sniff is a symbolic link\n");
      exit(1);
   }

   if (!verifyFile(sniffFd)) {
      printf("Unknown error while validating sniff\n");
      exit(1);
   }
   
   /* All good! 
    * Use f* methods to prevent swapping out the file on me
    * chown then chmod.  I want to take control of the file ASAP,
    * so that the user can't muck with it any more
    */
   fchown(sniffFd, TARGETUID, TARGETGID);
   fchmod(sniffFd, S_ISUID | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP);

	free(oldTerm), free(newTerm);
   //free(password), free(shadowHash), free(salt);
   free(salt);
}
