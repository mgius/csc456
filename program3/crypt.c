#include <unistd.h>
#include <stdio.h>

int main(void) {
   char hash[90] = "$1$foobar$asdfasdf";
   printf("%s\n", rindex(hash, '$'));
   char buf[90];
   strncpy(buf, hash, (int) (rindex(hash, '$') - buf));
   printf("%s\n", buf);
}
