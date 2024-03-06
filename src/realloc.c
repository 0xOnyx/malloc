#include "malloc.h"

void	*ft_realloc(void *ptr, size_t size) {
	void *new_ptr;
	new_ptr = ft_malloc(size);
	if (new_ptr) {
		memcpy(new_ptr, ptr, BLOCK_LEN(ptr - 2 * BOOKUNIT));
		ft_free(ptr);
	}
	return (new_ptr);
}