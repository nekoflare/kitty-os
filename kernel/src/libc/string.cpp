#include <libc/string.hpp>
// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .cpp file.

extern "C" {

    void *memcpy(void *dest, const void *src, std::size_t n) {
        std::uint8_t *pdest = static_cast<std::uint8_t *>(dest);
        const std::uint8_t *psrc = static_cast<const std::uint8_t *>(src);

        for (std::size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }

        return dest;
    }

    void *memset(void *s, int c, std::size_t n) {
        std::uint8_t *p = static_cast<std::uint8_t *>(s);

        for (std::size_t i = 0; i < n; i++) {
            p[i] = static_cast<uint8_t>(c);
        }

        return s;
    }

    void *memmove(void *dest, const void *src, std::size_t n) {
        std::uint8_t *pdest = static_cast<std::uint8_t *>(dest);
        const std::uint8_t *psrc = static_cast<const std::uint8_t *>(src);

        if (src > dest) {
            for (std::size_t i = 0; i < n; i++) {
                pdest[i] = psrc[i];
            }
        } else if (src < dest) {
            for (std::size_t i = n; i > 0; i--) {
                pdest[i-1] = psrc[i-1];
            }
        }

        return dest;
    }

    int memcmp(const void *s1, const void *s2, std::size_t n) {
        const std::uint8_t *p1 = static_cast<const std::uint8_t *>(s1);
        const std::uint8_t *p2 = static_cast<const std::uint8_t *>(s2);

        for (std::size_t i = 0; i < n; i++) {
            if (p1[i] != p2[i]) {
                return p1[i] < p2[i] ? -1 : 1;
            }
        }

        return 0;
    }

}

int strlen(const char* s)
{
    int v = 0;
    while (*s != 0)
    {
        v++;
        s++;
    }

    return v;
}