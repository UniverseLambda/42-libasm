extern int ft_strlen(const char *str);

#include <stdio.h>

int main() {
	const char *empty = "";
	const char *basic = "BASIC simple";
	const char *big = "klhrdlkfhesljkfhezskljfhejklszfherjklzfhrekljghrjklrkejlhbgkjlr";

	puts("Testing with C :/");

	printf(" * ft_strlen: %d for `%s`\n", ft_strlen(empty), empty);
	printf(" * ft_strlen: %d for `%s`\n", ft_strlen(basic), basic);
	printf(" * ft_strlen: %d for `%s`\n", ft_strlen(big), big);

	return 0;
}