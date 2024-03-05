NAME = libft_malloc_$(HOSTTYPE).so

LINK_NAME = libft_malloc.so

ifeq ($(HOSTTYPE),)
	HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

IDIR = src/include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -Wextra -Werror -g -fpic

ODIR=obj
SDIR=src

LIBS=

_DEPS = malloc.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = malloc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(NAME) $(LINK_NAME)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -shared -o $(NAME) $(OBJ) $(CFLAGS) $(LIBS)

$(LINK_NAME): $(NAME)
	ln -sf $(NAME) $(LINK_NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME) $(LINK_NAME)

re: fclean all

test:
	gcc $(CFLAGS) src/malloc.c

.PHONY: all fclean clean re test

