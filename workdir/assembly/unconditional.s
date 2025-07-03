global _start

section .text
_start:
    mov rbx, 2
    mov rcx, 5
loop:
    imul rbx, rbx
    jmp func
    loop loop
func:
    mov rax, 60
    mov rdi, 0
    syscall
