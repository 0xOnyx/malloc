#include "malloc.h"

void print_block( enum memory_plage index){
	int nbr;
	t_list *list;

	list = g_memory[index];
	printf("test => %p\n", list);
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