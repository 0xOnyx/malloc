#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct lst {
	char *str;
	struct lst *next;
} lst;

void test(lst *ptr, lst *list)
{
	lst **prevList = &ptr;
	while (*prevList && (*prevList)->next != list && (*prevList)->next != NULL)
		*prevList = (*prevList)->next;
	printf("test1 => %s %p && %s %p\n", (*prevList)->str, (*prevList)->next, list->str, list->next);
	(*prevList)->next = list->next;
}

lst *org;

int main() {
//	char *s = "Hello, World!";
//	void *test = (void *)s;
//	char *s1 = test + 2;
//	printf("strlen(\"%s\") = %lu\n", s, strlen(s));
//	printf("s1: %s\n", s1);
//	char *ptr = malloc(16 *sizeof(char));
//	free((void *) ptr+5);
//	abort();
	org = calloc(1, sizeof(lst));
	org->str = calloc(2, 1);
	org->str[0] = 'a';
	lst *tmp = org;
	lst *tmp2 = NULL;
	for (int i = 0; i < 15; i++)
	{
		lst *new = calloc(1, sizeof(lst));
		new->str = calloc(2, 1);
		new->str[0] = 'b' + (char) i;
		if (i == 5)
			tmp2 = new;
		tmp->next = new;
		tmp = tmp->next;
	}
	tmp = org;
	while (tmp)
	{
		printf("test1 => %s %p\n", tmp->str, tmp->next);
		tmp = tmp->next;
	}
	printf("\n");
	test(org, tmp2);
//	lst **prevList = &org;
//	while (*prevList && (*prevList)->next != tmp2 && (*prevList)->next != NULL)
//		*prevList = (*prevList)->next;
//	printf("test1 => %s %p && %s %p\n", (*prevList)->str, (*prevList)->next, tmp2->str, tmp2->next);
//	(*prevList)->next = tmp2->next;
	printf("\n");
	tmp = org;
	while (tmp)
	{
		printf("test2 => %s %p\n", tmp->str, tmp->next);
		tmp = tmp->next;
	}
	return 0;
}