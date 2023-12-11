#include "shutdown.h"
#include "io/io.h"

void qemu_shutdown()
{
    outw(0xb004, 0x2000);
}

void qemu_shutdown_newer()
{
    outw(0x604, 0x2000);
}

void vbox_shutdown()
{
    outw(0x4004, 0x3400);
}

void common_shutdown()
{
    qemu_shutdown();
    qemu_shutdown_newer();
    vbox_shutdown();
}