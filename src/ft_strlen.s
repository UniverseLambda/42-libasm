[bits 64]

%include "conf.s"

; prototype: int ft_strlen(const char *ptr);
global d_ft_strlen
d_ft_strlen:
	xor rax, rax
.loop:
	cmp byte [rdi], 0
	jz .end

	inc rax
	inc rdi
	jmp .loop
.end:
	ret
