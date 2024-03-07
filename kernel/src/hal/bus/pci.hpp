#ifndef _PCI_HPP
#define _PCI_HPP

#include <stdint.h>

namespace PCI
{
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
    extern unsigned int _pciDeviceCount;

    const char* dev_to_name(uint16_t vendorID, uint16_t deviceID);
    uint32_t readConfigDword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
    void printDevices();
    
    void scanPCI();
}

#endif