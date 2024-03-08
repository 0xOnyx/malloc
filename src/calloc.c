#include "malloc.h"

void	*calloc(size_t nmemb, size_t size){
	void *ptr;
	ptr = malloc(nmemb * size);
	if (ptr)
		ft_bzero(ptr, nmemb * size);
	return (ptr);
}