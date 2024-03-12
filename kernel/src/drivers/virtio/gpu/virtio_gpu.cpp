//
// Created by Piotr on 12.03.2024.
//

#include <drivers/hwinf.h>
#include "virtio_gpu.h"

static ERROR_CODE virtio_gpu_init()
{
    return SUCCESS;
}

static ERROR_CODE virtio_ctl(uint64_t ctrl_idx, const char* data_in, char* data_out, size_t data_out_len)
{
    if (ctrl_idx != 0)
    {
        return FAIL;
    }

    return SUCCESS;
}

static ERROR_CODE virtio_gpu_exit()
{
    return SUCCESS;
}

static driver_api driver_structure this_driver = {
        ._driver_type = DT_GRAPHICS,
        ._driver_info = {
                .driver_name = "VirtIO Graphics Driver",
                .driver_major = 0,
                .driver_minor = 1,
                .driver_build_num = 1,
        },
        ._driver_condition = {
                .driver_pci_vendor_id = 0x1af4,
                .driver_pci_device_id = 0x1050,
                .driver_pci_class_code = 0x3,
                .driver_pci_subclass_code = 0x0
        },

        .is_driver_inited = false,
        .driver_init = &virtio_gpu_init,
        .driver_exit = &virtio_gpu_exit,
        .driver_ctl_function = &virtio_ctl
};