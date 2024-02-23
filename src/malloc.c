
#include "malloc.h"

enum memory_plage {
	TINY,
	SMALL,
	LARGE
};

enum vacant {
	FREE = 1,
	USED,
};

#define HEADER sizeof(t_list)

#define BOOKUNIT sizeof(size_t)
#define BOOKKEEPING (size_t)(3 * BOOKUNIT)

#define PAGE_SIZE (size_t)getpagesize()

#define TINY_SIZE (size_t)(TINY_PAGE / 100)
#define TINY_PAGE (size_t)(4 * PAGE_SIZE)

#define SMALL_SIZE (size_t)(SMALL_PAGE / 100)
#define SMALL_PAGE (size_t)(40 * PAGE_SIZE)

#define ALIGN_PAGE(size) size % PAGE_SIZE ? (size / PAGE_SIZE + 1) * PAGE_SIZE : size

#define get_index_memory(size) (size <= TINY_SIZE ? TINY : size <= SMALL_SIZE ? SMALL : LARGE)
#define get_page_size(size) (size <= TINY_SIZE ? TINY_PAGE : size <= SMALL_SIZE ? SMALL_PAGE : ALIGN_PAGE(size + BOOKKEEPING + HEADER))

#define BLOCK_LEN(addr) *((size_t *)addr)
#define BLOCK_VACANT(addr) *(((size_t *)addr) + 1)
#define BLOCK_END(addr, size) *(size_t *)(addr + size + 2 * BOOKUNIT)

typedef struct s_list {
	size_t				size;
	struct s_list		*next;
} t_list;

t_list *g_memory[3];

bool can_alloc(size_t current_memory, size_t add_size){
	static rlim_t _sof_limit;
	struct rlimit limit;

	if (_sof_limit <= 0){
		if (getrlimit(RLIMIT_DATA, &limit) < 0)
			return false;
		_sof_limit = limit.rlim_cur;
	}
	else {
		if (current_memory + add_size > _sof_limit)
			return false;
	}
	return true;
}

void init_bookkeeping(void *addr, size_t size){
	BLOCK_LEN(addr) = size - BOOKKEEPING;
	BLOCK_VACANT(addr) = FREE;
	*(size_t *)(addr + size - BOOKUNIT) = size - BOOKKEEPING;
}

enum memory_manage {
	CREATE,
	DELETE,
	REALLOC
};

void *manage_memory(enum memory_manage option, size_t size){
	static unsigned long long cur_memory;

	if (option == CREATE && can_alloc(cur_memory, size)) {
		cur_memory += size;
		return mmap(NULL,
		size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1, 0);
	}
//	if (option == DELETE){
//		cur_memory -= free_memory(size);
//		return NULL;
//	}
	return NULL;
}

void set_memory(size_t size, void *addr){
	size_t old_free_size;

	old_free_size = BLOCK_LEN(addr);
	BLOCK_LEN(addr) = size;
	BLOCK_VACANT(addr) = USED;
	BLOCK_END(addr, size) = size;
	if (old_free_size - size)
	{
		BLOCK_LEN(addr + size + BOOKKEEPING) = old_free_size - size - 3 * BOOKUNIT;
		if (BLOCK_VACANT(addr + size + BOOKKEEPING) != USED)
			BLOCK_VACANT(addr + size + BOOKKEEPING) = FREE;
		BLOCK_END(addr, old_free_size) = old_free_size - size - 3 * BOOKUNIT;
	}
}

void *free_zone_in(size_t size, enum memory_plage index){
	t_list *list;

	list = g_memory[index];
	while(list != NULL){
		void *block = list + HEADER;
		void *end = block + list->size;
		while(block < end){
			size_t current_size = BLOCK_LEN(block);
			if (BLOCK_VACANT(block) == FREE && current_size >= size)
				return block;
			block += current_size + BOOKUNIT * 2;
		}
		list = list->next;
	}
	return NULL;
}

void *get_memory(size_t size, enum memory_plage index){
	void *addr;

	if (index == LARGE || (addr = free_zone_in(size, index)) != NULL){
		t_list *new = manage_memory(CREATE, get_page_size(size));
		new->size = get_page_size(size) - HEADER;
		addr = ((void *)new) + HEADER;
		init_bookkeeping(addr, new->size);
	}
	set_memory(size, addr);
	return addr;
}

void	*malloc(size_t size){
	return get_memory(size, get_index_memory(size));
}















void	free(void *ptr){
	(void)ptr;
}

#include <string.h> //todo modify with ft
void	*calloc(size_t nmemb, size_t size){
	void *ptr;
	ptr = malloc(nmemb * size);
	if (ptr)
		bzero(ptr, nmemb * size);
	return (ptr);
}

void	*realloc(void *ptr, size_t size) {
	void *new_ptr;
	new_ptr = malloc(size);
	if (new_ptr) {
		memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}
