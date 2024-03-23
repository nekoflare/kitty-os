//
// Created by Piotr on 23.03.2024.
//

#include "string.h"

extern "C"
{
    void* memcpy(void* dest, const void* src, std::size_t n)
    {
        std::uint8_t* pdest = static_cast<std::uint8_t*>(dest);
        const std::uint8_t* psrc = static_cast<const std::uint8_t*>(src);

        for (std::size_t i = 0; i < n; i++)
        {
            pdest[i] = psrc[i];
        }

        return dest;
    }

    void* memset(void* s, int c, std::size_t n)
    {
        std::uint8_t* p = static_cast<std::uint8_t*>(s);

        for (std::size_t i = 0; i < n; i++)
        {
            p[i] = static_cast<uint8_t>(c);
        }

        return s;
    }

    void* memmove(void* dest, const void* src, std::size_t n)
    {
        std::uint8_t* pdest = static_cast<std::uint8_t*>(dest);
        const std::uint8_t* psrc = static_cast<const std::uint8_t*>(src);

        if (src > dest)
        {
            for (std::size_t i = 0; i < n; i++)
            {
                pdest[i] = psrc[i];
            }
        }
        else if (src < dest)
        {
            for (std::size_t i = n; i > 0; i--)
            {
                pdest[i - 1] = psrc[i - 1];
            }
        }

        return dest;
    }

    int memcmp(const void* s1, const void* s2, std::size_t n)
    {
        const std::uint8_t* p1 = static_cast<const std::uint8_t*>(s1);
        const std::uint8_t* p2 = static_cast<const std::uint8_t*>(s2);

        for (std::size_t i = 0; i < n; i++)
        {
            if (p1[i] != p2[i])
            {
                return p1[i] < p2[i] ? -1 : 1;
            }
        }

        return 0;
    }

    std::size_t strlen(const char* s)
    {
        const char* p = s;
        while (*p)
        {
            ++p;
        }

        return p - s;
    }

    bool isalnum(char c)
    {
        return  (c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9');
    }

    std::size_t strncmp(const char* s1, const char* s2, std::size_t m)
    {
        for (std::size_t i = 0; i < m; ++i)
        {
            if (s1[i] != s2[i])
            {
                return static_cast<unsigned char>(s1[i]) - static_cast<unsigned char>(s2[i]);
            }

            if (s1[i] == '\0')
            {
                return 0;
            }
        }

        return 0;
    }

    std::size_t strcmp(const char* s1, const char* s2)
    {
        while (*s1 != '\0' || *s2 != '\0')
        {
            if (*s1 != *s2)
            {
                return *s1 - *s2;
            }

            ++s1;
            ++s2;
        }

        return 0;
    }

    const char* strchr(const char* s, int c)
    {
        while (*s != '\0')
        {
            if (*s == c)
            {
                return s;
            }

            ++s;
        }

        if (c == '\0')
            return s;

        return nullptr;
    }

    std::uint32_t strtoul(const char* str)
    {
        std::uint32_t result = 0;
        while (*str)
        {
            char c = *str;
            std::uint32_t digit;
            if (c >= '0' && c <= '9')
            {
                digit = c - '0';
            }
            else if (c >= 'a' && c <= 'f')
            {
                digit = c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F')
            {
                digit = c - 'A' + 10;
            }
            else
            {
                // Invalid character in input
                break;
            }
            result = (result << 4) | digit;
            ++str;
        }
        return result;
    }

}

