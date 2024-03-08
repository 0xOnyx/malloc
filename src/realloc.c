#include "malloc.h"

void	*realloc(void *ptr, size_t size) {
	void *new_ptr;

	new_ptr = malloc(size);
	if (new_ptr && ptr) {
		ft_memcpy(new_ptr, ptr, BLOCK_LEN(ptr - 2 * BOOKUNIT));
		free(ptr);
	}
	return (new_ptr);
}