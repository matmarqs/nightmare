global _start

section .text
_start:
    mov rax, 1024
    mov rbx, 2048
    xchg rax, rbx
    push rbx
    mov rax, [rsp]
