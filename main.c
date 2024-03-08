#define M (1024 * 1024) // todo delete

#include "malloc.h"

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
//	char *addr1;
//	char *addr3;
//
//	addr1 = (char *)malloc(16 * M);
//	bzero(addr1, 16 * M);
//	strcpy(addr1, "Bonjours\n");
//	ft_printf("%s", addr1);
//	//print_memory();
//	show_alloc_mem();
//	addr3 = (char *) realloc(addr1, 128*M);
//	addr3[127*M] = 42;
//	ft_printf("%s", addr3);
	//print_memory();
	show_alloc_mem();
	malloc(10000000450003);
	show_alloc_mem();
	return 0;
};
