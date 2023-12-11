#include <drivers/nvme-qemu/nvme-qemu.h>
#include <stdbool.h>
#include <drvapi/drvapi.h>

static bool is_driver_loaded = false;

static void qemu_nvme_init(void)
{
    if (is_driver_loaded)
    {
        return;
    }

    is_driver_loaded = true;

    return;
}

static void qemu_nvme_exit(void)
{
    is_driver_loaded = false;
}

static DRIVER_API struct DriverDescriptor driver_desc = {
    .driver_name = "QEMU NVMe Compliant driver.",

    // Matching policy
    .immediate_load = false,
    .match_for_class = false,
    .match_for_subclass = false,
    .match_for_pci_vendor_device = true,

    // Values
    .pci_class = 0,
    .pci_subclass = 0,
    .pci_device_id = 0x8086,
    .pci_vendor_id = 0x5845,
    
    // Pointer
    .driver_init_function = &qemu_nvme_init,
    .driver_exit_function = &qemu_nvme_exit,

    // Defaults
    .is_loaded = false
};