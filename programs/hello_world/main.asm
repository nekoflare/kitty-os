bits 64

section .text
global _start
_start:
    ;; Write
    mov rax, 1  ;; sys_write
    mov rdi, 0  ;; stdout
    mov rsi, hello_world        ;; Buffer
    mov rdx, [hello_world_len]    ;; Length
    int 0x80    ;; Ich habe syscall nicht.

    ;; Exit
    mov rax, 0x3c   ;; sys_exit
    mov rdi, 0      ;; ESUCCESS
    int 0x80        ;; Ich habe syscall nicht.

section .data
    hello_world: db "Hello, World!", 0xa, 0x0
    hello_world_len: dq 14