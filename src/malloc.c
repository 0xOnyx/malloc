
#include "malloc.h"

enum memory_plage {
	TINY,
	SMALL,
	LARGE,
	NONE
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

#define ALIGN_PAGE(size) (size) % PAGE_SIZE ? ((size) / PAGE_SIZE + 1) * PAGE_SIZE : (size)

#define get_index_memory(size) (size <= TINY_SIZE ? TINY : size <= SMALL_SIZE ? SMALL : LARGE)
#define get_page_size(size) (size <= TINY_SIZE ? TINY_PAGE : size <= SMALL_SIZE ? SMALL_PAGE : ALIGN_PAGE(size + BOOKKEEPING + HEADER))

#define BLOCK_LEN(addr) *((size_t *)(addr))
#define BLOCK_VACANT(addr) *(((size_t *)(addr)) + 1)
#define BLOCK_END(addr, size) *(size_t *)((addr) + size + 2 * BOOKUNIT)

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
	if (current_memory + add_size > _sof_limit)
		return false;
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

void	*get_memory(size_t size, enum memory_plage index){
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

void	*ft_malloc(size_t size){
	return get_memory(size, get_index_memory(size));
}

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

		while ((*prevList)->next != list && (*prevList)->next != NULL)
			*prevList = (*prevList)->next;
		*prevList = list->next;
		munmap((void *) list, list->size);

	}
}

#include <strings.h> //todo modify with ft
void	*ft_calloc(size_t nmemb, size_t size){
	void *ptr;
	ptr = ft_malloc(nmemb * size);
	if (ptr)
		bzero(ptr, nmemb * size);
	return (ptr);
}

#include <string.h> //todo modify with ft
void	*ft_realloc(void *ptr, size_t size) {
	void *new_ptr;
	new_ptr = ft_malloc(size);
	if (new_ptr) {
		memcpy(new_ptr, ptr, BLOCK_LEN(ptr - 2 * BOOKUNIT));
		ft_free(ptr);
	}
	return (new_ptr);
}

#include <stdio.h>
void print_block( enum memory_plage index){
	int nbr;
	t_list *list;

	list = g_memory[index];
	while(list != NULL){
		void *block = (void *)list + HEADER;
		void *end = block + list->size;
		printf("\tnew block size => %lu\n", list->size);
		nbr = 1;
		while(block < end){
			size_t current_size = BLOCK_LEN(block);
			size_t use = BLOCK_VACANT(block);
			printf("\t[%2d][%lu][%lu] --%s-- [%lu]\t", nbr - 1, current_size, use, use == USED ? "USED": "FREE", BLOCK_END(block, current_size));
			block += current_size + BOOKKEEPING;
			if (nbr % 4 == 0) {
				printf("\n");
			}
			nbr++;
		}
		printf("\n\n");
		list = list->next;
	}
}


void print_memory(){
	printf("[plage data for TINY]\n");
	print_block(TINY);
	printf("[plage data for SMALL]\n");
	print_block(SMALL);
	printf("[plage data for LARGE]\n");
	print_block(LARGE);
}

#define M (1024 * 1024)
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
