//
// Created by Piotr on 11.03.2024.
//

#include "kerror.h"

ko_reason_pair reasons[] = {
        {NO_REASON,      "No reason given."},
        {BUG_CHECK_FAIL, "The system failed to do the bug check test."}
};

const char* kerror(enum KO_REASON reason)
{
    // find the index in the reasons.
    for (auto& i : reasons)
    {
        if (i.idx == reason)
        {
            return i.reason;
        }
    }

    return "Invalid reason.";
}