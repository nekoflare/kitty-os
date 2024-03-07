#ifndef POWER_HPP
#define POWER_HPP

#define BOCHS_SHUTDOWN_PORT 0xb004
#define BOCHS_SHUTDOWN_VALUE  0x2000

#define QEMU_SHUTDOWN_PORT 0x604
#define QEMU_SHUTDOWN_VALUE 0x2000

#define VIRTUALBOX_SHUTDOWN_PORT 0x4004
#define VIRTUALBOX_SHUTDOWN_VALUE 0x3400

#define CLOUD_HYPERVISOR_SHUTDOWN_PORT 0x600
#define CLOUD_HYPERVISOR_SHUTDOWN_VALUE 0x34

namespace Power
{
    void ShutdownEmuBochs();
    void ShutdownEmuQemu();
    void ShutdownVMVBox();
    void ShutdownVMClHypervisor();
    void Shutdown();
}

#endif  