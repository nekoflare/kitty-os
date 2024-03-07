#include <panic/panic.hpp>

void Kernel::Control::Panic(const char* error_string)
{
    for (;;);
}