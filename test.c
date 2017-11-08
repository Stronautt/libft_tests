/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 18:07:22 by pgritsen          #+#    #+#             */
/*   Updated: 2017/11/08 20:54:49 by pgritsen         ###   ########.fr       */
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

/*
**		Color Library
*/

#define q_ABS(num)(num < 0 ? -num : num)

#define q_INT_MAX_LEN 10

#define MB_SIZE 1048576

char	NC[] = "\033[0m",
		GREEN[] = "\033[32m",
		RED[] = "\033[31m",
		FRED[] = "\033[41m",
		YELLOW[] = "\033[33m",
		LCYAN[] = "\033[96m";

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

static void	test_ft_putstr(void)
{
	pid_t pid_a, pid_b;
	char	*buff;
	
	pid_a = fork();
	if (pid_a == 0)
	{
		printf("\t");
		if (!(buff = output_get(&ft_putstr, "i'm putstr")))
		{
			printf("\n[ERROR] - ft_putstr : ft_putstr(\"i'm putstr\") -> %s\n", buff);
			exit(EXIT_FAILURE);
		}
		if (!strcmp(buff, "i'm putstr"))
			printf("%s[OK]%s", GREEN, NC);
		else
		{
			printf("%s[ERROR]%s", RED, NC);
			printf("\n[ERROR] - ft_putstr : ft_putstr(\"i'm putstr\") -> %s\n", buff);
		}
		free(buff);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_putstr : ft_putstr(\"i'm putstr\")", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			ft_putstr(NULL);
			printf("%s [OK]%s", GREEN, NC);
			exit(EXIT_SUCCESS);
		}
		else
			if(childpid_sig(pid_b))
				add_error("[CRASH] - ft_putstr : ft_putstr(NULL)", NULL);
	}
	printf("\n\n");
}

static void	test_ft_putendl(void)
{
	pid_t pid_a, pid_b;
	char	*buff;
	
	pid_a = fork();
	if (pid_a == 0)
	{
		printf("\t");
		if (!(buff = output_get(&ft_putendl, "i'm putendl")))
		{
			printf("\n[ERROR] - ft_putendl : ft_putendl(\"i'm putendl\") -> %s\n", buff);
			exit(EXIT_FAILURE);
		}
		if (!strcmp(buff, "i'm putendl\n"))
			printf("%s[OK]%s", GREEN, NC);
		else
		{
			printf("%s[ERROR]%s", RED, NC);
			printf("\n[ERROR] - ft_putendl : ft_putendl(\"i'm putendl\") -> %s\n", buff);			
		}
		free(buff);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_putendl : ft_putendl(\"i'm putendl\");", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			ft_putendl(NULL);
			printf("%s [OK]%s", GREEN, NC);
			exit(EXIT_SUCCESS);
		}
		else
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_putendl : ft_putendl(NULL);", NULL);
	}
	printf("\n\n");
}

static void test_ft_memset(void *(*func)(void *, int, size_t))
{
	char	s1[] = "Hello World!";
	char	s2[] = "Hello World!";
	char	s3[] = "Hello World!";
	char	s4[] = "Hello World!";
	char	ft_s1[] = "Hello World!";
	char	ft_s2[] = "Hello World!";
	char	ft_s3[] = "Hello World!";
	char	ft_s4[] = "Hello World!";
	pid_t pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		memset(s1, '\0', 4);
		func(ft_s1, '\0', 4);
		if (s1 && ft_s1 && !memcmp(s1, ft_s1, sizeof(s1)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("\n[ERROR] - ft_memset :\nchar ft_s1[] = \"Hello World!\";\nft_memset(ft_s1, '\0', 4) -> ft_s1 : %s\n",
				mem_replace(ft_s1, '\0', '0', sizeof(ft_s1) - 1));
			printf("%s%s%s", RED, "[FAILED] ", NC);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_memset :\nchar ft_s1[] = \"Hello World!\";\nft_memset(ft_s1, '\0', 4);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			memset(s2, '\200', 0);
			func(ft_s2, '\200', 0);
			if (s2 && ft_s2 && !memcmp(s2, ft_s2, sizeof(s2)))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("\n[ERROR] - ft_memset :\nchar ft_s2[] = \"Hello World!\";\nft_memset(ft_s2, '\200', 0) -> ft_s2 : %s\n", ft_s2);
				printf("%s%s%s", RED, " [FAILED] ", NC);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_memset :\nchar ft_s2[] = \"Hello World!\";\nft_memset(ft_s2, '\200', 0);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				memset(s3, 'O', sizeof(s3));
				func(ft_s3, 'O', sizeof(ft_s3));
				if (s3 && ft_s3 && !memcmp(s3, ft_s3, sizeof(s3)))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("\n[ERROR] - ft_memset :\nchar ft_s3[] = \"Hello World!\";\nft_memset(ft_s3, 'O', sizeof(ft_s3)) -> ft_s3 : %s\n", ft_s3);
					printf("%s%s%s", RED, " [FAILED] ", NC);
				}
				exit(EXIT_SUCCESS);
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_memset :\nchar ft_s3[] = \"Hello World!\";\nft_memset(ft_s3, 'O', sizeof(ft_s3));", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					memset(s4, '-', 1);
					func(ft_s4, '-', 1);
					if (!memcmp(s4, ft_s4, sizeof(s4)))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("\n[ERROR] - ft_memset :\nchar ft_s4[] = \"Hello World!\";\nft_memset(ft_s4, '-', 1) -> ft_s4 : %s\n", ft_s4);
						printf("%s%s%s", RED, " [FAILED] ", NC);
					}
					exit(EXIT_SUCCESS);
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_memset :\nchar ft_s4[] = \"Hello World!\";\nft_memset(ft_s4, '-', 1);", NULL);
					printf("\n");
				}
			}
		}		
	}
}

static void test_ft_bzero(void *(*func)(void *, size_t))
{
	char	s1[] = "Hello World!";
	char	s2[] = "Hello World!";
	char	s3[] = "Hello World!";
	char	s4[] = "Hello World!";
	char	ft_s1[] = "Hello World!";
	char	ft_s2[] = "Hello World!";
	char	ft_s3[] = "Hello World!";
	char	ft_s4[] = "Hello World!";
	pid_t pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		bzero(s1, 4);
		func(ft_s1, 4);
		if (!memcmp(s1, ft_s1, sizeof(s1)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_bzero :\nchar ft_s1[] = \"Hello World!\";\nft_bzero(ft_s1, 4) -> ft_s1 : %s\n",
				mem_replace(ft_s1, 0, '0', sizeof(ft_s1) - 1));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_bzero :\nchar ft_s1[] = \"Hello World!\";\nft_bzero(ft_s1, 4);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{			
			bzero(s2, 0);
			func(ft_s2, 0);
			if (!memcmp(s2, ft_s2, sizeof(s2)))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_bzero :\nchar ft_s2[] = \"Hello World!\";\nft_bzero(ft_s2, 0) -> ft_s2 : %s\n",
					mem_replace(ft_s2, 0, '0', sizeof(ft_s2) - 1));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_bzero :\nchar ft_s2[] = \"Hello World!\";\nft_bzero(ft_s2, 0);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{				
				bzero(s3, sizeof(s3));
				func(ft_s3, sizeof(ft_s3));
				if (!memcmp(s3, ft_s3, sizeof(s3)))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_bzero :\nchar ft_s3[] = \"Hello World!\";\nft_bzero(ft_s3, sizeof(ft_s3)) -> ft_s3 : %s\n",
						mem_replace(ft_s3, 0, '0', sizeof(ft_s3) - 1));
				}
				exit(EXIT_SUCCESS);
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_bzero :\nchar ft_s3[] = \"Hello World!\";\nft_bzero(ft_s3, sizeof(ft_s3));", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					bzero(s4, 1);
					func(ft_s4, 1);
					if (!memcmp(s4, ft_s4, sizeof(s4)))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_bzero :\nchar ft_s4[] = \"Hello World!\";\nft_bzero(ft_s4, 1) -> ft_s4 : %s\n",
							mem_replace(ft_s4, 0, '0', sizeof(ft_s4) - 1));
					}
					exit(EXIT_SUCCESS);
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_bzero :\nchar ft_s4[] = \"Hello World!\";\nft_bzero(ft_s4, 1);", NULL);
					printf("\n");
				}
			}
		}
	}
}

static struct	ft_memcpy{
	char name[40];
	int age;
}				ft_person, person, ft_person_copy, person_copy;

static void	test_ft_memcpy(void *(*func)(void *, const void *, size_t))
{
	pid_t	pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		char name[] = "Pavel G.";

		func(ft_person.name, name, strlen(name) + 1);
		ft_person.age = 42;

		memcpy(person.name, name, strlen(name) + 1);
		person.age = 42;

		if (!strcmp(ft_person.name, person.name))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, " [FAILED] ", NC);
			printf("\n[ERROR] - ft_memcpy :\nchar name[] = \"Pavel G.\";\nft_memcpy(ft_person.name, name, strlen(name) + 1) -> ft_person.name : %s\n",
				ft_person.name);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_memcpy :\nchar name[] = \"Pavel G.\";\nft_memcpy(ft_person.name, name, strlen(name) + 1);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			func(&ft_person, &ft_person_copy, sizeof(ft_person));

			memcpy(&person, &person_copy, sizeof(person));

			if (!strcmp(ft_person_copy.name, person_copy.name)
				&& ft_person_copy.age == person_copy.age)
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_memcpy :\nstruct ft_person;\nchar ft_person.name[] = \"Pavel G.\";\nft_memcpy(&ft_person, &ft_person_copy, sizeof(ft_person)) ->\nft_person_copy.name : %s\n",
					ft_person_copy.name);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_memcpy :\nstruct ft_person;\nchar ft_person.name[] = \"Pavel G.\";\nft_memcpy(&ft_person, &ft_person_copy, sizeof(ft_person));", NULL);
			printf("\n");
		}
	}
}

static void test_ft_memccpy(void *(*func)(void *, const void *, int, size_t))
{
	char	src[] = "test basic du memccpy !";
	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		char *r1, *r2;
		char buff1[22];
		char buff2[22];

		r1 = memccpy(buff1, src, 'z', 22);
		r2 = func(buff2, src, 'z', 22);

		if (r1 == r2)
			printf("%s%s%s", GREEN, " [OK] ", NC);
		else
		{
			printf("%s%s%s", RED, " [FAILED] ", NC);
			printf("\n[ERROR] - ft_memccpy :\nchar	src[] = \"test basic du memccpy !\";\nft_memccpy(buff1, src, 'z', 22) -> buff1 : %s\n", buff1);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_memccpy :\nchar	src[] = \"test basic du memccpy !\";\nft_memccpy(buff1, src, 'z', 22);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			char	buf1[] = "Ceci est un test.";
			char	buf2[200];
			void	*p1, *p2;

			p1 = memccpy(buf2, buf1, 'i', 10);
			p2 = func(buf2, buf1, 'i', 10);

			if (p1 == p2)
				printf("%s%s%s", GREEN, "[OK] ", NC);
			else
			{
				printf("%s%s%s", RED, "[FAILED] ", NC);
				printf("\n[ERROR] - ft_memccpy :\nchar	buf1[] = \"Ceci est un test.\";\nft_memccpy(buf2, buf1, 'i', 10) -> buf2 : %s\n", buf2);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_memccpy :\nchar	buf1[] = \"Ceci est un test.\";\nft_memccpy(buf2, buf1, 'i', 10);", NULL);			
			char	destination[24];
			char	ft_destination[24];

			pid_c = fork();
			if (pid_c == 0)
			{
				func(ft_destination, src, '\0', strlen(src) + 1);
				memccpy(destination, src, '\0', strlen(src) + 1);

				if (!strcmp(destination, ft_destination))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_memccpy :\nchar	src[] = \"test basic du memccpy !\";\nft_memccpy(ft_destination, src, '\0', strlen(src) + 1) -> ft_destination : %s\n", ft_destination);
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_memccpy :\nchar	src[] = \"test basic du memccpy !\";\nft_memccpy(ft_destination, src, '\0', strlen(src) + 1);", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					func(ft_destination, src, 'R', strlen(src) + 1);
					memccpy(destination, src, 'R', strlen(src) + 1);

					if (!strcmp(destination, ft_destination))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_memccpy :\nchar	src[] = \"test basic du memccpy !\";\nft_memccpy(ft_destination, src, 'R', strlen(src) + 1) -> ft_destination : %s\n", ft_destination);
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_memccpy :\nchar	src[] = \"test basic du memccpy !\";\nft_memccpy(ft_destination, src, 'R', strlen(src) + 1);", NULL);
					printf("\n");
				}
			}
		}
	}
}

static void test_ft_memmove(void *(*func)(void *, const void *, size_t))
{
	char	a[128] = "OKOKO World!";
	char	ft_a[128] = "OKOKO World!";	

	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		func(ft_a, "Hello", 5);
		memmove(a, "Hello", 5);
		if (!strncmp(a, ft_a, sizeof(a)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_memmove :\nchar	ft_a[128] = \"OKOKO World!\";\nft_memmove(ft_a, \"Hello\", 5) -> ft_a : %s\n", ft_a);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_memmove :\nchar	ft_a[128] = \"OKOKO World!\";\nft_memmove(ft_a, \"Hello\", 5);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			func(ft_a, "H\0llo", 5);
			memmove(a, "H\0llo", 5);
			if (!strncmp(a, ft_a, sizeof(a)))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_memmove :\nchar	ft_a[128] = \"Hello World!\";\nft_memmove(ft_a, \"H\0llo\", 5) -> ft_a : %s\n",
					mem_replace(ft_a, 0, '0', sizeof(ft_a) - 1));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_memmove :\nchar	ft_a[128] = \"Hello World!\";\nft_memmove(ft_a, \"H\0llo\", 5);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				char	b[] = "It's Perfect!!!";

				bzero(ft_a, sizeof(ft_a));
				bzero(a, sizeof(a));
				strcpy(ft_a, "OKOKO World!");
				strcpy(a, "OKOKO World!");
				func(ft_a, b, strlen(b) + 1);
				memmove(a, b, strlen(b) + 1);
				if (!strncmp(a, ft_a, sizeof(a)))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_memmove :\nchar	ft_a[128] = \"OKOKO World!\";\nchar	b[] = \"It's Perfect!!!\";\nft_memmove(ft_a, b, strlen(b) + 1) -> ft_a : %s\n", ft_a);
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_memmove :\nchar	ft_a[128] = \"OKOKO World!\";\nchar	b[] = \"It's Perfect!!!\";\nft_memmove(ft_a, b, strlen(b) + 1);", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					bzero(ft_a, sizeof(ft_a));
					bzero(a, sizeof(a));
					strcpy(ft_a, "It's Perfect!!!");
					strcpy(a, "It's Perfect!!!");
					func(ft_a + 15, ft_a + 4, 11);
					memmove(a + 15, a + 4, 11);
					if (!strncmp(a, ft_a, sizeof(a)))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_memmove :\nchar	ft_a[128] = \"It's Perfect!!!\";\nft_memmove(ft_a + 15, ft_a + 4, 11) -> ft_a : %s\n", ft_a);
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_memmove :\nchar	ft_a[128] = \"It's Perfect!!!\";\nft_memmove(ft_a + 15, ft_a + 4, 11);", NULL);
					printf("\n");
				}
			}
		}
	}
}

static void	test_ft_memchr(void *(*func)(const void *, int, size_t))
{
	char	*s2;
	char	*s3;
	char	*s4;
	char	*ft_p;
	char	*p;

	char	s1[] = "Example string";
	s2 = "";
	s3 = "abcdefabcdef";
	s4 = "11111111111111111111";

	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		p = (char *)memchr(s1, 'p', strlen(s1));
		ft_p = (char *)func(s1, 'p', strlen(s1));
		if (ft_p != NULL && p - s1 == ft_p - s1)
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{			
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_memchr :\nchar s1[] = \"Example string\";\nft_memchr(s1, 'p', strlen(s1)) -> %s\n", p);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_memchr :\nchar s1[] = \"Example string\";\nft_memchr(s1, 'p', strlen(s1));", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func(s2, 'x', 0) == NULL)
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_memchr :\nchar *s2 = \"\";\nft_memchr(s2, 'x', 0) -> %s\n", ft_memchr(s2, 'x', 0));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_memchr :\nchar *s2 = \"\";\nft_memchr(s2, 'x', 0);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (func(s3, 'y', 0) == NULL)
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_memchr :\nchar *s3 = \"abcdefabcdef\";\nft_memchr(s3, 'y', 0) -> %s\n", ft_memchr(s3, 'y', 0));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_memchr :\nchar *s3 = \"abcdefabcdef\";\nft_memchr(s3, 'y', 0);", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if ((char *)func(s4, '1', 20)  - s4 == 0)
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_memchr :\nchar *s4 = \"abcdefabcdef\";\nft_memchr(s4, '1', 20) -> %s\n", ft_memchr(s4, '1', 20));
					}
					exit(EXIT_SUCCESS);
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_memchr :\nchar *s4 = \"abcdefabcdef\";\nft_memchr(s4, '1', 20);", NULL);
					printf("\n");
				}
			}
		}
	}
}

static void	test_ft_memcmp(int (*func)(const void *, const void *, size_t))
{
	char	buffer1[] = "DWgaOtP12df0";
	char	buffer2[] = "DWgaOtP12DF0i";
	int		n1;
	int		n2;
	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		n1 = memcmp(buffer1, buffer2, sizeof(buffer1));
		n2 = func(buffer1, buffer2, sizeof(buffer1));

		if (n1 == n2)
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_memcmp :\nchar buffer1[] = \"DWgaOtP12df0\";\nchar	buffer2[] = \"DWgaOtP12DF0i\";\nft_memcmp(buffer1, buffer2, sizeof(buffer1)) -> %s\n", q_itoa(n2));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_memcmp :\nchar buffer1[] = \"DWgaOtP12df0\";\nchar	buffer2[] = \"DWgaOtP12DF0i\";\nft_memcmp(s4, '1', 20);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			n1 = memcmp(buffer1, buffer2, 9);
			n2 = func(buffer1, buffer2, 9);

			if (n1 == n2 && n2 == 0)
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_memcmp :\nchar buffer1[] = \"DWgaOtP12df0\";\nchar	buffer2[] = \"DWgaOtP12DF0i\";\nft_memcmp(buffer1, buffer2, 9) -> %s\n", q_itoa(n2));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_memcmp :\nchar buffer1[] = \"DWgaOtP12df0\";\nchar	buffer2[] = \"DWgaOtP12DF0i\";\nft_memcmp(buffer1, buffer2, 9);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				char buffer3[] = "lox mydak";
				char buffer4[] = "lox mydaki";

				n1 = memcmp(buffer3, buffer4, 10);
				n2 = func(buffer3, buffer4, 10);

				if (n1 == n2)
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_memcmp :\nchar buffer3[] = \"lox mydak\";\nchar buffer4[] = \"lox mydaki\";\nft_memcmp(buffer3, buffer4, 10) -> %s\n", q_itoa(n2));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_memcmp :\nchar buffer3[] = \"lox mydak\";\nchar buffer4[] = \"lox mydaki\";\nft_memcmp(buffer3, buffer4, 10);", NULL);
				printf("\n");				
			}
		}
	}
}

static void	test_ft_strlen(size_t (*func)(const char *))
{
	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (func("Hello World!") == strlen("Hello World!"))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strlen : ft_strlen(\"Hello World!\") -> %s\n", q_itoa(ft_strlen("Hello World!")));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strlen : ft_strlen(\"Hello World!\");", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func("") == strlen(""))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strlen : ft_strlen(\"\") -> %s\n", q_itoa(ft_strlen("")));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strlen : ft_strlen(\"\");", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (func("H") == strlen("H"))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strlen : ft_strlen(\"H\") -> %s\n", q_itoa(ft_strlen("H")));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strlen : ft_strlen(\"H\");", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (func("Hel\0lo World!") == strlen("Hel\0lo World!"))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strlen : ft_strlen(\"Hel\0lo World!\") -> %s\n", q_itoa(ft_strlen("Hel\0lo World!")));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_strlen : ft_strlen(\"Hel\0lo World!\");", NULL);
					printf("\n");
				}
			}
		}
	}
}

static void	test_ft_strdup(char	*(*func)(const char *))
{
	char	s[11] = "0123456789";
	char	empt[1] = {0};
	char	*d_s;
	char	*ft_d_s;
	pid_t	pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		d_s = strdup(s);
		ft_d_s = func(s);

		if (!strcmp(d_s, ft_d_s))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strdup :\nchar	s[11] = \"0123456789\";\nft_strdup(s) -> %s\n", ft_d_s);
		}
		free(d_s);
		free(ft_d_s);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strdup :\nchar	s[11] = \"0123456789\";\nft_strdup(s);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			d_s = strdup(empt);
			ft_d_s = func(empt);

			if (!strcmp(d_s, ft_d_s))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strdup :\nchar empt[1] = {0};\nft_strdup(empt) -> %s\n", ft_d_s);
			}
			free(d_s);
			free(ft_d_s);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strdup :\nchar empt[1] = {0};\nft_strdup(empt);", NULL);
			printf("\n");
		}
	}
}

static void	test_ft_strcpy(char	*(*func)(char *, const char *))
{
	char	s[] = "Copy successful!";
	char	ft_o[40];
	char	o[40];
	pid_t	pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		strcpy(o, s);
		func(ft_o, s);

		if (!strcmp(ft_o, o))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strcpy :\nchar s[] = \"Copy successful!\";\nft_strcpy(ft_o, s) -> ft_o : %s\n", ft_o);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strcpy :\nchar s[] = \"Copy successful!\";\nft_strcpy(ft_o, s);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			strcpy(o, "");
			func(ft_o, "");

			if (!strcmp(ft_o, o))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strcpy : ft_strcpy(ft_o, \"\") -> ft_o : %s\n", ft_o);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strcpy : ft_strcpy(ft_o, \"\");", NULL);
			printf("\n");
		}
	}
}

static void	test_ft_strncpy(char *(*func)(char *, const char *, size_t))
{
	char	s[] = "Copy successful!";
	char	ft_o[40];
	char	o[40];
	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		strncpy(o, s, sizeof(o));
		func(ft_o, s, sizeof(ft_o));

		if (!strncmp(ft_o, o, sizeof(o)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strncpy :\nchar s[] = \"Copy successful!\";\nft_strncpy(ft_o, s, sizeof(ft_o)) -> ft_o : %s\n", ft_o);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strncpy :\nchar s[] = \"Copy successful!\";\nft_strncpy(ft_o, s, sizeof(ft_o));", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			strncpy(o, "", 1);
			func(ft_o, "", 1);

			if (!strncmp(ft_o, o, sizeof(o)))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strncpy : ft_strcpy(ft_o, \"\", 1) -> ft_o : %s\n", ft_o);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strncpy : ft_strcpy(ft_o, \"\", 1);", NULL);
			printf("\n");
		}
	}
}

static void	test_ft_strcat(char	*(*func)(char *, const char *))
{
	char	s[80];
	char	ft_s[80];
	pid_t	pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		strcpy(s, "these ");
		strcpy(ft_s, "these ");
		strcat(s, "strings are ");
		func(ft_s, "strings are ");
		strcat(s, "concatenated.");
		func(ft_s, "concatenated.");		

		if (!strncmp(s, ft_s, sizeof(s)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strcat :\nchar ft_s[80];\nft_strcat(ft_s, \"strings are \");\nft_strcat(ft_s, \"concatenated.\") -> ft_s : %s\n", ft_s);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strcat :\nchar ft_s[80];\nft_strcat(ft_s, \"strings are \");\nft_strcat(ft_s, \"concatenated.\");", NULL);
		printf("\n");
	}	
}

static void	test_ft_strncat(char *(*func)(char *, const char *, size_t))
{
	char	s[20] = "or not to be";
	char	*ft_p;
	char	*p;
	char	ft_o[20];
	char	o[20];
	pid_t	pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		strcpy(ft_o, "To be ");
		strcpy(o, "To be ");

		p = strncat(o, s, 6);
		ft_p = func(ft_o, s, 6);
		if (!strncmp(o, ft_o, sizeof(o)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strncat :\nchar s[20] = \"or not to be\";\nstrcpy(ft_o, \"To be \");\nft_strncat(ft_o, s, 6) -> ft_o : %s\n", ft_o);
		}
		if (!strcmp(p, ft_p))
			printf("%s%s%s", GREEN, " [OK] ", NC);
		else
		{
			printf("%s%s%s", RED, " [FAILED] ", NC);
			printf("\n[ERROR] - ft_strncat :\nchar s[20] = \"or not to be\";\nstrcpy(ft_o, \"To be \");\nft_p = ft_strncat(ft_o, s, 6) -> ft_p : %s\n", ft_p);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - t_strncat :\nchar s[20] = \"or not to be\";\nstrcpy(ft_o, \"To be \");\nft_p = ft_strncat(ft_o, s, 6);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			strcpy(ft_o, "To be ");
			strcpy(o, "To be ");
			p = strncat(o, s, 12);
			ft_p = func(ft_o, s, 12);
			if (!strncmp(o, ft_o, sizeof(o)) && !strcmp(p, ft_p))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strncat :\nchar s[20] = \"or not to be\";\nstrcpy(ft_o, \"To be \");\nft_p = ft_strncat(ft_o, s, 12) -> ft_p : %s\n", ft_p);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strncat :\nchar s[20] = \"or not to be\";\nstrcpy(ft_o, \"To be \");\nft_p = ft_strncat(ft_o, s, 12);", NULL);
			printf("\n");
		}
	}
}

static void	test_ft_strlcat(size_t (*func)(char *, const char *, size_t))
{
	char	*ft_p_a;
	char	*p_a;
	char	*p_b;
	char	ft_a[128] = "Hello";
	char	a[128] = "Hello";
	char	b[] = "Hello";
	int		tmp;
	pid_t	pid_a, pid_b, pid_c, pid_d, pid_e, pid_f, pid_g, pid_h;

	ft_p_a = strdup("Hello");
	p_a = strdup("Hello");
	p_b = strdup(" World");

	pid_a = fork();
	if (pid_a == 0)
	{
		tmp = func(ft_p_a, p_b, strlen(p_b) + 1);
		if (tmp == strlcat(p_a, p_b, strlen(p_b) + 1)
			&& ft_p_a && !strcmp(ft_p_a, p_a))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, strlen(p_b) + 1) -> %s\n", q_itoa(tmp));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, strlen(p_b) + 1);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			tmp = func(ft_p_a, p_b, sizeof(ft_p_a));
			if (tmp == strlcat(p_a, p_b, sizeof(p_a))
				&& ft_p_a && !strcmp(ft_p_a, p_a))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, sizeof(ft_p_a)) -> %s\n", q_itoa(tmp));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, sizeof(ft_p_a));", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				tmp = func(ft_p_a, p_b, 1);
				if (tmp == strlcat(p_a, p_b, 1)
					&& ft_p_a && !strcmp(ft_p_a, p_a))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, 1) -> %s\n", q_itoa(tmp));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, 1);", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					tmp = func(ft_p_a, p_b, -1);
					if (tmp == strlcat(p_a, p_b, -1)
						&& ft_p_a && !strcmp(ft_p_a, p_a))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, -1) -> %s\n", q_itoa(tmp));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, -1);", NULL);
					pid_e = fork();
					if (pid_e == 0)
					{
						tmp = func(ft_a, b, 0);
						if (tmp == strlcat(a, b, 0)
							&& ft_a && !strcmp(ft_a, a))
							printf("%s%s%s", GREEN, " [OK] ", NC);
						else
						{
							printf("%s%s%s", RED, " [FAILED] ", NC);
							printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, 0) -> %s\n", q_itoa(tmp));
						}
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (childpid_sig(pid_e))
							add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_p_a, p_b, 0);", NULL);
						pid_f = fork();
						if (pid_f == 0)
						{
							tmp = func(ft_a, b, strlen(b) + strlen(ft_a) + 1);
							if (tmp == strlcat(a, b, strlen(b) + strlen(a) + 1)
								&& ft_a && !strcmp(ft_a, a))
								printf("%s%s%s", GREEN, " [OK] ", NC);
							else
							{
								printf("%s%s%s", RED, " [FAILED] ", NC);
								printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_a, b, strlen(b) + strlen(ft_a) + 1) -> %s\n", q_itoa(tmp));
							}
							exit(EXIT_SUCCESS);	
						}
						else
						{
							if (childpid_sig(pid_f))
								add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_a, b, strlen(b) + strlen(ft_a) + 1);", NULL);
							pid_g = fork();
							if (pid_g == 0)
							{
								tmp = func(ft_a, b, strlen(b) + strlen(ft_a) - 1);
								if (tmp == strlcat(a, b, strlen(b) + strlen(a) - 1)
									&& ft_a && !strcmp(ft_a, a))
									printf("%s%s%s", GREEN, " [OK] ", NC);
								else
								{
									printf("%s%s%s", RED, " [FAILED] ", NC);
									printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_a, b, strlen(b) + strlen(ft_a) - 1) -> %s\n", q_itoa(tmp));
								}
								exit(EXIT_SUCCESS);
							}
							else
							{
								if (childpid_sig(pid_g))
									add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_a, b, strlen(b) + strlen(ft_a) - 1);", NULL);
								pid_h = fork();
								if (pid_h == 0)
								{
									tmp = func(ft_a, b, strlen(b) + 1);
									if (tmp == strlcat(a, b, strlen(b) + 1)
										&& ft_a && !strcmp(ft_a, a))
										printf("%s%s%s", GREEN, " [OK] ", NC);
									else
									{
										printf("%s%s%s", RED, " [FAILED] ", NC);
										printf("\n[ERROR] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_a, b, strlen(b) + 1)) -> %s\n", q_itoa(tmp));
									}
									exit(EXIT_SUCCESS);
								}
								else
								{
									if (childpid_sig(pid_h))
										add_error("[CRASH] - ft_strlcat :\nft_p_a = strdup(\"Hello\");\np_b = strdup(\" World\");\nft_strlcat(ft_a, b, strlen(b) + 1));", NULL);
									printf("\n");
								}
							}
						}
					}					
				}
			}
		}
	}
}

static void	test_ft_strchr(char	*(*func)(const char *, int))
{
	char	ft_a[128] = "Hello World!";
	char	a[128] = "Hello World!";
	pid_t	pid_a, pid_b, pid_c, pid_d, pid_e;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (!strncmp(func(ft_a, 'o'), strchr(a, 'o'), sizeof(a)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{			
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, 'o') -> %s\n", func(ft_a, 'o'));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, 'o');", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func(ft_a, '3') == strchr(a, '3'))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, '3') -> %s\n", func(ft_a, '3'));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, '3');", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (!strncmp(func(ft_a, '!'), strchr(a, '!'), sizeof(a)))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, '!') -> %s\n", func(ft_a, '!'));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, '!');", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (!strncmp(func(ft_a, 0), strchr(a, 0), sizeof(a)))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, 0) -> %s\n", func(ft_a, 0));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ", NULL);
					pid_e = fork();
					if (pid_e == 0)
					{
						if (!strncmp(ft_a, a, sizeof(a)))
							printf("%s%s%s", GREEN, " [OK] ", NC);
						else
						{
							printf("%s%s%s", RED, " [FAILED] ", NC);
							printf("\n[ERROR] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, '0') -> ft_a : %s\n", ft_a);
						}
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (childpid_sig(pid_e))
							add_error("[CRASH] - ft_strchr :\nchar ft_a[128] = \"Hello World!\";\nft_strchr(ft_a, '0');", NULL);
						printf("\n");
					}					
				}
			}
		}
	}
}

static void	test_ft_strrchr(char *(*func)(const char *, int))
{
	char	ft_a[128] = "Hello World!";
	char	a[128] = "Hello World!";
	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (!strncmp(func(ft_a, 'o'), strrchr(a, 'o'), sizeof(a)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, 'o') -> %s\n", func(ft_a, 'o'));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, 'o');", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func(ft_a, '3') == strrchr(a, '3'))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, '3') -> %s\n", func(ft_a, '3'));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, '3');", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (!strncmp(func(ft_a, '!'), strrchr(a, '!'), sizeof(a)))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, '!');", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (!strncmp(func(ft_a, 0), strrchr(a, 0), sizeof(a)))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, 0) -> %s\n", func(ft_a, 0));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_strrchr :\nchar ft_a[128] = \"Hello World!\";\nft_strrchr(ft_a, 0);", NULL);
					printf("\n");
				}
			}
		}
	}
}

static void	test_ft_strstr(char *(*func)(const char *, const char *))
{
	const char *largestring = "Foo Bar Baz";
	pid_t	pid_a, pid_b, pid_c, pid_d;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (func(largestring, "Bar") && !strcmp(func(largestring, "Bar"), strstr(largestring, "Bar")))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"Bar\") -> %s\n", func(largestring, "Bar"));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"Bar\");", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (!strcmp(strstr(largestring, ""), func(largestring, "")))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"\") -> %s\n", func(largestring, ""));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"\");", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (func(largestring, "QW") == NULL
					&& strstr(largestring, "QW") == NULL)
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"QW\") -> %s\n", func(largestring, "QW"));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"QW\");", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (!strcmp(func(largestring, "Ba"), strstr(largestring, "Ba")))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"Ba\") -> %s\n", func(largestring, "Ba"));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_strstr :\nconst char *largestring = \"Foo Bar Baz\";\nft_strstr(largestring, \"Ba\");", NULL);
					printf("\n");
				}
			}
		}
	}
}

static void	test_ft_strnstr(char *(*func)(const char *, const char *, size_t))
{
	const char largestring[] = "MZIRIBMZIRIBMZE123";
	pid_t	pid_a, pid_b, pid_c;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (func(largestring, "MZIRIBMZE", -1) && !strcmp(func(largestring, "MZIRIBMZE", -1), strnstr(largestring, "MZIRIBMZE", -1))
			&& func(largestring, "MZIRIBMZE", 7) == strnstr(largestring, "MZIRIBMZE", 7))
			printf("%s%s%s", GREEN, "[OK]", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED]", NC);
			printf("\n[ERROR] - ft_strnstr :\nconst char largestring[] = \"MZIRIBMZIRIBMZE123\";\nft_strnstr(largestring, \"MZIRIBMZE\", -1) -> %s\n", func(largestring, "MZIRIBMZE", -1));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strnstr :\nconst char largestring[] = \"MZIRIBMZIRIBMZE123\";\nft_strnstr(largestring, \"MZIRIBMZE\", -1);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			for (int i = 1; i < 10; i++)
				if (func(largestring, "MZIRIBMZE", i) == strnstr(largestring, "MZIRIBMZE", i))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strnstr :\nconst char largestring[] = \"MZIRIBMZIRIBMZE123\";\nft_strnstr(largestring, \"MZIRIBMZE\", %d[1 - 9]) -> %s\n", func(largestring, "MZIRIBMZE", i));
				}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strnstr :\nconst char largestring[] = \"MZIRIBMZIRIBMZE123\";\nft_strnstr(largestring, \"MZIRIBMZE\", %d[1 - 9]);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				char	buf[10];

				bzero(buf, 10);
				strcpy(buf, "un deux 9");
				buf[9] = '6';
				buf[1] = 0;
				if(func(buf, "deux", 10) == strnstr(buf, "deux", 10))
					printf("%s%s%s", GREEN, "[OK]", NC);
				else
				{
					printf("%s%s%s", RED, "[FAILED]", NC);
					printf("\n[ERROR] - ft_strnstr :\nchar	buf[10] = \"un deux 9\";\nft_strnstr(buf, \"deux\", 10) -> %s\n", func(buf, "deux", 10));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strnstr :\nchar	buf[10] = \"un deux 9\";\nft_strnstr(buf, \"deux\", 10);", NULL);
				printf("\n");
			}
		}
	}
}

static void	test_ft_strcmp(int (*func)(const char *, const char *))
{
	char	a[] = "Hello World";
	char	b[] = "hello Wolrd";
	char	c[] = "Hello WolrD";
	char	d[] = "Hello";
	char	e[] = "hello";
	char	f[] = "Hello Wolrd ";
	char	g[] = "Hello Wolr\200";
	pid_t	pid_a, pid_b, pid_c, pid_d, pid_e, pid_f;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (func(a, b) == strcmp(a, b))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strcmp : ft_strcmp(\"Hello World\", \"hello Wolrd\") -> %s\n", q_itoa(func(a, b)));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strcmp : ft_strcmp(\"Hello World\", \"hello Wolrd\");", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func(a, c) == strcmp(a, c))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello WolrD\") -> %s\n", q_itoa(func(a, c)));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello WolrD\");", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (func(a, d) == strcmp(a, d))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello\") -> %s\n", q_itoa(func(a, d)));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello\");", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (func(a, e) == strcmp(a, e))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strcmp : ft_strcmp(\"Hello World\", \"hello\") -> %s\n", q_itoa(func(a, e)));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_strcmp : ft_strcmp(\"Hello World\", \"hello\");", NULL);
					pid_e = fork();
					if (pid_e == 0)
					{
						if (func(a, f) == strcmp(a, f))
							printf("%s%s%s", GREEN, " [OK] ", NC);
						else
						{
							printf("%s%s%s", RED, " [FAILED] ", NC);
							printf("\n[ERROR] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello Wolrd \") -> %s\n", q_itoa(func(a, f)));
						}						
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (childpid_sig(pid_e))
							add_error("[CRASH] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello Wolrd \");", NULL);
						pid_f = fork();
						if (pid_f == 0)
						{
							if (func(a, g) == strcmp(a, g))
								printf("%s%s%s", GREEN, " [OK] ", NC);
							else
							{
								printf("%s%s%s", RED, " [FAILED] ", NC);
								printf("\n[ERROR] - ft_strcmp : ft_strcmp(\"Hello World\", \"Hello Wolr\200\") -> %s\n", q_itoa(func(a, g)));
							}
							exit(EXIT_SUCCESS);	
						}
						else
						{
							if (childpid_sig(pid_f))
								add_error("[CRASH] - ft_strcmp(\"Hello World\", \"Hello Wolr\200\");", NULL);
							printf("\n");
						}
					}					
				}
			}
		}
	}
}

static void	test_ft_strncmp(int (*func)(const char *, const char *, size_t))
{
	char	a[] = "Hello World";
	char	b[] = "hello Wolrd";
	char	c[] = "Hello WolrD";
	char	d[] = "Hello";
	char	e[] = "hello";
	char	f[] = "Hello Wolrd ";
	char	g[] = "Hello Wolr\200";

	pid_t	pid_a, pid_b, pid_c, pid_d, pid_e, pid_f;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (func(a, b, sizeof(a)) == strncmp(a, b, sizeof(a)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strncmp : ft_strncmp(\"Hello World\", \"hello Wolrd\", 12) -> %s\n", q_itoa(func(a, b, sizeof(a))));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strncmp : ft_strncmp(\"Hello World\", \"hello Wolrd\", 12);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func(a, c, 0) == strncmp(a, c, 0))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello WolrD\", 0) -> %s\n", q_itoa(func(a, c, 0)));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello WolrD\", 0);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (func(a, d, 12) == strncmp(a, d, 12))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello\", 12) -> %s\n", q_itoa(func(a, d, 12)));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello\", 12);", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (func(a, e, 6) == strncmp(a, e, 6))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_strncmp : ft_strncmp(\"Hello World\", \"hello\", 6) -> %s\n", q_itoa(func(a, e, 6)));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_strncmp : ft_strncmp(\"Hello World\", \"hello\", 6);", NULL);
					pid_e = fork();
					if (pid_e == 0)
					{
						if (func(a, f, 13) == strncmp(a, f, 13))
							printf("%s%s%s", GREEN, " [OK] ", NC);
						else
						{
							printf("%s%s%s", RED, " [FAILED] ", NC);
							printf("\n[ERROR] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello Wolrd \", 13) -> %s\n", q_itoa(func(a, f, 13)));
						}						
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (childpid_sig(pid_e))
							add_error("[CRASH] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello Wolrd \", 13);", NULL);
						pid_f = fork();
						if (pid_f == 0)
						{
							if (func(a, c, -1) == strncmp(a, c, -1))
								printf("%s%s%s", GREEN, " [OK] ", NC);
							else
							{
								printf("%s%s%s", RED, " [FAILED] ", NC);
								printf("\n[ERROR] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello WolrD\", -1) -> %s\n", q_itoa(func(a, c, -1)));
							}
							exit(EXIT_SUCCESS);	
						}
						else
						{
							if (childpid_sig(pid_f))
								add_error("[CRASH] - ft_strncmp : ft_strncmp(\"Hello World\", \"Hello WolrD\", -1);", NULL);
							printf("\n");
						}
					}					
				}
			}
		}
	}
}

static void	test_ft_atoi(int (*func)(const char *))
{
	pid_t	pid_a, pid_b, pid_c, pid_d, pid_e, pid_f, pid_g, pid_h, pid_l, pid_m, pid_n;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (func("   1425") == atoi("   1425"))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_atoi : ft_atoi(\"   1425\") -> %s\n", q_itoa(func("   1425")));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_atoi : ft_atoi(\"   1425\");", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (func("12s1425") == atoi("12s1425"))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_atoi : ft_atoi(\"12s1425\") -> %s\n", q_itoa(func("12s1425")));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_atoi : ft_atoi(\"12s1425\");", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (func("") == atoi(""))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_atoi : ft_atoi(\"\") -> %s\n", q_itoa(func("")));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_atoi : ft_atoi(\"\");", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (func("     \t1425sad213") == atoi("     \t1425sad213"))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_atoi : ft_atoi(\"     \t1425sad213\") -> %s\n", q_itoa(func("     \t1425sad213")));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_atoi : ft_atoi(\"     \t1425sad213\");", NULL);
					pid_e = fork();
					if (pid_e == 0)
					{
						if (func(" 2147483647 ") == atoi(" 2147483647 "))
							printf("%s%s%s", GREEN, " [OK] ", NC);
						else
						{
							printf("%s%s%s", RED, " [FAILED] ", NC);
							printf("\n[ERROR] - ft_atoi : ft_atoi(\" 2147483647 \") -> %s\n", q_itoa(func(" 2147483647 ")));
						}
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (childpid_sig(pid_e))
							add_error("[CRASH] - ft_atoi : ft_atoi(\" 2147483647 \");", NULL);
						pid_f = fork();
						if (pid_f == 0)
						{
							if (func("-2147483648") == atoi("-2147483648"))
								printf("%s%s%s", GREEN, " [OK] ", NC);
							else
							{
								printf("%s%s%s", RED, " [FAILED] ", NC);
								printf("\n[ERROR] - ft_atoi : ft_atoi(\"-2147483648\") -> %s\n", q_itoa(func("-2147483648")));
							}
							exit(EXIT_SUCCESS);	
						}
						else
						{
							if (childpid_sig(pid_f))
								add_error("[CRASH] - ft_atoi : ft_atoi(\"-2147483648\");", NULL);
							pid_g = fork();
							if (pid_g == 0)
							{
								if (func("+2147483648") == atoi("+2147483648"))
									printf("%s%s%s", GREEN, " [OK] ", NC);
								else
								{
									printf("%s%s%s", RED, " [FAILED] ", NC);
									printf("\n[ERROR] - ft_atoi : ft_atoi(\"+2147483648\") -> %s\n", q_itoa(func("+2147483648")));
								}
								exit(EXIT_SUCCESS);
							}
							else
							{
								if (childpid_sig(pid_g))
									add_error("[CRASH] - ft_atoi : ft_atoi(\"+2147483648\");", NULL);
								pid_h = fork();
								if (pid_h == 0)
								{
									if (func("18446744073709551615") == atoi("18446744073709551615"))
										printf("%s%s%s", GREEN, " [OK] ", NC);
									else
									{
										printf("%s%s%s", RED, " [FAILED] ", NC);
										printf("\n[ERROR] - ft_atoi : ft_atoi(\"9223372036854775807\") -> %s\n", q_itoa(func("9223372036854775807")));
									}
									exit(EXIT_SUCCESS);
								}
								else
								{
									if (childpid_sig(pid_h))
										add_error("[CRASH] - ft_atoi : ft_atoi(\"9223372036854775807\");", NULL);
									pid_l = fork();
									if (pid_l == 0)
									{
										if (func("18446744073709551616") == atoi("18446744073709551616"))
											printf("%s%s%s", GREEN, " [OK] ", NC);
										else
										{
											printf("%s%s%s", RED, " [FAILED] ", NC);
											printf("\n[ERROR] - ft_atoi : ft_atoi(\"18446744073709551616\") -> %s \n", q_itoa(func("18446744073709551616")));
										}
										exit(EXIT_SUCCESS);
									}
									else
									{
										if (childpid_sig(pid_l))
											add_error("[CRASH] - ft_atoi : ft_atoi(\"9223372036854775808\");", NULL);
										pid_m = fork();
										if (pid_m == 0)
										{
											if (func("922337203685477580712125325543357343463643464634633466344366323523959359238")
												== atoi("922337203685477580712125325543357343463643464634633466344366323523959359238"))
												printf("%s%s%s", GREEN, " [OK] ", NC);
											else
											{
												printf("%s%s%s", RED, " [FAILED] ", NC);
												printf("\n[ERROR] - ft_atoi : ft_atoi(\"922337203685477580712125325543357343463643464634633466344366323523959359238\") -> %s\n", q_itoa(func("922337203685477580712125325543357343463643464634633466344366323523959359238")));
											}
											exit(EXIT_SUCCESS);
										}
										else
										{
											if (childpid_sig(pid_m))
												add_error("[CRASH] - ft_atoi : ft_atoi(\"922337203685477580712125325543357343463643464634633466344366323523959359238\");", NULL);
											pid_n = fork();
											if (pid_n == 0)
											{
												if (func("-922337203685477580712125325543357343463643464634633466344366323523959359238")
													== atoi("-922337203685477580712125325543357343463643464634633466344366323523959359238"))
													printf("%s%s%s", GREEN, " [OK] ", NC);
												else
												{
													printf("%s%s%s", RED, " [FAILED] ", NC);
													printf("\n[ERROR] - ft_atoi : ft_atoi(\"-922337203685477580712125325543357343463643464634633466344366323523959359238\") -> %s\n", q_itoa(func("-922337203685477580712125325543357343463643464634633466344366323523959359238")));
												}
												exit(EXIT_SUCCESS);
											}
											else
											{
												if (childpid_sig(pid_n))
													add_error("[CRASH] - ft_atoi : ft_atoi(\"-922337203685477580712125325543357343463643464634633466344366323523959359238\");", NULL);
												printf("\n");
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

static void	test_ft_strnew()
{
	char	a[] = "\0\0\0\0\0";
	char	*ft_a;
	pid_t pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		ft_a = ft_strnew(5);
		if (!strncmp(a, ft_a, sizeof(a)))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strnew : ft_strnew(5) -> %s\n", mem_replace(ft_strnew(5), 0, '0', 4));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strnew : ft_strnew(5);", NULL);
		printf("\n");
	}
}

static void	test_ft_strjoin()
{
	char	s1[] = "where is my ";
	char	s2[] = "malloc ???";
	char	s3[] = "where is my malloc ???";
	char	*p_s;
	pid_t pid_a, pid_b;

	pid_a = fork();
	if (pid_a == 0)
	{
		p_s = ft_strjoin(s1, s2);

		if (p_s && !strcmp(s1, "where is my ") && !strcmp(p_s, s3))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strjoin :\nchar	s1[] = \"where is my \";\nchar	s2[] = \"malloc ???\";\nft_strjoin(s1, s2) ->  %s\n", p_s);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strjoin :\nchar	s1[] = \"where is my \";\nchar	s2[] = \"malloc ???\";\nft_strjoin(s1, s2);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			ft_strjoin(NULL, s2);
			printf("%s%s%s", GREEN, " [OK] ", NC);
			ft_strjoin(s1, NULL);
			printf("%s%s%s", GREEN, " [OK] ", NC);
			ft_strjoin(NULL, NULL);
			printf("%s%s%s", GREEN, " [OK] ", NC);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_strjoin : Not Protected for NULL data entry!", NULL);
			printf("\n");		
		}
	}
}

static void	test_ft_strtrim()
{
	char *s1 = "\t   \n\n\n  \n\n\t    Hello \t  Please\n Trim me !\t\t\t\n  \t\t\t\t  ";
	char *s2 = "Hello \t  Please\n Trim me !";
	char *s3;
	int r_size = strlen(s2);
	int size;
	pid_t pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		s3 = ft_strtrim(s1);
		if (!memcmp(s2, s3, r_size + 1))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strtrim : ft_strtrim(\"\t   \n\n\n  \n\n\t    Hello \t  Please\n Trim me !\t\t\t\n  \t\t\t\t  \") ->  %s\n", s3);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strtrim : ft_strtrim(\"\t   \n\n\n  \n\n\t    Hello \t  Please\n Trim me !\t\t\t\n  \t\t\t\t  \");", NULL);
		printf("\n");
	}
}

static void	test_ft_strsplit()
{
	char	test[3][6] = { {'H', 'e', 'l', 'l', 'o', 0}, {'W', 'o', 'r', 'l', 'd', 0}, 0};
	char	**ft_test;
	pid_t pid_a;

	pid_a = fork();
	if (pid_a == 0)
	{
		ft_test = ft_strsplit("((((((((((((Hello((((((((((((((((((((((((((World((((((((((((((((((((", '(');
		for (int i = 0; i < 2; i++)
			if (strcmp(ft_test[i], test[i]))
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_strsplit : ft_strsplit(\"((((((((((((Hello((((((((((((((((((((((((((World((((((((((((((((((((\", '(') %s\n", NULL);
				exit(EXIT_SUCCESS);
			}
		if ((int)ft_test[2] == 0)
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_strsplit : ft_strsplit(\"((((((((((((Hello((((((((((((((((((((((((((World((((((((((((((((((((\", '(') %s\n", NULL);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_strsplit : Not NULL terminated list of words or BAD OUTPUT", NULL);
		printf("\n");
	}
}

static void	test_ft_itoa()
{
	pid_t	pid_a, pid_b, pid_c, pid_d, pid_e, pid_f;

	pid_a = fork();
	if (pid_a == 0)
	{
		if (!strcmp(ft_itoa(123), "123"))
			printf("%s%s%s", GREEN, "[OK] ", NC);
		else
		{
			printf("%s%s%s", RED, "[FAILED] ", NC);
			printf("\n[ERROR] - ft_itoa : ft_itoa(123) -> %s\n", ft_itoa(123));
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (childpid_sig(pid_a))
			add_error("[CRASH] - ft_itoa : ft_itoa(123);", NULL);
		pid_b = fork();
		if (pid_b == 0)
		{
			if (!strcmp(ft_itoa(2147483647), "2147483647"))
				printf("%s%s%s", GREEN, " [OK] ", NC);
			else
			{
				printf("%s%s%s", RED, " [FAILED] ", NC);
				printf("\n[ERROR] - ft_itoa : ft_itoa(2147483647) -> %s\n", ft_itoa(2147483647));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (childpid_sig(pid_b))
				add_error("[CRASH] - ft_itoa : ft_itoa(2147483647);", NULL);
			pid_c = fork();
			if (pid_c == 0)
			{
				if (!strcmp(ft_itoa(-2), "-2"))
					printf("%s%s%s", GREEN, " [OK] ", NC);
				else
				{
					printf("%s%s%s", RED, " [FAILED] ", NC);
					printf("\n[ERROR] - ft_itoa : ft_itoa(-2) -> %s\n", ft_itoa(-2));
				}
				exit(EXIT_SUCCESS);			
			}
			else
			{
				if (childpid_sig(pid_c))
					add_error("[CRASH] - ft_itoa : ft_itoa(-2);", NULL);
				pid_d = fork();
				if (pid_d == 0)
				{
					if (!strcmp(ft_itoa(-2147483648), "-2147483648"))
						printf("%s%s%s", GREEN, " [OK] ", NC);
					else
					{
						printf("%s%s%s", RED, " [FAILED] ", NC);
						printf("\n[ERROR] - ft_itoa : ft_itoa(-2147483648) -> %s\n", ft_itoa(-2147483648));
					}
					exit(EXIT_SUCCESS);	
				}
				else
				{
					if (childpid_sig(pid_d))
						add_error("[CRASH] - ft_itoa : ft_itoa(-2147483648);", NULL);
					pid_e = fork();
					if (pid_e == 0)
					{
						if (!strcmp(ft_itoa(0), "0"))
							printf("%s%s%s", GREEN, " [OK] ", NC);
						else
						{
							printf("%s%s%s", RED, " [FAILED] ", NC);
							printf("\n[ERROR] - ft_itoa : ft_itoa(0) -> %s\n", ft_itoa(0));
						}
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (childpid_sig(pid_e))
							add_error("[CRASH] - ft_itoa : ft_itoa(0);", NULL);
						pid_f = fork();
						if (pid_f == 0)
						{
							if (!strcmp(ft_itoa(489652), "489652"))
								printf("%s%s%s", GREEN, " [OK] ", NC);
							else
							{
								printf("%s%s%s", RED, " [FAILED] ", NC);
								printf("\n[ERROR] - ft_itoa : ft_itoa(489652) -> %s\n", ft_itoa(489652));
							}
							exit(EXIT_SUCCESS);	
						}
						else
						{
							if (childpid_sig(pid_f))
								add_error("[CRASH] - ft_itoa : ft_itoa(489652);", NULL);
							printf("\n");
						}
					}					
				}
			}
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

int		main(void)
{
    printf("%s!TEST!\t--\tft_putstr%s\n", LCYAN, NC);
    test_ft_putstr();
	
	printf("%s!TEST!\t--\tft_putendl%s\n", LCYAN, NC);
	test_ft_putendl();

	printf("%s%s%s", LCYAN, "!TEST!\t--\tft_memset\n", NC);
	printf("\tResults from MEMSET (STRING.H) below:\n\t");
	test_ft_memset(memset);
	printf("\tResults from FT_MEMSET (YOURS) below:\n\t");
	test_ft_memset(ft_memset);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_bzero\n", NC);
	printf("\tResults from BZERO (STRING.H) below:\n\t");
	test_ft_bzero(bzero);
	printf("\tResults from FT_BZERO (YOURS) below:\n\t");
	test_ft_bzero(ft_bzero);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_memcpy\n", NC);
	printf("\tResults from MEMCPY (STRING.H) below:\n\t");
	test_ft_memcpy(memcpy);
	printf("\tResults from FT_MEMCPY (YOURS) below:\n\t");
	test_ft_memcpy(ft_memcpy);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_memccpy\n", NC);
	printf("\tResults from MEMCCPY (STRING.H) below:\n\t");
	test_ft_memccpy(memccpy);
	printf("\tResults from FT_MEMCCPY (YOURS) below:\n\t");
	test_ft_memccpy(ft_memccpy);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_memcmp\n", NC);
	printf("\tResults from MEMCMP (STRING.H) below:\n\t");
	test_ft_memcmp(memcmp);
	printf("\tResults from FT_MEMCMP (YOURS) below:\n\t");
	test_ft_memcmp(ft_memcmp);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_memmove\n", NC);
	printf("\tResults from MEMMOVE (STRING.H) below:\n\t");
	test_ft_memmove(memmove);
	printf("\tResults from FT_MEMMOVE (YOURS) below:\n\t");
	test_ft_memmove(ft_memmove);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_memchr\n", NC);
	printf("\tResults from MEMCHR (STRING.H) below:\n\t");
	test_ft_memchr(memchr);
	printf("\tResults from FT_MEMCHR (YOURS) below:\n\t");
	test_ft_memchr(ft_memchr);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strlen\n", NC);
	printf("\tResults from STRLEN (STRING.H) below:\n\t");
	test_ft_strlen(strlen);
	printf("\tResults from FT_STRLEN (YOURS) below:\n\t");
	test_ft_strlen(ft_strlen);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strdup\n", NC);
	printf("\tResults from STRDUP (STRING.H) below:\n\t");
	test_ft_strdup(strdup);
	printf("\tResults from FT_STRDUP (YOURS) below:\n\t");
	test_ft_strdup(ft_strdup);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strcpy\n", NC);
	printf("\tResults from STRCPY (STRING.H) below:\n\t");
	test_ft_strcpy(strcpy);
	printf("\tResults from FT_STRCPY (YOURS) below:\n\t");
	test_ft_strcpy(ft_strcpy);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strncpy\n", NC);
	printf("\tResults from STRNCPY (STRING.H) below:\n\t");
	test_ft_strncpy(strncpy);
	printf("\tResults from FT_STRNCPY (YOURS) below:\n\t");
	test_ft_strncpy(ft_strncpy);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strcat\n", NC);
	printf("\tResults from STRCAT (STRING.H) below:\n\t");
	test_ft_strcat(strcat);
	printf("\tResults from FT_STRCAT (YOURS) below:\n\t");
	test_ft_strcat(ft_strcat);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strncat\n", NC);
	printf("\tResults from STRNCAT (STRING.H) below:\n\t");
	test_ft_strncat(strncat);
	printf("\tResults from FT_STRNCAT (YOURS) below:\n\t");
	test_ft_strncat(ft_strncat);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strlcat\n", NC);
	printf("\tResults from STRLCAT (STRING.H) below:\n\t");
	test_ft_strlcat(strlcat);
	printf("\tResults from FT_STRLCAT (YOURS) below:\n\t");
	test_ft_strlcat(ft_strlcat);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strchr\n", NC);
	printf("\tResults from STRCHR (STRING.H) below:\n\t");
	test_ft_strchr(strchr);
	printf("\tResults from FT_STRCHR (YOURS) below:\n\t");
	test_ft_strchr(ft_strchr);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strrchr\n", NC);
	printf("\tResults from STRRCHR (STRING.H) below:\n\t");
	test_ft_strrchr(strrchr);
	printf("\tResults from FT_STRRCHR (YOURS) below:\n\t");
	test_ft_strrchr(ft_strrchr);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strcmp\n", NC);
	printf("\tResults from STRCMP (STRING.H) below:\n\t");
	test_ft_strcmp(strcmp);
	printf("\tResults from FT_STRCMP (YOURS) below:\n\t");
	test_ft_strcmp(ft_strcmp);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strncmp\n", NC);
	printf("\tResults from STRNCMP (STRING.H) below:\n\t");
	test_ft_strncmp(strncmp);
	printf("\tResults from FT_STRNCMP (YOURS) below:\n\t");
	test_ft_strncmp(ft_strncmp);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strstr\n", NC);
	printf("\tResults from STRSTR (STRING.H) below:\n\t");
	test_ft_strstr(strstr);
	printf("\tResults from FT_STRSTR (YOURS) below:\n\t");
	test_ft_strstr(ft_strstr);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strnstr\n", NC);
	printf("\tResults from STRNSTR (STRING.H) below:\n\t");
	test_ft_strnstr(strnstr);
	printf("\tResults from FT_STRNSTR (YOURS) below:\n\t");
	test_ft_strnstr(ft_strnstr);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_atoi\n", NC);
	printf("\tResults from ATOI (STRING.H) below:\n\t");
	test_ft_atoi(atoi);
	printf("\tResults from FT_ATOI (YOURS) below:\n\t");
	test_ft_atoi(ft_atoi);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strnew\n\t", NC);
	test_ft_strnew();

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strjoin\n\t", NC);
	test_ft_strjoin();

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strtrim\n\t", NC);
	test_ft_strtrim();

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_strsplit\n\t", NC);
	test_ft_strsplit();

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_itoa\n\t", NC);
	test_ft_itoa();

	errors_log();
	if (*ERRORS)
		printf("\n\n%sSee detailed information about segfaults in errors.log%s\n", YELLOW, NC);

	return (0);
}
