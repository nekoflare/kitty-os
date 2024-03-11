section .text
global _amd64_halt
_amd64_halt:
    cli

    ._inf_loop:
        hlt
        jmp ._inf_loop