//
// Created by Piotr on 15.03.2024.
//

#include "debug.h"
#include <serial/Serial.h>
#include <stdarg.h>

static Serial* devices[8];

void debug_prepare()
{
    Serial serial1(Serial::COM1, Serial::DIV_115200, false);
    Serial serial2(Serial::COM2, Serial::DIV_115200, false);
    Serial serial3(Serial::COM3, Serial::DIV_115200, false);
    Serial serial4(Serial::COM4, Serial::DIV_115200, false);
    Serial serial5(Serial::COM5, Serial::DIV_115200, false);
    Serial serial6(Serial::COM6, Serial::DIV_115200, false);
    Serial serial7(Serial::COM7, Serial::DIV_115200, false);
    Serial serial8(Serial::COM8, Serial::DIV_115200, false);

    devices[0] = &serial1;
    devices[1] = &serial2;
    devices[2] = &serial3;
    devices[3] = &serial4;
    devices[4] = &serial5;
    devices[5] = &serial6;
    devices[6] = &serial7;
    devices[7] = &serial8;
}

void debug_print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    devices[0]->Print(fmt, args);
    va_end(args);
}