#include "malloc.h"

void	*calloc(size_t nmemb, size_t size){
	void *ptr;
	ptr = malloc(nmemb * size);
	if (ptr)
		bzero(ptr, nmemb * size);
	return (ptr);
}