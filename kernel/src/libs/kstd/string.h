//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_STRING_H
#define KITTY_OS_CPP_STRING_H

#include <cstdint>

extern "C"
{
    void* memcpy(void* dest, const void* src, std::size_t n);
    void* memset(void* s, int c, std::size_t n);
    void* memmove(void* dest, const void* src, std::size_t n);
    int memcmp(const void* s1, const void* s2, std::size_t n);
    std::size_t strlen(const char* s);
    bool isalnum(char c);
    std::size_t strncmp(const char* s1, const char* s2, std::size_t m);
    std::size_t strcmp(const char* s1, const char* s2);
    const char* strchr(const char* s, int c);
    std::uint32_t strtoul(const char* str);
}

#endif //KITTY_OS_CPP_STRING_H
