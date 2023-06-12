#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

extern int ft_strlen(const char *str);
extern char *ft_strcpy(char *dst, const char *src);
extern int ft_strcmp(const char *s0, const char *s1);
extern ssize_t ft_read(int fd, void *buffer, size_t size);
extern ssize_t ft_write(int fd, const void *buffer, size_t size);
extern char *ft_strdup(const char *str);

#define SPACE_PADDING 30
#define BUFFER_SIZE 1024

static int compute_checksum(const void *data, size_t size) {
	int sum = 0;
	for (size_t i = 0; i < size; ++i) {
		sum += ((char *)data)[i];
	}
	return sum;
}

static void exec_strlen(const char *test_name, const char *str) {
	int ret;
	int exp;
	int checksum;

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	exp = strlen(str);

	checksum = compute_checksum(str, exp);

	ret = ft_strlen(str);

	if (compute_checksum(str, exp) != checksum) {
		printf("\x1b[31mFAILED\x1b[0m (const correctness, string modified)\n");
		return;
	}

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (result: %d, expected: %d)\n", ret, exp);
		return;
	}

	printf("\x1b[32mPASSED\x1b[0m\n");
}

static void exec_strcpy(const char* test_name, const char *str) {
	char *ret;
	size_t len = strlen(str);
	char buffer[len + 10];
	int checksum;

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	checksum = compute_checksum(str, len);

	ret = ft_strcpy(buffer, str);

	if (compute_checksum(str, len) != checksum) {
		printf("\x1b[31mFAILED\x1b[0m (const correctness, string modified)\n");
		return;
	}

	if (strcmp(str, buffer) != 0) {
		printf("\x1b[31mFAILED\x1b[0m (cpy, result: ```%s```, expected: ```%s```)\n", buffer, str);
		return;
	}
	if (ret != buffer) {
		printf("\x1b[31mFAILED\x1b[0m (return, result: %p, expected: %p)\n", ret, buffer);
		return;
	}

	printf("\x1b[32mPASSED\x1b[0m\n");
}

static void exec_strcmp(const char *test_name, const char *s0, const char *s1) {
	int ret;
	int exp;
	int s0_checksum;
	int s1_checksum;
	int s0_len = strlen(s0);
	int s1_len = strlen(s1);

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	s0_checksum = compute_checksum(s0, s0_len);
	s1_checksum = compute_checksum(s1, s1_len);

	ret = ft_strcmp(s0, s1);
	exp = strcmp(s0, s1);

	if (compute_checksum(s0, s0_len) != s0_checksum) {
		printf("\x1b[31mFAILED\x1b[0m (const correctness on first argument, string modified)\n");
		return;
	}

	if (compute_checksum(s1, s1_len) != s1_checksum) {
		printf("\x1b[31mFAILED\x1b[0m (const correctness on second argument, string modified)\n");
		return;
	}

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (result: %d, expected: %d)\n", ret, exp);
	} else {
		printf("\x1b[32mPASSED\x1b[0m\n");
	}
}

static void exec_buffread(const char *test_name, int fd, void *buffptr, size_t size) {
	int ret;
	int exp;
	int errres;
	int errexp;

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	if (fd != STDIN_FILENO) {
		lseek(fd, 0, SEEK_SET);
	}

	errno = 0;

	ret = ft_read(fd, buffptr, size);
	errres = errno;

	if (fd != STDIN_FILENO) {
		lseek(fd, 0, SEEK_SET);
	}

	errno = 0;

	exp = read(fd, buffptr, size);
	errexp = errno;

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (return, result: %d, expected: %d)\n", ret, exp);
	} else {
		if (errres != errexp) {
			printf("\x1b[31mFAILED\x1b[0m (errno, result: %d, expected: %d)\n", errres, errexp);
		} else {
			printf("\x1b[32mPASSED\x1b[0m\n");
		}
	}

}

static void exec_read(const char *test_name, int fd) {
	int ret;
	int exp;
	int errres;
	int errexp;

	char buffer[BUFFER_SIZE];
	char ft_buffer[BUFFER_SIZE];

	int checksum = 0;
	int ft_checksum = 0;

	for(size_t i = 0; i < BUFFER_SIZE; ++i) {
		checksum += buffer[i];
		ft_checksum += ft_buffer[i];
	}

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	if (fd != STDIN_FILENO) {
		lseek(fd, 0, SEEK_SET);
	} else {
		printf("Testing stdin, please type the same thing twice\n");
	}

	errno = 0;

	ret = ft_read(fd, ft_buffer, BUFFER_SIZE);
	errres = errno;

	if (fd != STDIN_FILENO) {
		lseek(fd, 0, SEEK_SET);
	}

	errno = 0;

	exp = read(fd, buffer, BUFFER_SIZE);
	errexp = errno;

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (return, result: %d, expected: %d)", ret, exp);
		if ((compute_checksum(buffer, BUFFER_SIZE) == checksum) != (compute_checksum(ft_buffer, BUFFER_SIZE) == ft_checksum)) {
			printf("... and buffer preservation not matching");
		}
		putchar('\n');
	} else {
		if (errres != errexp) {
			printf("\x1b[31mFAILED\x1b[0m (errno, result: %d, expected: %d)\n", errres, errexp);
		} else {
			if (ret == 0 || ret == -1) {
				if ((compute_checksum(buffer, BUFFER_SIZE) == checksum) != (compute_checksum(ft_buffer, BUFFER_SIZE) == ft_checksum)) {
					printf("\x1b[31mFAILED\x1b[0m (buffer preservation, result: %d, expected: %d)", ret, exp);
				} else {
					printf("\x1b[32mPASSED\x1b[0m\n");
				}
			} else {
				int cmp;

				if ((cmp = memcmp(buffer, ft_buffer, ret)) != 0) {
					printf("\x1b[31mFAILED\x1b[0m (memcmp, returned: %d)\n", cmp);
				} else {
					printf("\x1b[32mPASSED\x1b[0m\n");
				}
			}
		}
	}
}

static void exec_buffwrite(const char *test_name, int fd, void *buffptr, size_t size) {
	int ret;
	int exp;
	int errres;
	int errexp;

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	errno = 0;

	ret = ft_write(fd, buffptr, size);
	errres = errno;

	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		putchar('\n');
	}

	errno = 0;

	exp = write(fd, buffptr, size);
	errexp = errno;

	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		putchar('\n');
	}

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (return, result: %d, expected: %d)\n", ret, exp);
	} else {
		if (errres != errexp) {
			printf("\x1b[31mFAILED\x1b[0m (errno, result: %d, expected: %d)\n", errres, errexp);
		} else {
			printf("\x1b[32mPASSED\x1b[0m\n");
		}
	}

}

static void exec_write(const char *test_name, int fd, const char *str) {
	int ret;
	int exp;
	int errres;
	int errexp;

	int checksum = 0;
	size_t len = strlen(str);

	checksum = compute_checksum(str, len);

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		putchar('\n');
	}

	errno = 0;

	ret = ft_write(fd, str, len);
	errres = errno;

	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		putchar('\n');
	}

	if (compute_checksum(str, len) != checksum) {
		printf("\x1b[31mFAILED\x1b[0m (const correctness, string modified)\n");
		return;
	}

	errno = 0;

	exp = write(fd, str, len);
	errexp = errno;

	if (ret != exp) {
		printf("\x1b[31mFAILED\x1b[0m (return, result: %d, expected: %d)\n", ret, exp);
		return;
	}
	if (errres != errexp) {
		printf("\x1b[31mFAILED\x1b[0m (errno, result: %d, expected: %d)\n", errres, errexp);
		return;
	}
	printf("\x1b[32mPASSED\x1b[0m\n");
}


static void exec_strdup(const char *test_name, const char *str) {
	char *result;
	int checksum;
	size_t len = strlen(str);

	checksum = compute_checksum(str, len);

	printf(" * executing %s...%*c\t", test_name, (int)(SPACE_PADDING - strlen(test_name)), ' ');

	result = ft_strdup(str);

	if (result == NULL) {
		if (errno != ENOMEM) {
			printf("\x1b[31mFAILED\x1b[0m (errno on NULL, result: %d, expected: %d)\n", ENOMEM, errno);
		}
		return;
	}

	if (compute_checksum(str, len) != checksum) {
		printf("\x1b[31mFAILED\x1b[0m (const correctness, string modified)\n");
	} else {
		if (strcmp(str, result) != 0) {
			printf("\x1b[31mFAILED\x1b[0m (cpy, result: ```%s```, expected: ```%s```)\n", result, str);
			return;
		} else {
			printf("\x1b[32mPASSED\x1b[0m\n");
		}
	}
	free (result);
}



int main() {
	const char *empty = "";
	const char *basic = "BASIC simple";
	const char *big = "azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456789+-*/. ";
	const char *the_answer = "4242424242424242424242424242 is the answer";
	int fd_main = open("main.c", O_RDONLY);
	int fd_emptyne = open("/tmp/empty_nonexisting", O_RDONLY | O_CREAT, 0644);
	int fd_devnull = open("/dev/null", O_RDWR);
	int fd_devzero = open("/dev/zero", O_RDWR);
	int fd_devrand = open("/dev/urandom", O_RDWR);

	puts("\x1b[34mTesting with C :)\x1b[0m");

	exec_strlen(	"STRLEN_EMPTY",							empty);
	exec_strlen(	"STRLEN_BASIC",							basic);
	exec_strlen(	"STRLEN_BIG",							big);
	exec_strlen(	"STRLEN_THEANSWER",						the_answer);

	exec_strcpy(	"STRCPY_EMPTY",							empty);
	exec_strcpy(	"STRCPY_BASIC",							basic);
	exec_strcpy(	"STRCPY_BIG",							big);
	exec_strcpy(	"STRCPY_THEANSWER",						the_answer);

	exec_strcmp(	"STRCMP_EMPTY_EMPTY",					empty,				empty);
	exec_strcmp(	"STRCMP_EMPTY_BASIC",					empty,				basic);
	exec_strcmp(	"STRCMP_EMPTY_BIG",						empty,				big);
	exec_strcmp(	"STRCMP_EMPTY_THEANSWER",				empty,				the_answer);
	exec_strcmp(	"STRCMP_BASIC_EMPTY",					basic,				empty);
	exec_strcmp(	"STRCMP_BASIC_BASIC",					basic,				basic);
	exec_strcmp(	"STRCMP_BASIC_BIG",						basic,				big);
	exec_strcmp(	"STRCMP_BASIC_THEANSWER",				basic,				the_answer);
	exec_strcmp(	"STRCMP_BIG_EMPTY",						big,				empty);
	exec_strcmp(	"STRCMP_BIG_BASIC",						big,				basic);
	exec_strcmp(	"STRCMP_BIG_BIG",						big,				big);
	exec_strcmp(	"STRCMP_BIG_THEANSWER",					big,				the_answer);
	exec_strcmp(	"STRCMP_THEANSWER_EMPTY",				the_answer,			empty);
	exec_strcmp(	"STRCMP_THEANSWER_BASIC",				the_answer,			basic);
	exec_strcmp(	"STRCMP_THEANSWER_BIG",					the_answer,			big);
	exec_strcmp(	"STRCMP_THEANSWER_THEANSWER",			the_answer,			the_answer);

	exec_read(		"FTREAD_TESTERMAIN",					fd_main);
	exec_read(		"FTREAD_EMPTYNONEXISTING",				fd_emptyne);
	exec_read(		"FTREAD_DEVNULL",						fd_devnull);
	exec_read(		"FTREAD_DEVZERO",						fd_devzero);

	// Okay. I'm dumb. Sorry
	// exec_read(		"FTREAD_DEVRAND",						fd_devrand);
	exec_read(		"FTREAD_INVALIDFD0",					-1);
	exec_read(		"FTREAD_INVALIDFD1",					-45);
	exec_read(		"FTREAD_INVALIDFD2",					1202);
	exec_buffread(	"FTREAD_DEVNULL_BUFFNULL",				fd_devnull,			NULL,			1024);
	exec_buffread(	"FTREAD_DEVNULL_BUFFNULL_SIZE0",		fd_devnull,			NULL,			0);
	exec_buffread(	"FTREAD_DEVNULL_BUFFNEG",				fd_devnull,			(void *)(-1),	1024);
	exec_buffread(	"FTREAD_DEVNULL_BUFFNEG_SIZE0",			fd_devnull,			(void *)(-1),	0);
	exec_buffread(	"FTREAD_DEVZERO_BUFFNULL",				fd_devzero,			NULL,			1024);
	exec_buffread(	"FTREAD_DEVZERO_BUFFNULL_SIZE0",		fd_devzero,			NULL,			0);
	exec_buffread(	"FTREAD_DEVZERO_BUFFNEG",				fd_devzero,			(void *)(-1),	1024);
	exec_buffread(	"FTREAD_DEVZERO_BUFFNEG_SIZE0",			fd_devzero,			(void *)(-1),	0);
	exec_buffread(	"FTREAD_DEVRAND_BUFFNULL",				fd_devrand,			NULL,			1024);
	exec_buffread(	"FTREAD_DEVRAND_BUFFNULL_SIZE0",		fd_devrand,			NULL,			0);
	exec_buffread(	"FTREAD_DEVRAND_BUFFNEG",				fd_devrand,			(void *)(-1),	1024);
	exec_buffread(	"FTREAD_DEVRAND_BUFFNEG_SIZE0",			fd_devrand,			(void *)(-1),	0);
	exec_read(		"FTREAD_STDIN",							STDIN_FILENO);

	exec_write(		"FTWRITE_EMPTY_STDOUT",					STDOUT_FILENO,		empty);
	exec_write(		"FTWRITE_EMPTY_READONLY0",				fd_main,			empty);
	exec_write(		"FTWRITE_EMPTY_READONLY1",				fd_emptyne,			empty);
	exec_write(		"FTWRITE_EMPTY_DEVNULL",				fd_devnull,			empty);
	exec_write(		"FTWRITE_EMPTY_DEVZERO",				fd_devnull,			empty);
	exec_write(		"FTWRITE_EMPTY_DEVRAND",				fd_devnull,			empty);
	exec_write(		"FTWRITE_EMPTY_INVALIDFD0",				-1,					empty);
	exec_write(		"FTWRITE_EMPTY_INVALIDFD1",				-45,				empty);
	exec_write(		"FTWRITE_EMPTY_INVALIDFD2",				1202,				empty);

	exec_write(		"FTWRITE_BASIC_STDOUT",					STDOUT_FILENO,		basic);
	exec_write(		"FTWRITE_BASIC_READONLY0",				fd_main,			basic);
	exec_write(		"FTWRITE_BASIC_READONLY1",				fd_emptyne,			basic);
	exec_write(		"FTWRITE_BASIC_DEVNULL",				fd_devnull,			basic);
	exec_write(		"FTWRITE_BASIC_DEVZERO",				fd_devnull,			basic);
	exec_write(		"FTWRITE_BASIC_DEVRAND",				fd_devnull,			basic);
	exec_write(		"FTWRITE_BASIC_INVALIDFD0",				-1,					basic);
	exec_write(		"FTWRITE_BASIC_INVALIDFD1",				-45,				basic);
	exec_write(		"FTWRITE_BASIC_INVALIDFD2",				1202,				basic);

	exec_write(		"FTWRITE_BIG_STDOUT",					STDOUT_FILENO,		big);
	exec_write(		"FTWRITE_BIG_READONLY0",				fd_main,			big);
	exec_write(		"FTWRITE_BIG_READONLY1",				fd_emptyne,			big);
	exec_write(		"FTWRITE_BIG_DEVNULL",					fd_devnull,			big);
	exec_write(		"FTWRITE_BIG_DEVZERO",					fd_devnull,			big);
	exec_write(		"FTWRITE_BIG_DEVRAND",					fd_devnull,			big);
	exec_write(		"FTWRITE_BIG_INVALIDFD0",				-1,					big);
	exec_write(		"FTWRITE_BIG_INVALIDFD1",				-45,				big);
	exec_write(		"FTWRITE_BIG_INVALIDFD2",				1202,				big);

	exec_write(		"FTWRITE_THEANSWER_READONLY0",			STDOUT_FILENO,			the_answer);
	exec_write(		"FTWRITE_THEANSWER_READONLY0",			fd_main,			the_answer);
	exec_write(		"FTWRITE_THEANSWER_READONLY1",			fd_emptyne,			the_answer);
	exec_write(		"FTWRITE_THEANSWER_DEVNULL",			fd_devnull,			the_answer);
	exec_write(		"FTWRITE_THEANSWER_DEVZERO",			fd_devnull,			the_answer);
	exec_write(		"FTWRITE_THEANSWER_DEVRAND",			fd_devnull,			the_answer);
	exec_write(		"FTWRITE_THEANSWER_INVALIDFD0",			-1,					the_answer);
	exec_write(		"FTWRITE_THEANSWER_INVALIDFD1",			-45,				the_answer);
	exec_write(		"FTWRITE_THEANSWER_INVALIDFD2",			1202,				the_answer);

	exec_buffwrite(	"FTWRITE_DEVNULL_BUFFNULL",				fd_devnull,			NULL,			1024);
	exec_buffwrite(	"FTWRITE_DEVNULL_BUFFNULL_SIZE0",		fd_devnull,			NULL,			0);
	exec_buffwrite(	"FTWRITE_DEVNULL_BUFFNEG",				fd_devnull,			(void *)(-1),	1024);
	exec_buffwrite(	"FTWRITE_DEVNULL_BUFFNEG_SIZE0",		fd_devnull,			(void *)(-1),	0);
	exec_buffwrite(	"FTWRITE_DEVZERO_BUFFNULL",				fd_devzero,			NULL,			1024);
	exec_buffwrite(	"FTWRITE_DEVZERO_BUFFNULL_SIZE0",		fd_devzero,			NULL,			0);
	exec_buffwrite(	"FTWRITE_DEVZERO_BUFFNEG",				fd_devzero,			(void *)(-1),	1024);
	exec_buffwrite(	"FTWRITE_DEVZERO_BUFFNEG_SIZE0",		fd_devzero,			(void *)(-1),	0);
	exec_buffwrite(	"FTWRITE_DEVRAND_BUFFNULL",				fd_devrand,			NULL,			1024);
	exec_buffwrite(	"FTWRITE_DEVRAND_BUFFNULL_SIZE0",		fd_devrand,			NULL,			0);
	exec_buffwrite(	"FTWRITE_DEVRAND_BUFFNEG",				fd_devrand,			(void *)(-1),	1024);
	exec_buffwrite(	"FTWRITE_DEVRAND_BUFFNEG_SIZE0",		fd_devrand,			(void *)(-1),	0);

	exec_strdup(	"STRDUP_EMPTY",							empty);
	exec_strdup(	"STRDUP_BASIC",							basic);
	exec_strdup(	"STRDUP_BIG",							big);
	exec_strdup(	"STRDUP_THEANSWER",						the_answer);

	close(fd_main);
	close(fd_emptyne);
	close(fd_devnull);
	close(fd_devzero);
	close(fd_devrand);

	remove("/tmp/empty_nonexisting");

	return 0;
}