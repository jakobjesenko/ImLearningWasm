format ELF64 executable 3
segment readable executable
entry start

start:
	mov rax, 60
	mov rdi, 0
	syscall
