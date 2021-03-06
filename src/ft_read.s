[bits 64]

%include "conf.s"

; prototype ssize_t ft_read(int fd, void *buff, size_t count);

extern d_error

global d_ft_read
d_ft_read:
		push	rbx

		mov		rax, SYSCALL_READ
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