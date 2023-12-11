#include <drvapi/drvapi_init.h>
#include <pci/pci.h>
#include <io/serial.h>

extern struct DriverDescriptor* DRIVERS_START;
extern struct DriverDescriptor DRIVERS_END;

void call_init_function(struct DriverDescriptor* drv_desc)
{
    drv_desc->driver_init_function();
    drv_desc->is_loaded = true;
}

void debug_print_loaded_driver(struct DriverDescriptor* drv_desc)
{
    serial_printf("Started %s driver.\n", drv_desc->driver_name);
    
    if (drv_desc->match_for_pci_vendor_device)
    {
        serial_printf("Matching for PCI vendor:device.\n");
        serial_printf("VEN:DEV: %x:%x\n", drv_desc->pci_vendor_id, drv_desc->pci_device_id);
    }
    else if (drv_desc->match_for_class)
    {
        serial_printf("Matching for PCI class.\n");
        serial_printf("Class: %d\n", drv_desc->pci_class);
    }
    else if (drv_desc->match_for_subclass)
    {
        serial_printf("Matching for PCI sub-class.\n");
        serial_printf("Sub-class: %d\n", drv_desc->pci_subclass);
    }
}

void initialize_drivers(void)
{
    const char *p1 = (const char *)&DRIVERS_START;
    const char *p2 = (const char *)&DRIVERS_END;
    
    unsigned long long diff = p2 - p1;
    unsigned long long num_drivers = diff / sizeof(struct DriverDescriptor);

    serial_printf("diff: %x\n", diff);
    serial_printf("num drivers: %x\n", num_drivers);

    for (
        unsigned int idx = 0;
        num_drivers > idx;
        idx ++
    )
    {    
        struct DriverDescriptor* driver = &DRIVERS_START[idx];
        serial_printf("Drivers: %s\n", driver->driver_name);
        // check for imm load
        if (driver->immediate_load == true && driver->is_loaded == false)
        {
            call_init_function(driver);
            debug_print_loaded_driver(driver);

            break;
        }

        // iterate through PCI.
        if (driver->match_for_class == true)
        {
            for (unsigned int index = 0; numPCIDevices > index; index++)
            {
                if (pcidevices[index].class == driver->pci_class)
                {
                    call_init_function(driver);
                    debug_print_loaded_driver(driver);
                    break;
                }
            }
        }

        // iterate through PCI.
        if (driver->match_for_subclass == true)
        {
            for (unsigned int index = 0; numPCIDevices > index; index++)
            {
                if (pcidevices[index].subclass == driver->pci_subclass)
                {
                    call_init_function(driver);
                    debug_print_loaded_driver(driver);

                    break;
                }
            }
        }

        // iterate through PCI.
        if (driver->match_for_pci_vendor_device == true)
        {
            for (unsigned int index = 0; numPCIDevices > index; index++)
            {
                if (pcidevices[index].deviceID == driver->pci_device_id && pcidevices[index].vendorID == driver->pci_vendor_id)
                {
                    call_init_function(driver);
                    debug_print_loaded_driver(driver);

                    break;
                }
            }
        }
    }
}