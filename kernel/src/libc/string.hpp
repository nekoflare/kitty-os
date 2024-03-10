#ifndef STRING_HPP
#define STRING_HPP

#include <cstdint>
#include <cstddef>

extern "C"
{
    void *memcpy(void *dest, const void *src, std::size_t n);
    void *memset(void *s, int c, std::size_t n);
    void *memmove(void *dest, const void *src, std::size_t n);
    int memcmp(const void *s1, const void *s2, std::size_t n);
    int strlen(const char* s);
    bool isalnum(char c);
    int strncmp(const char* str1, const char* str2, size_t count);
    int strcmp(const char* str1, const char* str2);
    const char* strchr(const char* str, int character);
}

#endif