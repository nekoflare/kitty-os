#include <power/power.hpp>
#include <hal/processor/io.hpp>

namespace Power
{
    void shutdown_bochs()
    {
        Processor::outw(BOCHS_SHUTDOWN_PORT, BOCHS_SHUTDOWN_VALUE);
    }

    void shutdown_qemu()
    {
        Processor::outw(QEMU_SHUTDOWN_PORT, QEMU_SHUTDOWN_VALUE);
    }

    void shutdown_virtualbox()
    {
        Processor::outw(VIRTUALBOX_SHUTDOWN_PORT, VIRTUALBOX_SHUTDOWN_VALUE);
    }

    void shutdown_vmcl_hypervisor()
    {
        Processor::outb(CLOUD_HYPERVISOR_SHUTDOWN_PORT, CLOUD_HYPERVISOR_SHUTDOWN_VALUE);
    }

    void shutdown()
    {
        Power::shutdown_bochs();
        Power::shutdown_qemu();
        Power::shutdown_virtualbox();
        Power::shutdown_vmcl_hypervisor();
    }
}