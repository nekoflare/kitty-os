bits 64

extern enable_sse_s
extern enable_avx_s
extern x86_halt_system
extern main
global _start

_start:
    call enable_sse_s
    call enable_avx_s
    call main
    call x86_halt_system