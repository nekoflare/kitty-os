#ifndef PANIC_PANIC_HPP
#define PANIC_PANIC_HPP

namespace Kernel
{
    namespace Control
    {
        void Panic(
            const char* error_string
        );
    }
}

#endif