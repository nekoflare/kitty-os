#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include "kdbg/dbg_print.h"
#include "bgraphics/graphics.h"
#include "io/serial.h"
#include "string.h"
#include "mem/mm.h"
#include "wallpaper.h"
#include "power/shutdown.h"
#include "pci/pci.h"
#include "klibc/psf2.h"
#include "font.h"
#include "klibc/stdio.h"
#include "hal/hal.h"
#include <drvapi/drvapi_init.h>
#include <kdbg/dmesg.h>
#include <hal/idt.h>

LIMINE_BASE_REVISION(1)

// Halt and catch fire function.
static void hcf(void)
{
    asm ("cli");
    for (;;)
    {
        asm ("hlt");
    }
}

struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

struct limine_boot_time_request boot_time_request = {
    .id = LIMINE_BOOT_TIME_REQUEST,
    .revision = 0
};

struct Time
{
    unsigned long year;
    unsigned long month;
    unsigned long day;
    unsigned long hour;
    unsigned long minute;
    unsigned long second;
};

struct Time convert_unix_timestamp_to_date(long timestamp) {
    long seconds_per_day = 86400; // Number of seconds in a day
    long days_in_400_years = 146097; // Number of days in 400 years
    long unix_epoch_days = timestamp / seconds_per_day; // Convert seconds to days

    // Calculation for year
    int years = 1970 + (unix_epoch_days / days_in_400_years) * 400;
    int days_remainder = unix_epoch_days % days_in_400_years;
    int year_adjustment = days_remainder / (365 * 4 + 1);
    years += year_adjustment * 4;
    days_remainder %= (365 * 4 + 1);

    // Calculate year within a 4-year cycle
    if (days_remainder >= 366) {
        days_remainder--;
        years++;
        days_remainder -= 365 * 3;
        years += days_remainder / 365;
        days_remainder %= 365;
    }

    years += days_remainder / 365;
    days_remainder %= 365;

    // Calculation for month and day
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Calculation for leap year
    int is_leap_year = ((years % 4 == 0 && years % 100 != 0) || (years % 400 == 0));

    // Adjust days in February for leap years
    if (is_leap_year) {
        days_per_month[1] = 29;
    }

    int month = 0;
    while (days_remainder >= days_per_month[month]) {
        days_remainder -= days_per_month[month];
        month++;
    }

    int day = days_remainder + 1; // Days are zero-based

    // Calculation for hours, minutes, and seconds
    int hours = (timestamp % seconds_per_day) / 3600;
    int minutes = (timestamp % 3600) / 60;
    int seconds = timestamp % 60;

    struct Time result;
    result.year = years;
    result.month = month + 1;
    result.day = day;
    result.hour = hours;
    result.minute = minutes;
    result.second = seconds;

    return result;
}

extern void isr0(void);

void _start(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        hcf();
    }

    initialize_graphics();
    initialize_com1();
    initialize_psf2_font(font_data);
    initialize_mm();
    pci_init();

    // add struct KernelBasicGraphicsAdapterInformation (kbgai)
    struct KernelMemoryInformation* kmi;
    kmi = get_kmi();

    unsigned int screen_width = framebuffer_request.response->framebuffers[0]->width;
    unsigned int screen_height = framebuffer_request.response->framebuffers[0]->height;

    kprintf("Memory left: %lld/%lld (MB).\n", kmi->usable_memory/1024/1024, kmi->memory_overall/1024/1024);
    kprintf("Screen size: %dx%d@%d\n", screen_width, screen_height, framebuffer_request.response->framebuffers[0]->bpp);

    print_pci_devices();

    kprintf("Hello, World!\nSupport for the multi-lines also!\nOwO\nI love KittyOS.\n");
    struct Time t = convert_unix_timestamp_to_date(boot_time_request.response->boot_time);
    kprintf("Boot time: (Y:M:D; H:M:S): %d:%d:%d; %d:%d:%d.\n", t.year, t.month, t.day, t.hour, t.minute, t.second);

    // common_shutdown();

    for (unsigned int i = 0; framebuffer_request.response->framebuffers[0]->mode_count > i; i++)
    {
        kprintf("Mode %d: %dx%d@%d\n", i, framebuffer_request.response->framebuffers[0]->modes[i]->width, framebuffer_request.response->framebuffers[0]->modes[i]->height, framebuffer_request.response->framebuffers[0]->modes[i]->bpp);
        serial_printf("Mode %d: %dx%d@%d\n", i, framebuffer_request.response->framebuffers[0]->modes[i]->width, framebuffer_request.response->framebuffers[0]->modes[i]->height, framebuffer_request.response->framebuffers[0]->modes[i]->bpp);
    }

    // qemu_bga_driver();

    put_rect(0, 0, this_framebuffer->width, this_framebuffer->height, 0, 0, 0);

    //put_bitmap_image_data(wallpaper_data, 0, 0, 1024, 768);
    
    kprintf("highest mode set!\n");
    
    clean_stdio();

    initialize_drivers();

    put_bitmap_image_data_fit_to(wallpaper_data, 0, 0, 1920, 1080, 1920, 1080);

    kprintf("Welcome to the KittyOS!\n");
    kprintf("Loading kitty stuffs~!\n");

    idt_install();
    // crash!?>

    kprintf("hello!\n");

    // We're done, just hang...
    hcf();
}
