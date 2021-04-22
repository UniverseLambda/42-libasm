#include <string.h>
#include <stdio.h>

extern int ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, const char *src);
extern int ft_strcmp(const char *s0, const char *s1);

#define SPACE_PADDING 30

static void exec_strlen(const char *test_name, const char *str) {
	int ret;
	int exp;

	printf(" * executing %s...%*c\t", test_name, SPACE_PADDING - strlen(test_name), ' ');
	ret = ft_strlen(str);
	exp = strlen(str);

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (result: %d, expected %d)\n", ret, exp);
	} else {
		printf("\x1b[32mPASSED\x1b[0m\n");
	}
}

static void exec_strcpy(const char* test_name, const char *str) {
	char *ret;
	char buffer[1024];

	printf(" * executing %s...%*c\t", test_name, SPACE_PADDING - strlen(test_name), ' ');
	ret = ft_strcpy(buffer, str);

	if (strcmp(str, buffer) != 0) {
		printf("\x1b[31mFAILED\x1b[0m (cpy, result: ```%s```, expected ```%s```)\n", buffer, str);
	} else {
		if (ret != buffer) {
			printf("\x1b[31mFAILED\x1b[0m (return, result: %p, expected %p)\n", ret, buffer);
		} else {
			printf("\x1b[32mPASSED\x1b[0m\n");
		}
	}

}

static void exec_strcmp(const char *test_name, const char *s0, const char *s1) {
	int ret;
	int exp;

	printf(" * executing %s...%*c\t", test_name, SPACE_PADDING - strlen(test_name), ' ');
	ret = ft_strcmp(s0, s1);
	exp = strcmp(s0, s1);

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (result: %d, expected %d)\n", ret, exp);
	} else {
		printf("\x1b[32mPASSED\x1b[0m\n");
	}
}

int main() {
	const char *empty = "";
	const char *basic = "BASIC simple";
	const char *big = "klhrdlkfhesljkfhezskljfhejklszfherjklzfhrekljghrjklrkejlhbgkjlr";
	const char *the_answer = "4242424242424242424242424242 is the answer";

	puts("\x1b[34mTesting with C :)\x1b[0m");

	exec_strlen("STRLEN_EMPTY", empty);
	exec_strlen("STRLEN_BASIC", basic);
	exec_strlen("STRLEN_BIG", big);
	exec_strlen("STRLEN_THEANSWER", the_answer);

	exec_strcpy("STRCPY_EMPTY", empty);
	exec_strcpy("STRCPY_BASIC", basic);
	exec_strcpy("STRCPY_BIG", big);
	exec_strcpy("STRCPY_THEANSWER", the_answer);
	
	exec_strcmp("STRCMP_EMPTY_EMPTY", empty, empty);
	exec_strcmp("STRCMP_EMPTY_BASIC", empty, basic);
	exec_strcmp("STRCMP_EMPTY_BIG", empty, big);
	exec_strcmp("STRCMP_EMPTY_THEANSWER", empty, the_answer);
	exec_strcmp("STRCMP_BASIC_EMPTY", basic, empty);
	exec_strcmp("STRCMP_BASIC_BASIC", basic, basic);
	exec_strcmp("STRCMP_BASIC_BIG", basic, big);
	exec_strcmp("STRCMP_BASIC_THEANSWER", basic, the_answer);
	exec_strcmp("STRCMP_BIG_EMPTY", big, empty);
	exec_strcmp("STRCMP_BIG_BASIC", big, basic);
	exec_strcmp("STRCMP_BIG_BIG", big, big);
	exec_strcmp("STRCMP_BIG_THEANSWER", big, the_answer);
	exec_strcmp("STRCMP_THEANSWER_EMPTY", the_answer, empty);
	exec_strcmp("STRCMP_THEANSWER_BASIC", the_answer, basic);
	exec_strcmp("STRCMP_THEANSWER_BIG", the_answer, big);
	exec_strcmp("STRCMP_THEANSWER_THEANSWER", the_answer, the_answer);


	return 0;
}