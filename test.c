#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct lst {
	char *str;
	struct lst *next;
} lst;

lst *org;

void test(lst *list)
{
	lst *tmp = org;
	while (org && org != list && org->next != list && org->next != NULL)
		org = org->next;
	org->next = list->next;
	ft_printf("test => %s %p & %s %p\n", org->str, org->next, list->str, list->next);
	free(list->str);
	free(list);
	org = tmp;
}

void    removeNode(lst *list)
{
	lst *tmp = org;
	lst *prev = NULL;

	while(tmp)
	{
		if (tmp == list)
		{
			if (prev)
				prev->next = tmp->next;
			else
				org = tmp->next;
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}


int main() {
//	char *s = "Hello, World!";
//	void *test = (void *)s;
//	char *s1 = test + 2;
//	ft_printf("strlen(\"%s\") = %lu\n", s, strlen(s));
//	ft_printf("s1: %s\n", s1);
//	char *ptr = malloc(16 *sizeof(char));
//	free((void *) ptr+5);
//	abort();
	org = calloc(1, sizeof(lst));
	org->str = calloc(2, 1);
	org->str[0] = 'a';
	lst *tmp = org;
	lst *tmp2 = org;
	for (int i = 0; i < 15; i++)
	{
		lst *new = calloc(1, sizeof(lst));
		new->str = calloc(2, 1);
		new->str[0] = 'b' + (char) i;
//		if (i == 5)
//			tmp2 = new;
		tmp->next = new;
		tmp = tmp->next;
	}
	tmp = org;
	while (tmp)
	{
		ft_printf("test1 => %s %p\n", tmp->str, tmp->next);
		tmp = tmp->next;
	}
	ft_printf("\n");
	removeNode(tmp2);
//	test(tmp2);
//	lst **prevList = &org;
//	while (*prevList && (*prevList)->next != tmp2 && (*prevList)->next != NULL)
//		*prevList = (*prevList)->next;
//	ft_printf("test1 => %s %p && %s %p\n", (*prevList)->str, (*prevList)->next, tmp2->str, tmp2->next);
//	(*prevList)->next = tmp2->next;
	ft_printf("\n");
	tmp = org;
	while (tmp)
	{
		ft_printf("test2 => %s %p\n", tmp->str, tmp->next);
		tmp = tmp->next;
	}
	return 0;
}