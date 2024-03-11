//
// Created by Piotr on 11.03.2024.
//

#ifndef KITTY_OS_CPP_HALT_H
#define KITTY_OS_CPP_HALT_H

extern "C" void _amd64_halt();
void halt(enum KO_REASON reason);

#endif //KITTY_OS_CPP_HALT_H
