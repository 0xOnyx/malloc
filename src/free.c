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

void    removeList(t_list *list, enum memory_plage index)
{
	t_list *tmp = g_memory[index];
	t_list *prev = NULL;

	while(tmp)
	{
		if (tmp == list)
		{
			if (prev)
				prev->next = tmp->next;
			else
				g_memory[index] = tmp->next;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	adjustInfo(void **addr1, void **addr2, bool option)
{
	if ((option && BLOCK_VACANT(*addr1) == FREE) \
	|| (!option && BLOCK_VACANT(*addr2) == FREE)) {
		size_t size = BLOCK_LEN(*addr1) + BLOCK_LEN(*addr2) + BOOKKEEPING;
		BLOCK_LEN(*addr1) = size;
		BLOCK_END(*addr1, size) = size;
		if (option)
			*addr2 = *addr1;
	}
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
		//abort();
	}
	void *addr = ptr - 2 * BOOKUNIT;
	BLOCK_VACANT(addr) = FREE;

	// check if previous slot is also empty
	if (addr - BOOKUNIT > (void *) list + HEADER) {
		void *addrPrev = addr - BLOCK_LEN(addr - BOOKUNIT) - BOOKKEEPING;
		adjustInfo(&addrPrev, &addr, true);
	}

	//check if next slot is also empty
	void *addrNext = addr + BLOCK_LEN(addr) + BOOKKEEPING;
	if (addrNext < (void *) list + HEADER + list->size) {
		adjustInfo(&addr, &addrNext, false);
	}

	//remove the page if full free
	if (BLOCK_LEN((void *) list + HEADER) + BOOKKEEPING == list->size) {
		removeList(list, index);
	}
}
