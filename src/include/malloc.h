#ifndef MALLOC_MALLOC_H
# define MALLOC_MALLOC_H

# include <stdbool.h>
# include <stdio.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

#include <strings.h> //todo modify with ft
#include <string.h> //todo modify with ft

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

/// \brief defining different page type,according to their size
enum memory_plage {
	TINY,
	SMALL,
	LARGE,
	NONE
};

/// \brief defining state of memory slot
enum vacant {
	FREE = 1,
	USED,
};

/// \brief define type of interaction with the memory
enum memory_manage {
	CREATE,
	DELETE,
	REALLOC
};

/// \brief linked listing all memory pages
typedef struct s_list {
	size_t				size;
	struct s_list		*next;
} t_list;

/// \brief the global variable containing all memory pages created
t_list *g_memory[3];

/// \brief pending on the size paramater, either use a slot within a memory page or create a new one
/// \param size memory dynamically allocated length
/// \return  memory dynamically allocated address
void	*malloc(size_t size);

/// \brief set the memory pointed by ptr as free, and if necessary, free the page containing ptr.
/// \param ptr the memory to be freed
void	free(void *ptr);

/// \brief like a malloc, but sets nmemb * size byte to 0
/// \param nmemb memory dynamically allocated type size
/// \param size memory dynamically allocated length
/// \return memory dynamically allocated address
void	*calloc(size_t nmemb, size_t size);

/// \brief free memory pointed by ptr and creates an new one of length define by size
/// \param ptr the memory to be reallocated
/// \param size memory dynamically reallocated length
/// \return memory dynamically reallocated address
void	*realloc(void *ptr, size_t size);

/// \brief print all the pages content
void	print_memory();

/// \brief print all the pages content as requested from the subject
void	show_alloc_mem();

#endif //MALLOC_MALLOC_H
