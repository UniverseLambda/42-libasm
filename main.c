#include <string.h>
#include <stdio.h>

extern int ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, const char *src);

int main() {
	const char *empty = "";
	const char *basic = "BASIC simple";
	const char *big = "klhrdlkfhesljkfhezskljfhejklszfherjklzfhrekljghrjklrkejlhbgkjlr";
	const char *the_answer = "4242424242424242424242424242 is the answer";
	char *ret;
	char buffer[1024];

	puts("Testing with C :)");

	printf("ft_strlen: %d, expected %d for `%s`\n", ft_strlen(empty), strlen(empty), empty);
	printf("ft_strlen: %d, expected %d for `%s`\n", ft_strlen(basic), strlen(basic), basic);
	printf("ft_strlen: %d, expected %d for `%s`\n", ft_strlen(big), strlen(big), big);
	printf("ft_strlen: %d, expected %d for `%s`\n", ft_strlen(the_answer), strlen(the_answer), the_answer);

	printf("ft_strcpy: %s, expected %s... (dst: %p, ret: %p)\n", (ret = ft_strcpy(buffer, empty)), empty, buffer, ret);
	printf("ft_strcpy: %s, expected %s... (dst: %p, ret: %p)\n", (ret = ft_strcpy(buffer, basic)), basic, buffer, ret);
	printf("ft_strcpy: %s, expected %s... (dst: %p, ret: %p)\n", (ret = ft_strcpy(buffer, big)), big, buffer, ret);
	printf("ft_strcpy: %s, expected %s... (dst: %p, ret: %p)\n", (ret = ft_strcpy(buffer, the_answer)), the_answer, buffer, ret);

	return 0;
}