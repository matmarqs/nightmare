global  _start

section .data
    message db "Fibonacci Sequence:", 0x0a

section .text
_start:
    mov rax, 1       ; rax: syscall number 1
    mov rdi, 1      ; rdi: fd 1 for stdout
    mov rsi,message ; rsi: pointer to message
    mov rdx, 20      ; rdx: print length of 20 bytes
    syscall         ; call write syscall to the intro message
    xor rax, rax    ; initialize rax to 0
    xor rbx, rbx    ; initialize rbx to 0
    inc rbx         ; increment rbx to 1
loopFib:
    add rax, rbx    ; get the next number
    xchg rax, rbx   ; swap values
    push rax        ; save the value of rax
    push rbx        ; save the value of rbx
    mov rax, 1      ; write syscall
    mov rdi, 1      ; print to stdout
    add rbx, 0x30   ; in ASCII: '0' = 0x30, '1' = 0x31, etc.
    push rbx        ; create pointer to rbx, at rsp
    mov rsi, rsp    ; string to be printed
    mov rdx, 1      ; length to be printed, only one char
    syscall
    pop rbx         ; NOP pop
    pop rbx         ; restore the value of rbx
    pop rax         ; restore the value of rax
    cmp rbx, 10		; do rbx - 10
    js loopFib		; jump if result is <0
    mov rax, 60
    mov rdi, 0
    syscall


; global _start
;
; section .text
; _start:
;     xor rax, rax    ; zero rax
;     xor rbx, rbx    ; zero rbx
;     inc rbx     ; increment rbx to 1
;     push rax    ; push registers to stack
;     push rbx
;     ; call function
;     pop rbx
;     pop rax     ; restore registers from stack
;     mov rcx, 10
; loopFib:
;     add rax, rbx
;     xchg rax, rbx
;     cmp rbx, 10
;     js loopFib
;
;     ;xor rax, rax
;     ;xor rbx, rbx
;     ;inc bl
;     ;add al, bl
;     ;not rax
;
;     ;mov rax, rsp
;     ;mov rax, [rsp]
;     ;mov al, 0
;     ;mov bl, 1
;     ;xchg rbx, rax
