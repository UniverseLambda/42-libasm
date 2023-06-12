[bits 64]

%include "conf.s"

; prototype char *ft_strcpy(char *dst, char *src);

global d_ft_strcpy
d_ft_strcpy:
		mov		rax, rdi
.loop:

		mov		cl, [rsi]
		mov		[rdi], cl

		test	cl,	cl
		jz		.end

		inc		rsi
		inc		rdi
		jmp		.loop

.end:
		ret

; Required to remove linker warning
SECTION  .note.GNU-stack
