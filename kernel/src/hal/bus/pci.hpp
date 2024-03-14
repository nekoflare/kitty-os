#ifndef _PCI_HPP
#define _PCI_HPP

#include <stdint.h>

namespace PCI
{
    #define PCI_CONFIG_PORT 0xCF8
    #define PCI_DATA_PORT 0xCFC
    #define MAX_PCI_DEVS 8192

    struct PCIDeviceInfo
    {
        uint16_t vendorID;
        uint16_t deviceID;
        const char* deviceName;
    };

    struct PCIDevice
    {
        uint16_t vendorID;
        uint16_t deviceID;
        uint8_t bus;
        uint8_t device;
        uint8_t function;
        uint8_t _class;
        uint8_t subclass;
    };

    extern PCIDevice _pciDevices[MAX_PCI_DEVS];

    const char* dev_to_name(uint16_t vendorID, uint16_t deviceID);
    uint32_t read_config_dword(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset);
    void printDevices();
    
    void scanPCI();
}

#endif