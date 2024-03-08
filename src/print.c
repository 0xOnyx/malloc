#include "malloc.h"

static void print_block( enum memory_plage index){
	int nbr;
	t_list *list;

	list = g_memory[index];
	while(list != NULL){
		void *block = (void *)list + HEADER;
		void *end = block + list->size;
		ft_printf("\tnew block size => %l\n", list->size);
		nbr = 1;
		while(block < end){
			size_t current_size = BLOCK_LEN(block);
			size_t use = BLOCK_VACANT(block);
			ft_printf("\t[%2d][%l][%l] --%s-- [%l]\t", nbr - 1, current_size, use, use == USED ? "USED": "FREE", BLOCK_END(block, current_size));
			block += current_size + BOOKKEEPING;
			if (nbr % 4 == 0) {
				ft_printf("\n");
			}
			nbr++;
		}
		ft_printf("\n\n");
		list = list->next;
	}
}

void print_memory(){
	ft_printf("[plage data for TINY]\n");
	print_block(TINY);
	ft_printf("[plage data for SMALL]\n");
	print_block(SMALL);
	ft_printf("[plage data for LARGE]\n");
	print_block(LARGE);
}

static void show_all_block( enum memory_plage index, char *str){
	int nbr;
	t_list *list;

	list = g_memory[index];
	while(list != NULL){
		void *block = (void *)list + HEADER;
		void *end = block + list->size;
		ft_printf("%s : %p\n", str, list);
		nbr = 1;
		while(block < end){
			size_t current_size = BLOCK_LEN(block);
			size_t use = BLOCK_VACANT(block);
			if (use == USED)
				ft_printf("%p - %p : %l bytes\n", block, block + current_size, current_size);
			block += current_size + BOOKKEEPING;
			nbr++;
		}
		ft_printf("\n\n");
		list = list->next;
	}
}

void show_alloc_mem() {
	show_all_block(TINY, "TINY");
	show_all_block(SMALL, "SMALL");
	show_all_block(LARGE, "LARGE");
}
