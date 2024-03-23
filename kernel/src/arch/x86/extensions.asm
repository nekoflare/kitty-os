bits 64

global enable_sse
global has_sse2
global has_sse3
global has_ssse3
global has_full_sse4
global has_sse4_1
global has_sse4_2
global has_sse4a
global has_full_sse5
global has_xop
global has_fma4
global has_cvt16
global has_avx
global has_xsave
global has_avx2
global has_avx512f
global enable_avx

global enable_sse_s
global enable_avx_s

enable_avx_s:
    call has_avx
    cmp rax, 0
    jz .no_avx

    call enable_avx
.no_avx:
    ret

enable_sse_s:
enable_sse:
    push rax
    push rdx

    ; Check if SSE is supported.
    mov rax, 1
    cpuid
    test rdx, 1 << 25
    jz .no_sse

    ; Enable SSE
    mov rax, cr0
    and rax, 0xFFFFFFFFFFFFFFFB ; Clear CR0.EM
    or rax, 0x2 ; Enable CR0.MP
    mov cr0, rax
    mov rax, cr4
    or rax, 3 << 9 ; Set CR4.OSFXSR and CR4.OSXMMEXCPT
    mov cr4, rax

    pop rdx
    pop rax
    mov rax, 1  ; Return true
    ret
.no_sse:
    pop rdx
    pop rax
    mov rax, 0 ; Return false
    ret

has_sse2:
    push rax
    push rdx

    mov rax, 1
    cpuid
    test rdx, 1 << 26

    jz .no_sse2

    pop rdx
    pop rax
    mov rax, 1
    ret
.no_sse2:
    pop rdx
    pop rax
    mov rax, 0
    ret

has_sse3:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 0

    jz .no_sse3

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_sse3:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_ssse3:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 26

    jz .no_ssse3

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_ssse3:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_full_sse4:
    call has_sse4_1
    cmp rax, 0
    je .no_full_sse4
    call has_sse4_2
    cmp rax, 0
    je .no_full_sse4
    call has_sse4a
    cmp rax, 0
    je .no_full_sse4

    mov rax, 1
    ret
.no_full_sse4:
    mov rax, 0
    ret

has_sse4_1:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 19

    jz .no_sse4_1

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_sse4_1:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_sse4_2:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 20

    jz .no_sse4_2

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_sse4_2:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_sse4a:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 6

    jz .no_sse4a

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_sse4a:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_full_sse5:
    call has_xop
    cmp rax, 0
    je .no_full_sse5
    call has_fma4
    cmp rax, 0
    je .no_full_sse5
    call has_cvt16
    cmp rax, 0
    je .no_full_sse5
    call has_avx
    cmp rax, 0
    je .no_full_sse5
    call has_xsave
    cmp rax, 0
    je .no_full_sse5
    call has_avx2
    cmp rax, 0
    je .no_full_sse5

    mov rax, 1
    ret
.no_full_sse5:
    mov rax, 0
    ret

has_xop:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 11
    jz .no_xop

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_xop:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_fma4:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 16
    jz .no_fma4

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_fma4:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_cvt16:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 29
    jz .no_cvt16

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_cvt16:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_avx:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 28
    jz .no_avx

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_avx:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_xsave:
    push rax
    push rcx

    mov rax, 1
    cpuid
    test rcx, 1 << 26
    jz .no_xsave

    pop rcx
    pop rax
    mov rax, 1
    ret
.no_xsave:
    pop rcx
    pop rax
    mov rax, 0
    ret

has_avx2:
    push rax
    push rbx

    mov rax, 1
    cpuid
    test rbx, 1 << 5
    jz .no_avx2

    pop rbx
    pop rax
    mov rax, 1
    ret
.no_avx2:
    pop rbx
    pop rax
    mov rax, 0
    ret

has_avx512f:
    push rax
    push rbx

    mov rax, 7
    cpuid
    test rbx, 1 << 16
    jz .no_avx512f

    pop rbx
    pop rax
    mov rax, 1
    ret
.no_avx512f:
    pop rbx
    pop rax
    mov rax, 0
    ret

enable_avx:
    push rax
    push rcx
    push rdx

    xor rcx, rcx
    xgetbv ;; load xcr0
    or rax, 7
    xsetbv

    pop rdx
    pop rcx
    pop rax
    ret

    ret