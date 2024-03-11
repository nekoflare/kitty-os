//
// Created by Piotr on 11.03.2024.
//
// INFO: This defines how driver interfaces MUST look like.

#ifndef KITTY_OS_CPP_HWINF_H
#define KITTY_OS_CPP_HWINF_H

#include <stdint.h>
#include <error_codes.hpp>

enum driver_type : uint64_t
{
    DT_GRAPHICS = 0,
    DT_AUDIO = 1,
    DT_AUDIO_VIDEOS = 2,
    DT_NETWORK = 3,
    DT_STORAGE = 4,
    DT_USB = 5,
    DT_SERIAL = 6,
    DT_IO = 7,
    DT_PRINTER = 8
};

typedef void* CAP_HANDLE;

struct driver_common {
    driver_type drv_type;
    CAP_HANDLE capabilities_handle;
};

struct serial_driver_interface : public driver_common
{
    ERROR_CODE (*serial_init_function)();
    ERROR_CODE (*serial_init_port)(unsigned short com_port_idx, unsigned long long flags);
    ERROR_CODE (*serial_write)(unsigned short com_port_idx, const unsigned char* data, size_t length);
    ERROR_CODE (*serial_read)(unsigned short com_port_idx, unsigned char* buffer, size_it length);
};

struct graphics_driver_interface : public driver_common
{

};

#endif //KITTY_OS_CPP_HWINF_H
