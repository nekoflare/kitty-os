#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void (*driver_init)(void);
typedef void (*driver_exit)(void);

struct DriverDescriptor
{
    const char driver_name[36];

    bool immediate_load;
    bool match_for_class;
    bool match_for_subclass;
    bool match_for_pci_vendor_device;

    uint16_t pci_vendor_id;
    uint16_t pci_device_id;
    uint16_t pci_class;
    uint16_t pci_subclass;

    driver_init driver_init_function;
    driver_exit driver_exit_function;

    bool is_loaded;
} __attribute__((packed));

#define REGISTER_DRIVER(local_name, driver_name, driver_init, driver_exit, immediate_load, match_for_class, match_for_subclass, class_val, subclass_val, match_for_pci_vendor_device, pci_vendor_id, pci_device_id) \
    __attribute__((section(".drivers"), used)) static struct DriverDescriptor local_name = { \
        .driver_name = driver_name, \
        .immediate_load = immediate_load, \
        .match_for_class = match_for_class, \
        .match_for_subclass = match_for_subclass, \
        .match_for_pci_vendor_device = match_for_pci_vendor_device, \
        .pci_vendor_id = pci_vendor_id, \
        .pci_device_id = pci_device_id, \
        .driver_init_function = driver_init, \
        .driver_exit_function = driver_exit, \
        .is_loaded = false, \
        .pci_class = class_val, \
        .pci_subclass = subclass_val \
    }

#define DRIVER_API __attribute__((section(".drivers"), used))