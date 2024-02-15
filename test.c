#include <string.h>
#include <stdio.h>

int main() {
	char *s = "Hello, World!";
	printf("strlen(\"%s\") = %lu\n", s, strlen(s));
	return 0;
}