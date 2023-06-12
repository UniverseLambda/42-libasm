[bits 64]

%include "conf.s"

; prototype int ft_strcmp(const char *s0, const char *s1);

global d_ft_strcmp
d_ft_strcmp:
		xor		rax, rax
.loop:
		mov		al, [rdi]
		mov		cl, [rsi]

		sub		rax, rcx
		jnz		.end

		test	al, al
		jz		.end

		inc		rsi
		inc		rdi
		jmp		.loop

.end:
		ret

; Required to remove linker warning
SECTION  .note.GNU-stack
