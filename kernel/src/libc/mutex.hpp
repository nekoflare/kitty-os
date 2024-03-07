#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <atomic>

class Spinlock
{
public:
    Spinlock() : flag(false) {}

    void lock()
    {
        while (flag.exchange(true, std::memory_order_acquire))
        {
            // Spinning.
            __asm__("nop");
        }
    }

    void unlock()
    {
        flag.store(false, std::memory_order_release);
    }
private:
    std::atomic<bool> flag;
};

#endif