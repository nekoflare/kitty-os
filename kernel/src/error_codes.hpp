#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

#include <stdint.h>

/*

    8-bit MSB stands or the region of the error.
        Drivers: 0x10
        FS: 0x20
        Graphics: 0x30
        HAL: 0x40
        KernelAPI: 0x50
        Klibc: 0x60
        Procman: 0x70
        Kernel itself: 0x80

    Next, 4-bits stand for error seriousness
        Critical: 0x1
        Major: 0x2
        Minor: 0x3
        Not critical: 0x0

*/

#define ERR_SUCCESS             0x0000000000000000
#define ERR_NO_FRAMEBUFFERS     0x3010000000000001
#define ERR_FRAMEBUFFER_NULL    0x3010000000000002
#define ERR_SCREEN_OOB          0x3000000000000003
#define ERR_PIXEL_POS_OOB       0x3000000000000004
#define ERR_FAIL                0xFFFFFFFFFFFFFFFF

typedef uint64_t ERR_CODE;

#endif