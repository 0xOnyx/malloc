#include <string.h>
#include <stdio.h>

int main() {
	char *s = "Hello, World!";
	void *test = (void *)s;
	char *s1 = test + 2;
	printf("strlen(\"%s\") = %lu\n", s, strlen(s));
	printf("s1: %s\n", s1);
	return 0;
}