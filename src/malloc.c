#include "malloc.h"

bool can_alloc(size_t current_memory, size_t add_size){
	static rlim_t _sof_limit;
	struct rlimit limit;

	if (_sof_limit <= 0){
		if (getrlimit(RLIMIT_DATA, &limit) < 0)
			return false;
		_sof_limit = limit.rlim_cur;

	}
	if (current_memory + add_size > _sof_limit)
		return false;
	return true;
}

void init_bookkeeping(void *addr, size_t size){
	BLOCK_LEN(addr) = size - BOOKKEEPING;
	BLOCK_VACANT(addr) = FREE;
	*(size_t *)(addr + size - BOOKUNIT) = size - BOOKKEEPING;
}

void *manage_memory(enum memory_manage option, size_t size){
	static unsigned long long cur_memory;

	if (option == CREATE && can_alloc(cur_memory, size)) {
		cur_memory += size;
		return mmap(NULL,
		size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS,
		-1, 0);
	}
	//todo ft_free and realloc implementation
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
		BLOCK_LEN(addr + size + BOOKKEEPING) = old_free_size - size - BOOKKEEPING;
		if (BLOCK_VACANT(addr + size + BOOKKEEPING) != USED)
			BLOCK_VACANT(addr + size + BOOKKEEPING) = FREE;
		BLOCK_END(addr, old_free_size) = old_free_size - size - BOOKKEEPING;
	}
}

void *free_zone_in(size_t size, enum memory_plage index){
	t_list *list;

	list = g_memory[index];
	while(list != NULL){
		void *block = (void *)list + HEADER;
		void *end = block + list->size;
		while(block < end){
			size_t current_size = BLOCK_LEN(block);
			if (BLOCK_VACANT(block) == FREE && current_size >= size)
				return block;
			block += current_size + BOOKKEEPING;
		}
		list = list->next;
	}
	return NULL;
}

void	append_memory_to_global(enum memory_plage index, t_list *new) {
	t_list *current = g_memory[index];

	if (!current)
		g_memory[index] = new;
	else {
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

void	*get_memory(size_t size, enum memory_plage index) {
	void *addr;

	if (index == LARGE || (addr = free_zone_in(size, index)) == NULL){
		t_list *new = manage_memory(CREATE, get_page_size(size));
		new->size = get_page_size(size) - HEADER;
		new->next = NULL;
		addr = ((void *)new) + HEADER;
		init_bookkeeping(addr, new->size);
		append_memory_to_global(index, new);
	}
	set_memory(size, addr);
	return ((char *) addr) + BOOKUNIT * 2;
}

void	*ft_malloc(size_t size) {
	return get_memory(size, get_index_memory(size));
}

#define M (1024 * 1024) // todo delete
int	main()
{
//	size_t size_tiny = TINY_SIZE - 1;
//	size_t size_small = SMALL_SIZE - 1;
//	size_t size_large = getpagesize() - 130;
//
//	void *tinyblock = ft_malloc(size_tiny);
//
//	void *block[100];
//	for (int i = 0; i < 100; i++){
//		block[i] = ft_malloc(size_small);
//	}
//	void *megablock[4];
//	for (int i = 0; i < 4; i++){
//		megablock[i] = ft_malloc(size_large);
//	}
//	print_memory();
//
//	ft_free(tinyblock);
//	for (int i = 0; i < 100; i++) {
//		ft_free(block[i]);
//	}
//	ft_free(megablock[2]);
//	print_memory();
	char *addr1;
	char *addr3;

	addr1 = (char *)ft_malloc(16 * M);
	bzero(addr1, 16 * M);
	strcpy(addr1, "Bonjours\n");
	printf("%s", addr1);
	print_memory();
	addr3 = (char *) ft_realloc(addr1, 128*M);
	addr3[127*M] = 42;
	printf("%s", addr3);
	print_memory();
	return 0;
};
