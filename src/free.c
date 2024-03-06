#include "malloc.h"

t_list	*getList(void *ptr, enum memory_plage *index) {
	t_list *list;

	for (*index = TINY; *index < NONE; (*index)++)
	{
		list = g_memory[*index];
		while(list != NULL){
			void *block = (void *)list + HEADER;
			void *end = block + list->size;
			if (ptr >= block && ptr <= end)
				return list;
			list = list->next;
		}
	}
	return NULL;
}

void	ft_free(void *ptr) {
	t_list *list;
	enum memory_plage index;

	if (ptr == NULL)
		return;
	list = getList(ptr, &index);
	if (list == NULL) {
		dprintf(2, "malloc: *** error for object %p: pointer being freed was not allocated\n", ptr);
		dprintf(2, "malloc: *** set a breakpoint in malloc_error_break to debug\n");
		return;
	}
	void *addr = ptr - 2 * BOOKUNIT;
	void *addrPrev = NULL;
	BLOCK_VACANT(addr) = FREE;

	if (addr - BOOKUNIT > (void *) list + HEADER) {
		addrPrev = addr - BLOCK_LEN(addr - BOOKUNIT) - BOOKKEEPING;
		if (BLOCK_VACANT(addrPrev) == FREE) {
			size_t size = BLOCK_LEN(addrPrev) + BLOCK_LEN(addr) + BOOKKEEPING;
			BLOCK_LEN(addrPrev) = size;
			BLOCK_END(addrPrev, size) = size;
			addr = addrPrev;
		}
	}
	void *addrNext = addr + BLOCK_LEN(addr) + BOOKKEEPING;
	if (addrNext < (void *) list + HEADER + list->size) {
		if (BLOCK_VACANT(addrNext) == FREE) {
			size_t finalSize = BLOCK_LEN(addr) + BLOCK_LEN(addrNext) + BOOKKEEPING;
			BLOCK_LEN(addr) = finalSize;
			BLOCK_END(addr, finalSize) = finalSize;
		}
	}
	if (BLOCK_LEN((void *) list + HEADER) + BOOKKEEPING == list->size) {
		t_list **prevList = &g_memory[index];
		while (*prevList && (*prevList)->next != list && (*prevList)->next != NULL)
			*prevList = (*prevList)->next;
		*prevList = list->next; //todo: comprendre why *prevList et non (*prevList)->next
		munmap((void *) list, list->size);
	}
}