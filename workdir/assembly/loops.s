global _start

section .text
_start:
    mov rax, 2
    mov rcx, 10
loop:
    imul rax, rax
    loop loop
