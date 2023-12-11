#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <stddef.h>


// Structure to hold PCI device information
struct PCIDeviceInfo {
    uint16_t vendorID;
    uint16_t deviceID;
    const char* deviceName;
};

struct PCIDevice {
    uint16_t vendorID;
    uint16_t deviceID;
    uint8_t bus;
    uint8_t device;
    uint8_t function;

    uint8_t class, subclass;
};

extern struct PCIDevice pcidevices[1024];
unsigned int numPCIDevices;


// Function to add a PCI device to the pcidevices array
void add_pci_device(uint16_t vendorID, uint16_t deviceID, uint8_t bus, uint8_t device, uint8_t function, uint8_t class, uint8_t subclass);

// Function to look up a device name based on Vendor ID and Device ID
const char* get_device_name(uint16_t vendor_id, uint16_t device_id);

uint8_t  pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint32_t pci_config_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
uint64_t pci_config_read_qword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
void pci_config_memcopy(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, void* destination, size_t length);

void print_pci_devices();

void pci_init();

#endif // PCI_H
