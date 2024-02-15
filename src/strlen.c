#include <unistd.h>
#include <stdio.h>

size_t	strlen(const char *s){
	printf("USE IT FROM MY LIBRARY\n");
	size_t i = 0;
	while (s[i])
		i++;
	return (i);
}
