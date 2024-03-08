#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

// Define PACKED attribute for GCC and compatible compilers
#define PACKED __attribute__((packed))
#define UNUSED __attribute__((unused))
#define ALIGNED(x) __attribute__((aligned(x)))
#define NORETURN __attribute__((noreturn))
#define WEAK __attribute__((weak))
#define CONST __attribute__((const))
#define PURE __attribute__((pure))
#define USED __attribute__((used))
#endif // ATTRIBUTES_HPP
