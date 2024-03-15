//
// Created by Piotr on 12.03.2024.
//

#ifndef KITTY_OS_CPP_VIRTIO_GPU_H
#define KITTY_OS_CPP_VIRTIO_GPU_H

#include <drivers/hwinf.h>

static ERROR_CODE virtio_gpu_init();
static ERROR_CODE virtio_ctl(uint64_t ctrl_idx, const char* data_in, char* data_out, size_t data_out_len);

#endif //KITTY_OS_CPP_VIRTIO_GPU_H
