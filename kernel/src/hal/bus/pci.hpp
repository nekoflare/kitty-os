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

    struct pci_common_header
    {
        uint16_t vendor_id;
        uint16_t device_id;
        uint16_t command;
        uint16_t status;
        uint8_t revision_id;
        uint8_t prog_if;
        uint8_t subclass;
        uint8_t class_code;
        uint8_t cache_line_size;
        uint8_t latency_timer;
        uint8_t header_type;
        uint8_t bist;
    } __attribute__((packed));

    struct pci_header_0
    {
        pci_common_header header;
        uint32_t bar0, bar1, bar2, bar3, bar4, bar5;
        uint32_t cardbus_cis_pointer;
        uint16_t subsystem_vendor_id;
        uint16_t subsystem_id;
        uint32_t expansion_rom_base_address;
        uint8_t capabilities_pointer;
        uint8_t rsvd0;
        uint16_t rsvd1;
        uint32_t rsvd2;
        uint8_t interrupt_line;
        uint8_t interrupt_pin;
        uint8_t min_grant;
        uint8_t max_latency;
    } __attribute__((packed));

    struct pci_header_pci_to_pci_bridge
    {
        pci_common_header header;
        uint32_t bar0, bar1;
        uint8_t primary_bus_number;
        uint8_t secondary_bus_number;
        uint8_t subordinate_bus_number;
        uint8_t secondary_latency_timer;
        uint8_t io_base;
        uint8_t io_limit;
        uint16_t secondary_status;
        uint16_t memory_base;
        uint16_t memory_limit;
        uint32_t prefetchable_memory_base;
        uint32_t prefetchable_memory_limit;
        uint32_t prefetchable_base_upper32;
        uint32_t prefetchable_limit_upper32;
        uint16_t io_base_upper16;
        uint16_t io_limit_upper16;
        uint8_t capability_pointer;
        uint8_t rsvd0;
        uint16_t rsvd1;
        uint32_t expansion_rom_base_address;
        uint8_t interrupt_line;
        uint8_t interrupt_pin;
        uint16_t bridge_control;

    };

    extern PCIDevice _pciDevices[MAX_PCI_DEVS];

    const char* dev_to_name(uint16_t vendorID, uint16_t deviceID);
    uint32_t read_config_dword(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset);
    void print_devices();
    
    void scanPCI();
}

#endif