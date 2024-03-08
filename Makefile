NAME = libft_malloc_$(HOSTTYPE).so

LINK_NAME = libft_malloc.so

ifeq ($(HOSTTYPE),)
	HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

IDIR = src/include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -Wextra -Werror -g  -fpic -fPIC

ODIR=obj
SDIR=src

LIBS=

_DEPS = malloc.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = malloc.o calloc.o free.o print.o realloc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(NAME) $(LINK_NAME)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -shared -o $(@) $(OBJ) $(CFLAGS) $(LIBS)

$(LINK_NAME): $(NAME)
	ln -sf $(NAME) $(LINK_NAME)

clean:
	rm -rf $(OBJ)
	rm -rf $(ODIR)


fclean: clean
	rm -rf $(NAME) $(LINK_NAME) test

re: fclean all


test:	all
	gcc $(CFLAGS) main.c -o $(@) -L. -lft_malloc
	./run.sh ./test

.PHONY: all fclean clean re test

