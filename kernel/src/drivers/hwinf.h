//
// Created by Piotr on 11.03.2024.
//
// INFO: This defines how driver interfaces MUST look like.

#ifndef KITTY_OS_CPP_HWINF_H
#define KITTY_OS_CPP_HWINF_H

#include <stdint.h>
#include <error_codes.hpp>
#include <types.h>
#include <stddef.h>
#include <hal/processor/gios.h>

enum driver_type : uint64_t
{
    DT_INVALID, DT_GRAPHICS, DT_AUDIO, DT_NETWORK, DT_STORAGE, DT_USB, DT_SERIAL, DT_IO, DT_PRINTER
};

struct driver_info {
    const char* driver_name;

    const uint64_t driver_major;
    const uint64_t driver_minor;
    const uint64_t driver_build_num;
};

struct driver_condition {
    const uint16_t driver_pci_vendor_id;
    const uint16_t driver_pci_device_id;
    const uint8_t driver_pci_class_code;
    const uint8_t driver_pci_subclass_code;
};

#define driver_api __attribute__((section(".drivers"), used))

struct driver_structure {
    driver_type _driver_type = DT_INVALID;
    driver_info _driver_info = {"Invalid Driver", 0, 0, 0};
    driver_condition _driver_condition = {0, 0, 0, 0};

    bool is_driver_inited = false;
    ERROR_CODE (*driver_init)(io_handle _IoHandle);
    ERROR_CODE (*driver_exit)();
    ERROR_CODE (*driver_ctl_function)(uint64_t ctrl_idx, const char* data_in, char* data_out, size_t data_out_len);
};

#endif //KITTY_OS_CPP_HWINF_H
