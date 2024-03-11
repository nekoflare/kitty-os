//
// Created by Piotr on 11.03.2024.
//

#include <libc/stdio.hpp>
#include <types.h>
#include <kerror.h>
#include "halt.h"

void halt(enum KO_REASON reason) {
    // TODO_: Report to the other processors to kill themselves.

    std::printf("[CRITICAL] Halting the system. Reason: %s\n", kerror(reason));

    _amd64_halt();
}