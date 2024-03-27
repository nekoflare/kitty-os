//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_FEATURES_H
#define KITTY_OS_CPP_FEATURES_H

#include <cstdint>

namespace Kernel
{
    void print_cpu_features();
}

#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
void enable_sse();
std::uint64_t has_sse();
std::uint64_t has_sse2();
std::uint64_t has_sse3();
std::uint64_t has_ssse3();
std::uint64_t has_full_sse4();
std::uint64_t has_sse4_1();
std::uint64_t has_sse4_2();
std::uint64_t has_sse4a();
std::uint64_t has_full_sse5();
std::uint64_t has_xop();
std::uint64_t has_fma4();
std::uint64_t has_cvt16();
std::uint64_t has_avx();
std::uint64_t has_xsave();
std::uint64_t has_avx2();
std::uint64_t has_avx512f();
void enable_avx();
void enable_sse_s();
void enable_avx_s();

#ifdef __cplusplus
}
#endif

#endif //KITTY_OS_CPP_FEATURES_H
