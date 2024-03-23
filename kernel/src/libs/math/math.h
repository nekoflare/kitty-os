//
// Created by Piotr on 23.03.2024.
//

#ifndef KITTY_OS_CPP_MATH_H
#define KITTY_OS_CPP_MATH_H

namespace Math
{
    template <typename T>
    T abs(T value) {
        return (value < 0) ? -value : value;
    }
}

#endif //KITTY_OS_CPP_MATH_H
