#include "pci.h"
#include "io/serial.h"
#include "io/io.h"

// PCI device database
struct PCIDeviceInfo pciDeviceDatabase[] = {
    {0x8086, 0x100E, "Intel Ethernet Controller"},
    {0x8086, 0x1237, "Intel 440FX - 82441FX PMC [Natoma]"},
    {0x8086, 0x7000, "Intel 82371SB PIIX3 ISA [Natoma/Triton II]"},
    {0x8086, 0x7010, "Intel 82371SB PIIX3 IDE [Natoma/Triton II]"},
    {0x8086, 0x7113, "Intel 82371AB/EB/MB PIIX4 ACPI"},
    {0x8086, 0x100F, "Intel 82540EM Gigabit Ethernet Controller"},
    {0x1234, 0x1111, "QEMU VGA PCI Device"},
    {0x10EC, 0x0887, "Realtek Audio Controller"},
    {0x14E4, 0x43A3, "Broadcom Wi-Fi Adapter"},
    {0x1102, 0x0002, "Creative Labs Sound Blaster"},
    {0x1B21, 0x1042, "ASMedia USB 3.0 Controller"},
    {0x8086, 0x282A, "Intel SATA Controller"},
    {0x1969, 0xE0A1, "Atheros Ethernet Adapter"},
    {0x8086, 0x2918, "82801IB (ICH9) LPC Interface Controller"},
    {0x8086, 0x29C0, "82G33/G31/P35/P31 Express DRAM Controller"},
    {0x8086, 0x10D3, "82574L Gigabit Network Connection"},
    {0x8086, 0x2922, "82801IR/IO/IH (ICH9R/DO/DH) 6 port SATA Controller [AHCI mode]"},
    {0x8086, 0x2930, "82801I (ICH9 Family) SMBus Controller"},
    {0x1AF4, 0x1050, "Red Hat, Inc. Virtio GPU"},
    {0x1b36, 0x0100, "QXL paravirtual graphic card"},
    {0x1002, 0x5046, "Advanced Micro Devices, Inc. [AMD/ATI] Rage 4 [Rage 128 PRO AGP 4X TMDS]"},
    {0x8086, 0x10F5, "Intel 82567LM Gigabit Network Connection"},
    {0x10DE, 0x1B80, "NVIDIA GeForce GTX 1080"},
    {0x1022, 0x145C, "Family 17h (Models 00h-0fh) USB 3.0 Host Controller"},
    {0x1A03, 0x2000, "ASPEED Graphics Family"},
    {0x8086, 0x8C22, "8 Series/C220 Series Chipset Family SMBus Controller"},
    {0x8086, 0x9D3D, "Sunrise Point-LP Active Management Technology - SOL"},
    {0x1022, 0x1451, "Family 17h (Models 00h-0fh) I/O Memory Management Unit"},
    {0x1B21, 0x2142, "ASM2142 USB 3.1 Host Controller	"},
    {0x8086, 0x9A03, "TigerLake-LP Dynamic Tuning Processor Participant"},
    {0x10DE, 0x1C03, "GP106 [GeForce GTX 1060 6GB]"},
    {0x1D6B, 0x0001, "Linux Foundation 1.1 root hub"},
    {0x197B, 0x2393, "JMicron Technology Corp. MS Host Controller"},
    {0x8086, 0x5845, "Intel Corporation. QEMU NVM Express Controller"}
};


struct PCIDevice pcidevices[1024];
unsigned int numPCIDevices = 0;

// Function to add a PCI device to the pcidevices array
void add_pci_device(uint16_t vendorID, uint16_t deviceID, uint8_t bus, uint8_t device, uint8_t function, uint8_t class, uint8_t subclass) {
    if (numPCIDevices < sizeof(pcidevices) / sizeof(pcidevices[0])) {
        pcidevices[numPCIDevices].vendorID = vendorID;
        pcidevices[numPCIDevices].deviceID = deviceID;
        pcidevices[numPCIDevices].bus = bus;
        pcidevices[numPCIDevices].device = device;
        pcidevices[numPCIDevices].function = function;
        pcidevices[numPCIDevices].class = class;
        pcidevices[numPCIDevices].subclass = subclass;
        numPCIDevices++;
    }
}

// Function to look up a device name based on Vendor ID and Device ID
const char* get_device_name(uint16_t vendorID, uint16_t deviceID) {
    int numDevices = sizeof(pciDeviceDatabase) / sizeof(pciDeviceDatabase[0]);
    for (int i = 0; i < numDevices; i++) {
        if (pciDeviceDatabase[i].vendorID == vendorID && pciDeviceDatabase[i].deviceID == deviceID) {
            return pciDeviceDatabase[i].deviceName;
        }
    }
    return "Unknown Device"; // Return a default name for unknown devices
}

uint32_t pci_config_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t ldevice = (uint32_t)device;
    uint32_t lfunction = (uint32_t)function;

    // Create the address as per PCI specifications
    address = (uint32_t)((lbus << 16) | (ldevice << 11) | (lfunction << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write the address to the PCI configuration address port (0xCF8)
    outl(0xCF8, address);

    // Read the DWORD from the PCI configuration data port (0xCFC)
    return inl(0xCFC);
}

// Function to read a byte from PCI configuration space
uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    // Read a DWORD and extract the relevant byte
    uint32_t dword_data = pci_config_read_dword(bus, device, function, offset & 0xFC);
    return (uint8_t)((dword_data >> ((offset & 0x3) * 8)) & 0xFF);
}

// Function to read a word (2 bytes) from PCI configuration space
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    // Read a DWORD and extract the relevant word
    uint32_t dword_data = pci_config_read_dword(bus, device, function, offset & 0xFC);
    return (uint16_t)((dword_data >> ((offset & 0x2) * 8)) & 0xFFFF);
}

// Function to read a quad word (8 bytes) from PCI configuration space
uint64_t pci_config_read_qword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    // Read two DWORDs and combine them to form a qword
    uint64_t qword_data = 0;
    uint32_t dword_low = pci_config_read_dword(bus, device, function, offset & 0xFC);
    uint32_t dword_high = pci_config_read_dword(bus, device, function, (offset & 0xFC) + 4);

    qword_data = dword_high;
    qword_data <<= 32;
    qword_data |= dword_low;

    return qword_data;
}

void pci_config_memcopy(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, void* destination, size_t length) {
    // Ensure the destination pointer is valid
    if (destination == NULL) {
        // Handle invalid destination pointer
        return;
    }

    // Read data from PCI configuration space and copy to the destination
    size_t num_dwords = length / sizeof(uint32_t);
    size_t remaining_bytes = length % sizeof(uint32_t);
    uint32_t* dest = (uint32_t*)destination;

    for (size_t i = 0; i < num_dwords; ++i) {
        // Read DWORD by DWORD and copy to the destination pointer
        dest[i] = pci_config_read_dword(bus, device, function, offset + i * sizeof(uint32_t));
    }

    // If there are remaining bytes, read them separately
    if (remaining_bytes > 0) {
        // Read a DWORD and copy the remaining bytes
        uint32_t dword_data = pci_config_read_dword(bus, device, function, offset + num_dwords * sizeof(uint32_t));
        uint8_t* remaining_dest = (uint8_t*)&dest[num_dwords];
        uint8_t* dword_data_ptr = (uint8_t*)&dword_data;

        for (size_t i = 0; i < remaining_bytes; ++i) {
            remaining_dest[i] = dword_data_ptr[i];
        }
    }
}

const char* get_dev_class(unsigned int classID) {
    switch(classID) {
        case 0x00:
            return "Device was built prior definition of the class code field";
        case 0x01:
            return "Mass Storage Controller";
        case 0x02:
            return "Network Controller";
        case 0x03:
            return "Display Controller";
        case 0x04:
            return "Multimedia Controller";
        case 0x05:
            return "Memory Controller";
        case 0x06:
            return "Bridge Device";
        case 0x07:
            return "Simple Communication Controllers";
        case 0x08:
            return "Base System Peripherals";
        case 0x09:
            return "Input Devices";
        case 0x0A:
            return "Docking Stations";
        case 0x0B:
            return "Processors";
        case 0x0C:
            return "Serial Bus Controllers";
        case 0x0D:
            return "Wireless Controllers";
        case 0x0E:
            return "Intelligent I/O Controllers";
        case 0x0F:
            return "Satellite Communication Controllers";
        case 0x10:
            return "Encryption/Decryption Controllers";
        case 0x11:
            return "Data Acquisition and Signal Processing Controllers";
        default:
            return "Unknown Device Class";
    }
}

void print_pci_devices() {
    serial_printf("PCI Devices:\n");
    for (unsigned int i = 0; i < numPCIDevices; i++) {
        uint16_t vendorID = pcidevices[i].vendorID;
        uint16_t deviceID = pcidevices[i].deviceID;
        const char* deviceName = get_device_name(vendorID, deviceID);

        serial_printf("Bus:%x Dev:%x Func:%d VenID:%x DevID:%x DevName:%s\n",
               pcidevices[i].bus, pcidevices[i].device, pcidevices[i].function, vendorID, deviceID, deviceName);
        serial_printf("Dev Class: %s\n", get_dev_class(pcidevices[i].class));
    }
}

void pci_init()
{
    for (uint8_t bus = 0; bus < 8; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {
                // Read the Vendor ID and Device ID
                uint32_t vendorID = pci_config_read_dword(bus, device, function, 0x00);
                if (vendorID == 0xFFFFFFFF) {
                    continue; // No device at this address
                }

                // Read the Class ID
                uint32_t classID = pci_config_read_dword(bus, device, function, 0x08);
                uint8_t classCode = (classID >> 24) & 0xFF;
                uint8_t subclassCode = (classID >> 16) & 0xFF;

                // Print vendor ID, device ID, class, and subclass in hexadecimal format
                serial_printf("Bus:");
                print16_hex(bus);
                serial_printf(" Device:");
                print16_hex(device);
                serial_printf(" Function:");
                print16_hex(function);
                serial_printf(" VendorID:");
                print16_hex((uint16_t)(vendorID & 0xFFFF));
                serial_printf(" DeviceID:");
                print16_hex((uint16_t)((vendorID >> 16) & 0xFFFF));
                serial_printf(" Class:");
                print16_hex(classCode);
                serial_printf(" Subclass:");
                print16_hex(subclassCode);
                serial_printf("\n");

                // Add the PCI device to the list with class and subclass information
                add_pci_device((uint16_t)(vendorID & 0xFFFF), (uint16_t)((vendorID >> 16) & 0xFFFF),
                               bus, device, function, classCode, subclassCode);
            }
        }
    }
}
