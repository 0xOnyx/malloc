
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

#define BOOKUNIT sizeof(size_t)
#define BOOKKEEPING 3 * BOOKUNIT

#define PAGE_SIZE getpagesize()

#define TINY_SIZE TINY_PAGE / 100
#define TINY_PAGE 4 * PAGE_SIZE

#define SMALL_SIZE SMALL_PAGE / 100
#define SMALL_PAGE 40 * PAGE_SIZE

#define get_index_memory(size) (size <= TINY_SIZE ? TINY : size <= SMALL_SIZE ? SMALL : LARGE)
#define get_page_size(size) (size <= TINY_SIZE ? TINY_PAGE : size <= SMALL_SIZE ? SMALL_PAGE : size + BOOKKEEPING)

#define BLOCK_LEN(addr) *((size_t *)addr)
#define BLOCK_VACANT(addr) *(((size_t *)addr) + 1)
#define BLOCK_END(addr, size) *(size_t *)(addr + size + 2 * BOOKUNIT)

void **g_memory[3];


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

void *create_memory(size_t size){
	void *addr = mmap(NULL,
	size, PROT_READ | PROT_WRITE,
	MAP_PRIVATE | MAP_ANONYMOUS,
	-1, 0);
	if (addr != NULL)
	{
		BLOCK_LEN(addr) = size - BOOKKEEPING;
		BLOCK_VACANT(addr) = FREE;
		*(size_t *)(addr + size - BOOKUNIT) = size - BOOKKEEPING;
	}
	return addr;
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
		return create_memory(size);
	}
	if (option == DELETE){
		cur_memory -= free_memory(size);
		return NULL;
	}

}

void set_memory(size_t size, void *addr, void *endBlock){ //todo
	size_t old_free_size;

	old_free_size = BLOCK_LEN(addr);
	BLOCK_LEN(addr) = size;
	BLOCK_END(addr, size) = size;
	if (old_free_size - size)
		BLOCK_END(addr, old_free_size) = old_free_size - size - 3 * BOOKUNIT;
	BLOCK_VACANT(addr) = USED;
	if (&BLOCK_END(addr, size) + BOOKUNIT < endBlock)
		BLOCK_END(addr, size) = FREE;

}

void *free_zone_in(size_t size, enum memory_plage index, void **endBlock){
	void **tab;

	tab = g_memory[index];
	while(tab != NULL && !(*tab)){
		void *block = *tab;
		*endBlock = block + get_page_size(size);
		while(block < *endBlock){
			size_t current_size = BLOCK_LEN(block);
			if (BLOCK_VACANT(block) == FREE && current_size >= size)
				return block;
			block += current_size + BOOKUNIT * 2;
		}
		tab++;
	}
	*endBlock = NULL;
	return NULL;
}


void *get_memory(size_t size, enum memory_plage index){
	void *addr;
	void *endBlock;

	if (index == LARGE || (addr = free_zone_in(size, index, &endBlock) != NULL){
		g_memory[index] = manage_memory(CREATE, get_page_size(size));
		addr = g_memory[index];
	}
	set_memory(size, addr, endBlock);
}

void	*malloc(size_t size){
	return get_memory(size, get_index_memory(size));
}















void	free(void *ptr){
	manage_memory()// and more
}
void	*calloc(size_t nmemb, size_t size){
	void *ptr;
	ptr = malloc(nmemb * size);
	if (ptr)
		ft_bzero(ptr, nmemb * size);
	return (ptr);
}

void	*realloc(void *ptr, size_t size) {
	void *new_ptr;
	new_ptr = malloc(size);
	if (new_ptr) {
		ft_memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}
