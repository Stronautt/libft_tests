/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 16:08:49 by pgritsen          #+#    #+#             */
/*   Updated: 2017/11/04 18:28:35 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	del(void *data, size_t size)
{
	free(data);
	size = 0;
}

static t_list *listr(t_list *elem)
{
	t_list *tmp;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = malloc(elem->content_size + 1);
	memcpy(tmp->content, strcat(elem->content, "\t"), elem->content_size + 1);
	tmp->content_size = elem->content_size;
	tmp->next = elem->next;
	return (tmp);
}

static void	print(t_list *lst)
{
	printf("%s", lst->content);
}

static void	test_ft_lstnew()
{
	t_list *tmp;

	tmp = ft_lstnew("Hello", 6);
	if (tmp != NULL && !strcmp(tmp->content, "Hello") && tmp->content_size == 6)
		printf("[OK] ");
	else
		printf("[FAILED] ");

	tmp = ft_lstnew(NULL, 6);
	if (tmp->content == NULL && tmp->content_size == 0)
		printf(" [OK] ");
	else
		printf(" [FAILED] ");
	printf("\n");
}

static void	test_ft_lstdelone()
{
	t_list *tmp, **tmp2;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("Hello");
	tmp->content_size = 6;
	tmp2 = &tmp->next;
	*tmp2 = (t_list *)malloc(sizeof(t_list));
	(*tmp2)->content = strdup("World");
	(*tmp2)->content_size = 6;
	(*tmp2)->next = NULL;

	ft_lstdelone(&tmp, &del);
	if (tmp == NULL)
		printf("[OK] ");
	else
		printf("[FAILED] ");
	if (!strcmp((*tmp2)->content, "World") && (*tmp2)->content_size == 6)
		printf(" [OK] ");
	else
		printf(" [FAILED] ");
	printf("\n");
}

static void	test_ft_lstdel()
{
	t_list *tmp, **tmp2;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("Hello");
	tmp->content_size = 6;
	tmp2 = &tmp->next;
	*tmp2 = (t_list *)malloc(sizeof(t_list));
	(*tmp2)->content = strdup("World");
	(*tmp2)->content_size = 6;
	(*tmp2)->next = NULL;

	ft_lstdel(&tmp, &del);
	if (tmp == NULL)
		printf("[OK] ");
	else
		printf("[FAILED] ");
	printf("\n");
}

static void	test_ft_lstadd()
{
	t_list *tmp, *tmp2;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("Hello");
	tmp->content_size = 8;
	tmp->next = NULL;
	tmp2 = (t_list *)malloc(sizeof(t_list));
	tmp2->content = strdup("World");
	tmp2->content_size = 5;
	tmp2->next = NULL;

	ft_lstadd(&tmp, tmp2);
	if (!strcmp(tmp->content, "World"))
		printf("[OK] ");
	else
		printf("[FAILED] ");
	printf("\n");
}

static void	test_ft_lstiter()
{
	t_list *tmp;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("\tTest");
	tmp->content_size = 6;
	tmp->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->content = strdup("\t--\t");
	tmp->next->content_size = 5;
	tmp->next->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->next->content = strdup("[OK]\n");
	tmp->next->next->content_size = 6;
	tmp->next->next->next = NULL;

	ft_lstiter(tmp, &print);

	printf("\t(If above no text \"Test    --      [OK]\", test failed)\n");
}

static void	test_ft_lstmap()
{
	t_list *tmp, *tmp2;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("\tTest");
	tmp->content_size = 7;
	tmp->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->content = strdup("--");
	tmp->next->content_size = 4;
	tmp->next->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->next->content = strdup("[OK]");
	tmp->next->next->content_size = 6;
	tmp->next->next->next = NULL;

	tmp2 = ft_lstmap(tmp, &listr);

	printf("%s%s%s\n", tmp2->content, tmp2->next->content, tmp2->next->next->content);

	printf("\t(If above no text \"Test    --      [OK]\", test failed)\n");
}

int		main()
{
	printf("!TEST!\t--\tft_lstnew\t\n");
	test_ft_lstnew();

	printf("\n!TEST!\t--\tft_lstdelone\t\n");
	test_ft_lstdelone();

	printf("\n!TEST!\t--\tft_lstdel\t\n");
	test_ft_lstdel();

	printf("\n!TEST!\t--\tft_lstadd\t\n");
	test_ft_lstadd();

	printf("\n!TEST!\t--\tft_lstiter\t\n");
	test_ft_lstiter();

	printf("\n!TEST!\t--\tft_lstmap\t\n");
	test_ft_lstmap();
}
