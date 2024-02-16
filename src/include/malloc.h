#ifndef MALLOC_MALLOC_H
# define MALLOC_MALLOC_H
# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <stdbool.h>


void	*malloc(size_t size);
void	free(void *ptr);
void	*calloc(size_t nmemb, size_t size);
void	*realloc(void *ptr, size_t size);


#endif //MALLOC_MALLOC_H
