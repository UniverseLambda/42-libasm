[bits 64]

%include "conf.s"

; prototype ssize_t ft_write(int fd, void *buff, size_t count);

extern d_error

global d_ft_write
d_ft_write:
		push	rbx

		mov		rax, SYSCALL_WRITE
		syscall
		
		test	rax, rax
		jns		.end
		
		mov		rbx, -1
		imul	rbx, rax
		
		call	d_error
		
		mov		[rax], rbx
		mov		rax, -1

.end:
		pop		rbx
		ret