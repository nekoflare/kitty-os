#ifndef LIMITS_H
#define LIMITS_H

#define CHAR_BIT 8                // Number of bits in a char
#define SCHAR_MIN (-128)          // Minimum value for a signed char
#define SCHAR_MAX 127             // Maximum value for a signed char
#define UCHAR_MAX 255             // Maximum value for an unsigned char
#define CHAR_MIN SCHAR_MIN        // Minimum value for a char
#define CHAR_MAX SCHAR_MAX        // Maximum value for a char
#define MB_LEN_MAX 1              // Maximum number of bytes in a multibyte character
#define SHRT_MIN (-32768)         // Minimum value for a short
#define SHRT_MAX 32767            // Maximum value for a short
#define USHRT_MAX 65535           // Maximum value for an unsigned short
#define INT_MIN (-2147483647 - 1) // Minimum value for an int
#define INT_MAX 2147483647        // Maximum value for an int
#define UINT_MAX 4294967295U      // Maximum value for an unsigned int
#define LONG_MIN (-2147483647L - 1) // Minimum value for a long
#define LONG_MAX 2147483647L        // Maximum value for a long
#define ULONG_MAX 4294967295UL      // Maximum value for an unsigned long

#endif // LIMITS_H
