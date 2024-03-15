#include <hal/bus/pci.hpp>
#include <hal/processor/io.hpp>
#include <libc/stdio.hpp>

namespace PCI
{
    PCIDevice _pciDevices[MAX_PCI_DEVS] = {};
    unsigned int _pciDeviceCount = 0;

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
        {0x1002, 0x5046, "[AMD/ATI] Rage 4 [Rage 128 PRO AGP 4X TMDS]"},
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
        {0x8086, 0x5845, "Intel Corporation. QEMU NVM Express Controller"},
        {0x1022, 0x1452, "[AMD] Family 17h (Models 00h-0fh) PCIe Dummy Host Bridge"},
        {0x1022, 0x1453, "[AMD] Family 17h (Models 00h-0fh) PCIe GPP Bridge"},
        {0x1022, 0x43c7, "[AMD] 400 Series Chipset PCIe Port"},
        {0x1022, 0x1454, "[AMD] Family 17h (Models 00h-0fh) Internal PCIe GPP Bridge 0 to Bus B"},
        {0x1022, 0x790b, "[AMD] FCH SMBus Controller"},
        {0x1022, 0x790e, "FCH LPC Bridge"},
        {0x1022, 0x1460, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 0"},
        {0x1022, 0x1461, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 1"},
        {0x1022, 0x1462, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 2"},
        {0x1022, 0x1463, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 3"},
        {0x1022, 0x1464, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 4"},
        {0x1022, 0x1465, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 5"},
        {0x1022, 0x1466, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 6"},
        {0x1022, 0x1467, "Family 17h (Models 00h-0fh) Data Fabric: Device 18h; Function 7"},
        {0x1022, 0x43d5, "400 Series Chipset USB 3.1 XHCI Controller"},
        {0x1022, 0x1450, "Family 17h (Models 00h-0fh) Root Complex"},
        {0x1022, 0x43c8, "400 Series Chipset SATA Controller"},
        {0x1022, 0x43c6, "400 Series Chipset PCIe Bridge"}
    };

    uint32_t read_config_dword(uint32_t bus, uint32_t device, uint32_t function, uint32_t offset)
    {
        uint32_t address = (offset | (function << 8) | (device << 11) | (bus << 16)) & 0x80ffffff;
        address |= (1 << 31);

        std::printf("The PCI address: %llx\n", (uint64_t)address);

        Processor::outl(0xcf8, address);
        return Processor::inl(0xCFC);
    }

    const char* dev_to_name(uint16_t vendorID, uint16_t deviceID)
    {
        int numDevices = sizeof(pciDeviceDatabase) / sizeof(pciDeviceDatabase[0]);
        for (int i = 0; i < numDevices; i++) {
            if (pciDeviceDatabase[i].vendorID == vendorID && pciDeviceDatabase[i].deviceID == deviceID) {
                return pciDeviceDatabase[i].deviceName;
            }
        }
        return "Unknown Device"; // Return a default name for unknown devices
    }

    void print_devices()
    {
        std::printf("Devices: ");
        for (unsigned int i = 0; _pciDeviceCount > i; i++)
        {
            uint16_t venID = _pciDevices[i].vendorID;
            uint16_t devID = _pciDevices[i].deviceID;
            const char* devName = dev_to_name(venID, devID);

            std::printf("Bus:%x Dev:%x Func:%d VenID:%x DevID:%x DevName:%s\n",
               _pciDevices[i].bus, _pciDevices[i].device, _pciDevices[i].function, venID, devID, devName);
        }
    }

    void add_devices(uint16_t vendorID, uint16_t deviceID, uint8_t bus, uint8_t device, uint8_t function, uint8_t _class, uint8_t subclass)
    {
        if (_pciDeviceCount > MAX_PCI_DEVS)
        {
            // oops.
            return;
        }

        _pciDevices[_pciDeviceCount].vendorID = vendorID;
        _pciDevices[_pciDeviceCount].deviceID = deviceID;
        _pciDevices[_pciDeviceCount].bus = bus;
        _pciDevices[_pciDeviceCount].device = device;
        _pciDevices[_pciDeviceCount].function = function;
        _pciDevices[_pciDeviceCount]._class = _class;
        _pciDevices[_pciDeviceCount].subclass = subclass;

        _pciDeviceCount++;
    }

    void print_full_device_info(uint32_t bus, uint32_t device, uint32_t function)
    {

    }

    void scanPCI()
    {
        for (uint8_t bus = 0; bus < 8; bus++)
        {
            for (uint8_t device = 0; device < 32; device++)
            {
                for (uint8_t function = 0; function < 8; function++)
                {
                    uint32_t vendor_id = read_config_dword(bus, device, function, 0);
                    if (vendor_id == 0xffffffff)
                    {
                        continue;
                    }


                    // device found
                    uint32_t classID = read_config_dword(bus, device, function, 0x8);
                    uint8_t classCode = (classID >> 24) & 0xff;
                    uint8_t subclassCode = (classID >> 16) & 0xff;

                    PCI::add_devices((uint16_t)(vendor_id & 0xffff), (uint16_t)((vendor_id >> 16) & 0xffff), bus, device, function, classCode, subclassCode); 
                }
            }
        }
    }
}