#include <power/power.hpp>
#include <hal/processor/io.hpp>

namespace Power
{
    void ShutdownEmuBochs()
    {
        Processor::outw(BOCHS_SHUTDOWN_PORT, BOCHS_SHUTDOWN_VALUE);
    }

    void ShutdownEmuQemu()
    {
        Processor::outw(QEMU_SHUTDOWN_PORT, QEMU_SHUTDOWN_VALUE);
    }

    void ShutdownVMVBox()
    {
        Processor::outw(VIRTUALBOX_SHUTDOWN_PORT, VIRTUALBOX_SHUTDOWN_VALUE);
    }

    void ShutdownVMClHypervisor()
    {
        Processor::outb(CLOUD_HYPERVISOR_SHUTDOWN_PORT, CLOUD_HYPERVISOR_SHUTDOWN_VALUE);
    }

    void Shutdown()
    {
        Power::ShutdownEmuBochs();
        Power::ShutdownEmuQemu();
        Power::ShutdownVMVBox();
        Power::ShutdownVMClHypervisor();
    }
}