[bits 64]

%include "conf.s"

; prototype char *ft_strdup(const char *s);

extern d_malloc
extern d_ft_strlen
extern d_ft_strcpy

global d_ft_strdup
d_ft_strdup:
		push		rbx

		mov			rbx, rdi
		call		d_ft_strlen

		mov			rdi, rax
		inc			rdi
		call		d_malloc

		test		rax, rax
		jz			.end

		mov			rdi, rax
		mov			rsi, rbx
		call		d_ft_strcpy

.end:
		pop			rbx
		ret


; Required to remove linker warning
SECTION  .note.GNU-stack
