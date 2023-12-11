#include <drivers/qemu-gpu/qemu-gpu.h>
#include <drvapi/drvapi.h>
#include <stdbool.h>
#include <io/serial.h>
#include <bgraphics/graphics.h>
#include <klibc/stdio.h>
#include <kdbg/dmesg.h>

static bool is_driver_loaded = false;

static void qemu_driver_init(void)
{
    if (is_driver_loaded)
    {
        return;
    }

    // Set to good.
    serial_printf("QEMU Compliant driver has been loaded.\n");
    dmesg_printf("QEMU Compliant driver has been loaded.\n");

    bga_find_mode_and_set_it(framebuffer_request.response->framebuffers[0], 1920, 1080, 32);
    dmesg_printf("Video mode has been set to 1920x1080 @ 32-BPP.\n");

    is_driver_loaded = true;
    dmesg_printf("QEMU Compliant Graphics driver has finished the job.\n");
    return;
}

static void qemu_driver_exit(void)
{
    dmesg_printf("QEMU Compliant graphics driver has been unloaded.\n");
    is_driver_loaded = false;
}

static void bga_write_register(unsigned short index_value, unsigned short data_value)
{
    outw(VBE_DISPI_IOPORT_INDEX, index_value);
    outw(VBE_DISPI_IOPORT_DATA, data_value);
}

static unsigned short bga_read_register(unsigned short index_value)
{
    outw(VBE_DISPI_IOPORT_INDEX, index_value);
    return inw(VBE_DISPI_IOPORT_DATA);
}

static int bga_is_available()
{
    return (bga_read_register(VBE_DISPI_INDEX_ID) >= 0xB0C2);
}

static void bga_set_video_mode(unsigned int width, unsigned int height, unsigned int bitdepth, int use_linear_framebuffer, int clear_video_memory)
{
    bga_write_register(VBE_DISPI_INDEX_ENABLE, 0x00);
    bga_write_register(VBE_DISPI_INDEX_XRES, width);
    bga_write_register(VBE_DISPI_INDEX_YRES, height);
    bga_write_register(VBE_DISPI_INDEX_BPP, bitdepth);
    bga_write_register(VBE_DISPI_INDEX_ENABLE, 
        4 | (use_linear_framebuffer ? 0x40 : 0)
        | (clear_video_memory ? 0 : 0xC1)
    );
}

static struct limine_video_mode* bga_find_mode(struct limine_framebuffer* limine_fb, unsigned int desired_width, unsigned int desired_height, unsigned int desired_bpp)
{
    for (unsigned int i = 0; limine_fb->mode_count > i; i++)
    {
        struct limine_video_mode* mode = limine_fb->modes[i];

        if (
            desired_width == mode->width &&
            desired_height == mode->height &&
            desired_bpp == mode->bpp
        )
        {
            return mode;
        }
    }

    return (struct limine_video_mode*) NULL;
}

static void bga_find_mode_and_set_it(struct limine_framebuffer* limine_fb, unsigned int desired_width, unsigned int desired_height, unsigned int desired_bpp)
{
    struct limine_video_mode* new_mode = bga_find_mode(
        limine_fb, desired_width, desired_height, desired_bpp
    );

    if (new_mode == NULL)
    {
        serial_printf("Failed to set new mode. Not found.\n");
        return;
    }

    bga_set_video_mode(desired_width, desired_height, desired_bpp, false, false);

    this_framebuffer->width = desired_width;
    this_framebuffer->height = desired_height;
    this_framebuffer->bpp = desired_bpp;
    this_framebuffer->pitch = new_mode->pitch;

    this_framebuffer->blue_mask_shift = new_mode->blue_mask_shift;
    this_framebuffer->blue_mask_size = new_mode->blue_mask_size;

    this_framebuffer->green_mask_shift = new_mode->green_mask_shift;
    this_framebuffer->green_mask_size = new_mode->green_mask_size;

    this_framebuffer->red_mask_shift = new_mode->red_mask_shift;
    this_framebuffer->red_mask_size = new_mode->red_mask_size;
}
static void qemu_bga_driver()
{
    bga_find_mode_and_set_it(framebuffer_request.response->framebuffers[0], 1920, 1080, 32);
    clean_stdio();
}

static DRIVER_API struct DriverDescriptor driver_desc = {
    .driver_name = "QEMU Compliant driver.",

    // Matching policy
    .immediate_load = false,
    .match_for_class = false,
    .match_for_subclass = false,
    .match_for_pci_vendor_device = true,

    // Values
    .pci_class = 0,
    .pci_subclass = 0,
    .pci_device_id = 0x1111,
    .pci_vendor_id = 0x1234,
    
    // Pointer
    .driver_init_function = &qemu_driver_init,
    .driver_exit_function = &qemu_driver_exit,

    // Defaults
    .is_loaded = false
};