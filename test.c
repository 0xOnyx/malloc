#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
//	char *s = "Hello, World!";
//	void *test = (void *)s;
//	char *s1 = test + 2;
//	printf("strlen(\"%s\") = %lu\n", s, strlen(s));
//	printf("s1: %s\n", s1);
	char *ptr = malloc(16 *sizeof(char));
	free((void *) ptr+5);
//	abort();
	return 0;
}