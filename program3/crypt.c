#include <unistd.h>
#include <stdio.h>

int main(void) {
	printf("%s\n", crypt("test", "1"));
}
