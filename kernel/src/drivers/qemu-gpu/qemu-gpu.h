#pragma once

#include <limine.h>
#include <stdint.h>
#include <stddef.h>
#include <io/io.h>
#include <stdbool.h>
#include <bgraphics/graphics.h>
#include <io/serial.h>
#include <klibc/stdio.h>

#define VBE_DISPI_IOPORT_INDEX 0x01CE
#define VBE_DISPI_IOPORT_DATA  0x01CF

#define VBE_DISPI_INDEX_ID (0)
#define VBE_DISPI_INDEX_XRES (1)
#define VBE_DISPI_INDEX_YRES (2)
#define VBE_DISPI_INDEX_BPP (3)
#define VBE_DISPI_INDEX_ENABLE (4)
#define VBE_DISPI_INDEX_BANK (5)
#define VBE_DISPI_INDEX_VIRT_WIDTH (6)
#define VBE_DISPI_INDEX_VIRT_HEIGHT (7)
#define VBE_DISPI_INDEX_X_OFFSET (8)
#define VBE_DISPI_INDEX_Y_OFFSET (9) 

static void bga_write_register(unsigned short index_value, unsigned short data_value);
static unsigned short bga_read_register(unsigned short index_value);
static int bga_is_available();
static void bga_set_video_mode(unsigned int width, unsigned int height, unsigned int bitdepth, int use_linear_framebuffer, int clear_video_memory);
static struct limine_video_mode* bga_find_mode(struct limine_framebuffer* limine_fb, unsigned int desired_width, unsigned int desired_height, unsigned int desired_bpp);
static void bga_find_mode_and_set_it(struct limine_framebuffer* limine_fb, unsigned int desired_width, unsigned int desired_height, unsigned int desired_bpp);
static void qemu_bga_driver();
