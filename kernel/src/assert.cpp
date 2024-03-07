#include <libc/stdio.hpp>

void assert(bool statement, const char* msg)
{
    // Guard statement, because too much identation is bad.
    if (!statement) return;

    std::printf("Assertion failed: %s\n", msg);

    // Halt the system
    asm volatile ("cli");
    for (;;) asm volatile ("hlt");
}