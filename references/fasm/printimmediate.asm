format ELF64 executable 3
segment readable executable
entry start

start:
	pushq 65h
	mov rax, 1
	mov rdi, 1
	mov rsi, [rsp]
	mov rdx, 10
	syscall
	add rsp, 8  ;pop top of stack

	mov rax, 60
	mov rdi, 0
	syscall

