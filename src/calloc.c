#include "malloc.h"

void	*ft_calloc(size_t nmemb, size_t size){
	void *ptr;
	ptr = ft_malloc(nmemb * size);
	if (ptr)
		bzero(ptr, nmemb * size);
	return (ptr);
}