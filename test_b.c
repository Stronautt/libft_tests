/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 16:08:49 by pgritsen          #+#    #+#             */
/*   Updated: 2017/11/08 21:27:28 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define q_ABS(num)(num < 0 ? -num : num)

#define q_INT_MAX_LEN 10

#define MB_SIZE 1048576

char	NC[] = "\033[0m",
		GREEN[] = "\033[32m",
		RED[] = "\033[31m",
		FRED[] = "\033[41m",
		YELLOW[] = "\033[33m",
		LCYAN[] = "\033[96m";

char	tmp_buffer[MB_SIZE / 1024];

char	ERRORS[2 * MB_SIZE];

static void	add_error(char	*s1, char	*s2)
{
	strlcat(ERRORS, s1, sizeof(ERRORS));
	if (s2)
		strlcat(ERRORS, s2, sizeof(ERRORS));
	strlcat(ERRORS, "\n\n", sizeof(ERRORS));
}

static char *output_get(void (*func)(const char *), const char *s)
{
	char	*buffer;
	int		out_pipe[2];
	int		saved_stdout;

	buffer = (char *)malloc(sizeof(char) * 128);
	bzero(buffer, 128);

	saved_stdout = dup(fileno(stdout)); 

	if(pipe(out_pipe) != 0)
		return (NULL);

	dup2(out_pipe[1], fileno(stdout));
	close(out_pipe[1]);

	func(s);

	read(out_pipe[0], buffer, 128);
	dup2(saved_stdout, fileno(stdout));

	return (buffer);
}

static void	*mem_replace(void *str, char sb, char t_sb, size_t n)
{
	unsigned char *s = str;

	if (!s)
		return (NULL);
	while (n--)
	{
		if (*s == sb)
			*s = t_sb;
		s++;
	}
	return (str);
}

static char	*q_strrev(char *s)
{
	char	*p_s;
	size_t	it;
	char	buff;

	if ((p_s = s) == NULL)
		return (NULL);
	while (*p_s)
		p_s++;
	it = 0;
	while (s + it < p_s--)
	{
		buff = s[it];
		s[it++] = *p_s;
		*p_s = buff;
	}
	return (s);
}

static char		*q_itoa(int n)
{
	char		*num;
	int			sign;
	int			it;
	long int	n_l;

	sign = 0;
	if (n < 0)
		sign = 1;
	if (n == 0)
		return (strdup("0"));
	if ((num = (char *)malloc(q_INT_MAX_LEN + sign + 1)) == NULL)
		return (NULL);
	it = 0;
	if (sign)
		num[it++] = '-';
	n_l = n;
	while (n_l)
	{
		num[it++] = q_ABS(n_l % 10) + '0';
		n_l /= 10;
	}
	num[it] = 0;
	num = q_strrev(num + sign);
	return (num -= sign);
}

static char *output_get_fd(void (*func)(const char *, int), const char *s)
{
	char	*buffer;
	int		out_pipe[2];
	int		saved_stdout;

	buffer = (char *)malloc(sizeof(char) * 128);
	bzero(buffer, 128);

	saved_stdout = dup(2); 

	if(pipe(out_pipe) != 0)
		return (NULL);

	dup2(out_pipe[1], 2);
	close(out_pipe[1]);

	func(s, 2);

	read(out_pipe[0], buffer, 128);
	dup2(saved_stdout, 2);

	return (buffer);
}

static int	childpid_sig(pid_t pid)
{
	int status;
	alarm(5);
	waitpid(pid, &status, NULL);
	if (status == SIGSEGV)
	{
		printf(" %s[SEGFAULT]%s ", FRED, NC);
		return (SIGSEGV);
	}
	else if (status == SIGBUS)
	{
		printf(" %s[BUSERROR]%s ", FRED, NC);
		return (SIGBUS);
	}
	else if (status)
	{
		printf(" %s[CRASH]%s ", FRED, NC);
		return (1);
	}
	return (0);
}

static size_t	q_ft_strlen(char const *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

static void	q_ft_putstr(char const *s)
{
	if (!s)
		return ;
	write(1, s, q_ft_strlen(s));
}

static void	del(void *data, size_t size)
{
	free(data);
	size = 0;
}

static void	ft_lstdel_my(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*lst;
	t_list	*buff;

	if (!alst || !del)
		return ;
	lst = *alst;
	while (lst)
	{
		del(lst->content, lst->content_size);
		buff = lst->next;
		ft_memdel((void **)&lst);
		lst = buff;
	}
	*alst = NULL;
}

static char	*ft_strnew_my(size_t size)
{
	char		*tmp;
	size_t		it;

	tmp = (char *)malloc((size + 1) * sizeof(char));
	if (tmp == NULL)
		return (NULL);
	it = 0;
	while (it <= size)
		tmp[it++] = 0;
	return (tmp);
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

static t_list *listr_c(t_list *elem)
{
	strcat(elem->content, "\t");
	return (elem);
}

static void	print(t_list *lst)
{
	strlcat(tmp_buffer, output_get(&q_ft_putstr, lst->content), sizeof(tmp_buffer));
}

static void	test_ft_lstnew()
{
	t_list *tmp;
	pid_t pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		tmp = ft_lstnew("Hello", 6);
		if (tmp != NULL && !strcmp(tmp->content, "Hello") && tmp->content_size == 6)
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[FAILED] - ft_lstnew : tmp = ft_lstnew(\"Hello\", 6) -> tmp->content : |%s|;tmp->content_size : |%d|\n", tmp->content, tmp->content_size);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_lstnew : tmp = ft_lstnew(\"Hello\", 6);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			tmp = ft_lstnew(NULL, 6);
			if (tmp->content == NULL && tmp->content_size == 0)
				printf("%s%s%s", GREEN, "[OK] ", NC);
			else
			{
				printf("%s%s%s", RED, "[FAILED] ", NC);
				printf("\n[FAILED] - ft_lstnew : tmp = ft_lstnew(NULL, 6) -> tmp->content : |%s|;tmp->content_size : |%d|\n", tmp->content, tmp->content_size);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_lstnew : tmp = ft_lstnew(NULL, 6);", NULL);
			printf("\n");
		}
	}	
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

	pid_t	pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		ft_lstdelone(&tmp, &del);
		if (tmp == NULL)
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[FAILED] - ft_lstdelone : ft_lstdelone(&tmp, &del) -> tmp : %s\n", tmp);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_lstdelone : ft_lstdelone(&tmp, &del);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			ft_lstdelone(&tmp, &del);
			if (!strcmp((*tmp2)->content, "World") && (*tmp2)->content_size == 6)
				printf("%s%s%s", GREEN, "[OK] ", NC);
			else
			{
				printf("%s%s%s", RED, "[FAILED] ", NC);
				printf("\n[FAILED] - ft_lstdelone :\nt_list *tmp2 = tmp->next;\nft_lstdelone(&tmp, &del) -> (*tmp2)->content : %s\n", (*tmp2)->content);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_lstdelone :\nt_list *tmp2 = tmp->next;\nft_lstdelone(&tmp, &del);", NULL);
			printf("\n");
		}
	}
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
	pid_t	pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		ft_lstdel(&tmp, &del);
		if (tmp == NULL)
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[FAILED] - ft_lstdel : ft_lstdel(&tmp, &del) -> tmp : %s\n", tmp);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_lstdel : ft_lstdel(&tmp, &del);", NULL);
		printf("\n");
	}
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
	pid_t	pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		ft_lstadd(&tmp, tmp2);
		if (!strcmp(tmp->content, "World"))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[FAILED] - ft_lstadd :\ntmp = (t_list *)malloc(sizeof(t_list));\ntmp->content = strdup(\"Hello\");\ntmp->content_size = 8;\ntmp->next = NULL;\nft_lstadd(&tmp, tmp2) -> tmp->content : %s\n", tmp->content);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_lstadd :\ntmp = (t_list *)malloc(sizeof(t_list));\ntmp->content = strdup(\"Hello\");\ntmp->content_size = 8;\ntmp->next = NULL;\nft_lstadd(&tmp, tmp2);", NULL);
		printf("\n");
	}
}

static void	test_ft_lstiter()
{
	t_list *tmp;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("\t|Test");
	tmp->content_size = 6;
	tmp->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->content = strdup("\t--\t");
	tmp->next->content_size = 5;
	tmp->next->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->next->content = strdup("[OK]|\n");
	tmp->next->next->content_size = 6;
	tmp->next->next->next = NULL;
	pid_t	pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		bzero(tmp_buffer, sizeof(tmp_buffer));
		ft_lstiter(tmp, &print);
		if (!strcmp(tmp_buffer, "\t|Test\t--\t[OK]|\n"))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[FAILED] - ft_lstiter : ft_lstiter(tmp, &print) -> %s\n", tmp_buffer);
		}
		bzero(tmp_buffer, sizeof(tmp_buffer));
		exit(EXIT_SUCCESS);
	}
	else
	{
		if(childpid_sig(pid_a))
			add_error("[CRASH] - ft_lstiter : ft_lstiter(tmp, &print);", NULL);
		printf("\n");
	}
}

static void	test_ft_lstmap()
{
	t_list *tmp, *tmp2;

	tmp = (t_list *)malloc(sizeof(t_list));
	tmp->content = strdup("Test");
	tmp->content_size = 6;
	tmp->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->content = strdup("--");
	tmp->next->content_size = 4;
	tmp->next->next = (t_list *)malloc(sizeof(t_list));
	tmp->next->next->content = strdup("[OK]");
	tmp->next->next->content_size = 6;
	tmp->next->next->next = NULL;

	pid_t	pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		tmp2 = ft_lstmap(tmp, &listr);
		bzero(tmp_buffer, sizeof(tmp_buffer));
		strlcat(tmp_buffer, tmp2->content, sizeof(tmp_buffer));
		strlcat(tmp_buffer, tmp2->next->content, sizeof(tmp_buffer));
		strlcat(tmp_buffer, tmp2->next->next->content, sizeof(tmp_buffer));
		if (!strcmp(tmp_buffer, "Test\t--\t[OK]\t"))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[FAILED] - ft_lstmap :\nlistr - Function what ALLOCATE MEMMORY\nft_lstmap(tmp, &listr) -> |%s|\n", tmp_buffer);
		}
		bzero(tmp_buffer, sizeof(tmp_buffer));
		exit(EXIT_SUCCESS);
	}
	else
	{
		if(childpid_sig(pid_a))
			add_error("[CRASH] - ft_lstmap :\nlistr - Function what ALLOCATE MEMMORY\nft_lstmap(tmp, &listr);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{

			tmp2 = ft_lstmap(tmp, &listr_c);
			bzero(tmp_buffer, sizeof(tmp_buffer));
			strlcat(tmp_buffer, tmp2->content, sizeof(tmp_buffer));
			strlcat(tmp_buffer, tmp2->next->content, sizeof(tmp_buffer));
			strlcat(tmp_buffer, tmp2->next->next->content, sizeof(tmp_buffer));
			if (!strcmp(tmp_buffer, "Test\t--\t[OK]\t"))
				printf("%s%s%s", GREEN, "[OK] ", NC);
			else
			{
				printf("%s%s%s", RED, "[FAILED] ", NC);
				printf("\n[FAILED] - ft_lstmap :\nlistr_c - Function what DOES NOT ALLOCATE MEMMORY\nft_lstmap(tmp, &listr_c) -> |%s|\n", tmp_buffer);
			}
			bzero(tmp_buffer, sizeof(tmp_buffer));
			exit(EXIT_SUCCESS);
		}
		else
		{
			if(childpid_sig(pid_b))
				add_error("[CRASH] - ft_lstmap :\nlistr_c - Function what DOES NOT ALLOCATE MEMMORY\nft_lstmap(tmp, &listr_c);", NULL);
			printf("\n");
		}
	}
}

static void	errors_log()
{
	int		file_desc;
	char	header[256] = "LIBFT TEST error log\nDate : ";
	struct 	tm tm = *localtime(&(time_t){time(NULL)});
	strlcat(header, asctime(&tm), sizeof(header));
	strlcat(header, "\n\n\n", sizeof(header));
	remove("errors.log");
	if ((file_desc = open("errors.log", O_WRONLY | O_APPEND | O_CREAT, 777)) < 0)
		return ;
	write(file_desc, header, sizeof(header));
	write(file_desc, ERRORS, sizeof(ERRORS));
}

int		main()
{
	printf("%s!TEST!\t--\tft_lstnew%s\t\n", LCYAN, NC);
	test_ft_lstnew();

	printf("\n%s!TEST!\t--\tft_lstdelone%s\t\n", LCYAN, NC);
	test_ft_lstdelone();

	printf("\n%s!TEST!\t--\tft_lstdel%s\t\n", LCYAN, NC);
	test_ft_lstdel();

	printf("\n%s!TEST!\t--\tft_lstadd%s\t\n", LCYAN, NC);
	test_ft_lstadd();

	printf("\n%s!TEST!\t--\tft_lstiter%s\t\n", LCYAN, NC);
	test_ft_lstiter();

	printf("\n%s!TEST!\t--\tft_lstmap%s\t\n", LCYAN, NC);
	test_ft_lstmap();

	errors_log();
	if (*ERRORS)
		printf("\n\n%sSee detailed information about segfaults in errors.log%s\n", YELLOW, NC);
}
