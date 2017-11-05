/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgritsen <pgritsen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/25 18:07:22 by pgritsen          #+#    #+#             */
/*   Updated: 2017/11/05 13:49:11 by pgritsen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
**		Color Library
*/

char	NC[] = "\033[0m",
		GREEN[] = "\033[32m",
		RED[] = "\033[31m",
		YELLOW[] = "\033[33m",
		LCYAN[] = "\033[96m";

static void	test_ft_putstr(void)
{
	printf("%s!TEST!\t--\tft_putstr%s\n", LCYAN, GREEN);
	ft_putstr("\t[OK]");
	printf("%s\n", NC);
}

static void	test_ft_putendl(void)
{
	printf("%s!TEST!\t--\tft_putendl%s\n", LCYAN, GREEN);
	ft_putendl("\t[OK]");
	printf("%s\n", NC);
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

	memset(s1, '\0', 4);
	func(ft_s1, '\0', 4);
	if (!memcmp(s1, ft_s1, sizeof(s1)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	memset(s2, '\200', 0);
	func(ft_s2, '\200', 0);
	if (!memcmp(s2, ft_s2, sizeof(s2)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	memset(s3, 'O', sizeof(s3));
	func(ft_s3, 'O', sizeof(ft_s3));
	if (!memcmp(s3, ft_s3, sizeof(s3)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	memset(s4, '-', 1);
	func(ft_s4, '-', 1);
	if (!memcmp(s4, ft_s4, sizeof(s4)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
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

	bzero(s1, 4);
	func(ft_s1, 4);
	if (!memcmp(s1, ft_s1, sizeof(s1)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	bzero(s2, 0);
	func(ft_s2, 0);
	if (!memcmp(s2, ft_s2, sizeof(s2)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	bzero(s3, sizeof(s3));
	func(ft_s3, sizeof(ft_s3));
	if (!memcmp(s3, ft_s3, sizeof(s3)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	bzero(s4, 1);
	func(ft_s4, 1);
	if (!memcmp(s4, ft_s4, sizeof(s4)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static struct	ft_memcpy{
	char name[40];
	int age;
}				ft_person, person, ft_person_copy, person_copy;

static void	test_ft_memcpy(void *(*func)(void *, const void *, size_t))
{
	char name[] = "Pavel G.";

	func(ft_person.name, name, strlen(name) + 1);
	ft_person.age = 42;

	memcpy(person.name, name, strlen(name) + 1);
	person.age = 42;

	if (!strcmp(ft_person.name, person.name))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	func(&ft_person, &ft_person_copy, sizeof(ft_person));

	memcpy(&person, &person_copy, sizeof(person));

	if (!strcmp(ft_person_copy.name, person_copy.name)
		&& ft_person_copy.age == person_copy.age)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void test_ft_memccpy(void *(*func)(void *, const void *, int, size_t))
{
	char	destination[24];
	char	ft_destination[24];
	char	src[] = "test basic du memccpy !";

	char *r1, *r2;
	char buff1[22];
	char buff2[22];

	r1 = memccpy(buff1, src, 'z', 22);
	r2 = func(buff2, src, 'z', 22);

	char	buf1[] = "Ceci est un test.";
	char	buf2[200];
	void	*p1, *p2;

	p1 = memccpy(buf2, buf1, 'i', 10);
	p2 = ft_memccpy(buf2, buf1, 'i', 10);

	if (p1 == p2)
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	if (r1 == r2)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	func(ft_destination, src, '\0', strlen(src) + 1);
	memccpy(destination, src, '\0', strlen(src) + 1);

	if (!strcmp(destination, ft_destination))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	func(ft_destination, src, 'R', strlen(src) + 1);
	memccpy(destination, src, 'R', strlen(src) + 1);

	if (!strcmp(destination, ft_destination))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void test_ft_memmove(void *(*func)(void *, const void *, size_t))
{
	char	a[128] = "OKOKO World!";
	char	ft_a[128] = "OKOKO World!";
	char	b[] = "It's Perfect!!!";

	func(ft_a, "Hello", 5);
	memmove(a, "Hello", 5);
	if (!strncmp(a, ft_a, sizeof(a)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	func(ft_a, "H\0llo", 5);
	memmove(a, "H\0llo", 5);
	if (!strncmp(a, ft_a, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	func(ft_a, b, strlen(b) + 1);
	memmove(a, b, strlen(b) + 1);
	if (!strncmp(a, ft_a, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	func(ft_a + 15, ft_a + 4, 11);
	memmove(a + 15, a + 4, 11);
	if (!strncmp(a, ft_a, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	int		dst, ft_dst, src = 2147483647;

	func(&dst, &src, 4);
	memmove(&ft_dst, &src, 4);
	if (dst == ft_dst)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
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

	p = (char *)memchr(s1, 'p', strlen(s1));
	ft_p = (char *)func(s1, 'p', strlen(s1));
	if (ft_p != NULL && p - s1 == ft_p - s1)
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func(s2, 'x', 0) == NULL)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(s3, 'y', 0) == NULL)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if ((char *)func(s3, 'a', 1) - s3 == 0)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(s3, 'd', 2)  == NULL)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if ((char *)func(s3, 'd', 12)  - s3 == 3)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if ((char *)func(s3, 'f', 12)  - s3 == 5)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if ((char *)func(s4, '1', 20)  - s4 == 0)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_memcmp(int (*func)(const void *, const void *, size_t))
{
	char	buffer1[] = "DWgaOtP12df0";
	char	buffer2[] = "DWgaOtP12DF0";
	int		n1;
	int		n2;

	n1 = memcmp(buffer1, buffer2, sizeof(buffer1));
	n2 = func(buffer1, buffer2, sizeof(buffer1));

	if (n1 == n2)
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	n1 = memcmp(buffer1, buffer2, 9);
	n2 = func(buffer1, buffer2, 9);

	if (n1 == n2 && n2 == 0)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strlen(size_t (*func)(const char *))
{
	if (func("Hello World!") == strlen("Hello World!"))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func("") == strlen(""))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("H") == strlen("H"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("Hel\0lo World!") == strlen("Hel\0lo World!"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strdup(char	*(*func)(const char *))
{
	char	s[11] = "0123456789";
	char	empt[1];
	char	*d_s;
	char	*ft_d_s;

	d_s = strdup(s);
	ft_d_s = func(s);

	if (!strcmp(d_s, ft_d_s))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	d_s = strdup(empt);
	ft_d_s = func(empt);

	if (!strcmp(d_s, ft_d_s))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	free(d_s);
	free(ft_d_s);
	printf("\n");
}

static void	test_ft_strcpy(char	*(*func)(char *, const char *))
{
	char	s[] = "Copy successful!";
	char	ft_o[40];
	char	o[40];

	strcpy(o, s);
	func(ft_o, s);

	if (!strcmp(ft_o, o))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	strcpy(o, "");
	func(ft_o, "");

	if (!strcmp(ft_o, o))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strncpy(char *(*func)(char *, const char *, size_t))
{
	char	s[] = "Copy successful!";
	char	ft_o[40];
	char	o[40];

	strncpy(o, s, sizeof(o));
	func(ft_o, s, sizeof(ft_o));

	if (!strncmp(ft_o, o, sizeof(o)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	strncpy(o, "", 1);
	func(ft_o, "", 1);

	if (!strncmp(ft_o, o, sizeof(o)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	char	s15[] = "Source";
	char	d16[] = "Destin";
	char	*p, *ft_p;

	p = strncpy(d16, s15, 5);
	ft_p = func(d16, s15, 5);

	if (!strncmp(ft_p, p, sizeof(d16)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strcat(char	*(*func)(char *, const char *))
{
	char	s[80];
	char	ft_s[80];

	strcpy(s,"these ");
	strcpy(ft_s,"these ");
	strcat(s,"strings are ");
	func(ft_s,"strings are ");
	strcat(s,"concatenated.");
	func(ft_s,"concatenated.");
	

	if (!strncmp(s, ft_s, sizeof(s)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strncat(char *(*func)(char *, const char *, size_t))
{
	char	s[20];
	char	*ft_p;
	char	*p;
	char	ft_o[20];
	char	o[20];

	strcpy(ft_o, "To be ");
	strcpy(o, "To be ");
	strcpy(s, "or not to be");

	p = strncat(o, s, 6);
	ft_p = func(ft_o, s, 6);

	if (!strncmp(o, ft_o, sizeof(o)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);

	if (!strcmp(p, ft_p))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);

	strcpy(ft_o, "To be ");
	strcpy(o, "To be ");

	p = strncat(o, s, 12);
	ft_p = func(ft_o, s, 12);

	if (!strncmp(o, ft_o, sizeof(o)) && !strcmp(p, ft_p))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strlcat(size_t (*func)(char *, const char *, size_t))
{
	char	*ft_p_a;
	char	*p_a;
	char	*p_b;
	char	ft_a[128] = "Hello";
	char	a[128] = "Hello";
	char	b[] = "Hello";

	ft_p_a = strdup("Hello");
	p_a = strdup("Hello");
	p_b = strdup(" World");
	if (func(ft_p_a, p_b, strlen(p_b) + 1) == strlcat(p_a, p_b, strlen(p_b) + 1)
		&& !strcmp(ft_p_a, p_a))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func(ft_p_a, p_b, sizeof(ft_p_a)) == strlcat(p_a, p_b, sizeof(p_a))
		&& !strcmp(ft_p_a, p_a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(ft_p_a, p_b, 1) == strlcat(p_a, p_b, 1)
		&& !strcmp(ft_p_a, p_a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(ft_p_a, p_b, -1) == strlcat(p_a, p_b, -1)
		&& !strcmp(ft_p_a, p_a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(ft_a, b, 0) == strlcat(a, b, 0)
		&& !strcmp(ft_a, a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(ft_a, b, strlen(b) + strlen(ft_a) + 1) == strlcat(a, b, strlen(b) + strlen(a) + 1)
		&& !strcmp(ft_a, a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(ft_a, b, strlen(b) + strlen(ft_a) - 1) == strlcat(a, b, strlen(b) + strlen(a) - 1)
		&& !strcmp(ft_a, a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(ft_a, b, strlen(b) + 1) == strlcat(a, b, strlen(b) + 1)
		&& !strcmp(ft_a, a))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strchr(char	*(*func)(const char *, int))
{
	char	ft_a[128] = "Hello World!";
	char	a[128] = "Hello World!";

	if (!strncmp(func(ft_a, 'o'), strchr(a, 'o'), sizeof(a)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func(ft_a, '3') == strchr(a, '3'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strncmp(func(ft_a, '!'), strchr(a, '!'), sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strncmp(func(ft_a, 0), strchr(a, 0), sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strncmp(ft_a, a, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strrchr(char *(*func)(const char *, int))
{
	char	ft_a[128] = "Hello World!";
	char	a[128] = "Hello World!";

	if (!strncmp(func(ft_a, 'o'), strrchr(a, 'o'), sizeof(a)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func(ft_a, '3') == strrchr(a, '3'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strncmp(func(ft_a, '!'), strrchr(a, '!'), sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strncmp(func(ft_a, 0), strrchr(a, 0), sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strncmp(ft_a, a, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strstr(char *(*func)(const char *, const char *))
{
	const char *largestring = "Foo Bar Baz";

	if (!strcmp(func(largestring, "Bar"), strstr(largestring, "Bar")))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (!strcmp(strstr(largestring, ""), func(largestring, "")))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(largestring, "QW") == NULL
		&& strstr(largestring, "QW") == NULL)
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strcmp(func(largestring, "Ba"), strstr(largestring, "Ba")))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strnstr(char *(*func)(const char *, const char *, size_t))
{
	const char largestring[] = "MZIRIBMZIRIBMZE123";

	if (!strcmp(func(largestring, "MZIRIBMZE", -1), strnstr(largestring, "MZIRIBMZE", -1))
		&& func(largestring, "MZIRIBMZE", 7) == strnstr(largestring, "MZIRIBMZE", 7))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else	
		printf("%s%s%s", RED, "[FAILED] ", NC);
	for (int i = 1; i < 10; i++)
		if (func(largestring, "MZIRIBMZE", i) == strnstr(largestring, "MZIRIBMZE", i))
			printf("%s%s%s", GREEN, " [OK] ", NC);
		else
			printf("%s%s%s", RED, " [FAILED] ", NC);

	char	buf[10];

	bzero(buf, 10);
	strcpy(buf, "un deux 9");
	buf[9] = '6';
	buf[1] = 0;
	if(func(buf, "deux", 10) == strnstr(buf, "deux", 10))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
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

	if (func(a, b) == strcmp(a, b))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func(a, c) == strcmp(a, c))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, d) == strcmp(a, d))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, e) == strcmp(a, e))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, f) == strcmp(a, f))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, g) == strcmp(a, g))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
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

	if (func(a, b, sizeof(a)) == strncmp(a, b, sizeof(a)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func(a, c, 0) == strncmp(a, c, 0))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, d, sizeof(a)) == strncmp(a, d, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, e, sizeof(e)) == strncmp(a, e, sizeof(e)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, f, sizeof(f)) == strncmp(a, f, sizeof(f)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, g, sizeof(a)) == strncmp(a, g, sizeof(a)))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, c, -1) == strncmp(a, c, -1))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, c, 123456) == strncmp(a, c, 123456))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(a, d, 5) == strncmp(a, d, 5))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_atoi(int (*func)(const char *))
{
	if (func("   1425") == atoi("   1425"))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func("12s1425") == atoi("12s1425"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("s1425") == atoi("s1425"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("     \t1425sad213") == atoi("     \t1425sad213"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("2147483647") == atoi("2147483647"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("-2147483648") == atoi("-2147483648"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("+2147483648") == atoi("+2147483648"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("9223372036854775807") == atoi("9223372036854775807"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("9223372036854775808") == atoi("9223372036854775808"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("922337203685477580712125325543357343463643464634633466344366323523959359238")
		== atoi("922337203685477580712125325543357343463643464634633466344366323523959359238"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func("-922337203685477580712125325543357343463643464634633466344366323523959359238")
		== atoi("-922337203685477580712125325543357343463643464634633466344366323523959359238"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_isalpha(int (*func)(char))
{
	if (func('\108') == isalpha('\108'))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func('\148') == isalpha('\148'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(12) == isalpha(12))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func('\200') == isalpha('\200'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func(0) == isalpha(0))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func('\101') == isalpha('\101'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_isdigit(int (*func)(char))
{
	if (func('1') == isdigit('1'))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (func('4') == isdigit('4'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func('\200') == isdigit('\200'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (func('A') == isdigit('A'))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strnew()
{
	char	a[] = "\0\0\0\0\0";
	char	*ft_a;

	ft_a = ft_strnew(5);
	if (!strncmp(a, ft_a, sizeof(a)))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strjoin()
{
	char	s1[] = "where is my ";
	char	s2[] = "malloc ???";
	char	s3[] = "where is my malloc ???";
	char	*p_s;

	p_s = ft_strjoin(s1, s2);

	if (p_s && !strcmp(s1, "where is my ") && !strcmp(p_s, s3))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	ft_strjoin(NULL, s2);
	printf("%s%s%s", GREEN, " [OK] ", NC);
	ft_strjoin(s1, NULL);
	printf("%s%s%s", GREEN, " [OK] ", NC);
	ft_strjoin(NULL, NULL);
	printf("%s%s%s", GREEN, " [OK] ", NC);
	printf("\n");
}

static void	test_ft_strtrim()
{
	char *s1 = "\t   \n\n\n  \n\n\t    Hello \t  Please\n Trim me !\t\t\t\n  \t\t\t\t  ";
	char *s2 = "Hello \t  Please\n Trim me !";
	char *s3;
	int r_size = strlen(s2);
	int size;

	s3 = ft_strtrim(s1);
	if (!memcmp(s2, s3, r_size + 1))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	printf("\n");
}

static void	test_ft_strsplit()
{
	char	test[3][6] = { {'H', 'e', 'l', 'l', 'o', 0}, {'W', 'o', 'r', 'l', 'd', 0}, 0};
	char	**ft_test;

	ft_test = ft_strsplit("((((((((((((Hello((((((((((((((((((((((((((World((((((((((((((((((((", '(');
	for (int i = 0; i < 2; i++)
		if (strcmp(ft_test[i], test[i]))
		{
			printf("%s%s%s", RED, " [FAILED] ", NC);
			return ;
		}
	if ((int)ft_test[2] == 0)
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	printf("\n");
}

static void	test_ft_itoa()
{
	if (!strcmp(ft_itoa(123), "123"))
		printf("%s%s%s", GREEN, "[OK] ", NC);
	else
		printf("%s%s%s", RED, "[FAILED] ", NC);
	if (!strcmp(ft_itoa(2147483647), "2147483647"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strcmp(ft_itoa(-2), "-2"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strcmp(ft_itoa(-2147483648), "-2147483648"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strcmp(ft_itoa(0), "0"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	if (!strcmp(ft_itoa(489652), "489652"))
		printf("%s%s%s", GREEN, " [OK] ", NC);
	else
		printf("%s%s%s", RED, " [FAILED] ", NC);
	printf("\n");
}


int		main(void)
{
	test_ft_putstr();
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

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_isalpha\n", NC);
	printf("\tResults from ISALPHA (STRING.H) below:\n\t");
	test_ft_isalpha(isalpha);
	printf("\tResults from FT_ISALPHA (YOURS) below:\n\t");
	test_ft_isalpha(ft_isalpha);

	printf("%s%s%s", LCYAN, "\n!TEST!\t--\tft_isdigit\n", NC);
	printf("\tResults from ISDIGIT (STRING.H) below:\n\t");
	test_ft_isdigit(isdigit);
	printf("\tResults from FT_ISDIGIT (YOURS) below:\n\t");
	test_ft_isdigit(ft_isdigit);

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

	return (0);
}
