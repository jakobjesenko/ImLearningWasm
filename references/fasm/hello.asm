format ELF64 executable 3
segment readable executable
entry start

start:
	mov rax, 1
	mov rdi, 1
	lea rsi, [astring]
	mov rdx, 14
	syscall

	mov rax, 60
	mov rdi, 0
	syscall

segment readable writable
astring db "Hello, World!", 10
