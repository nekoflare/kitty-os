#include <acpi/acpi.hpp>

#include <stdint.h>
#include <libc/stdio.hpp>

namespace Acpi
{
    struct limine_rsdp_request rsdp_request = {
        .id = LIMINE_RSDP_REQUEST,
        .revision = 0
    };

    limine_rsdp_response* rsdp_response = nullptr;
    xsdp_t* xSDP = nullptr;
    acpi_sdt_header* rsdt = nullptr;
    uint64_t num_entries = 0;

    void acpi_initialize()
    {
        if (rsdp_request.response == nullptr)
        {
            return;
        }

        rsdp_response = rsdp_request.response;
        uint64_t rsdp_address = (uint64_t)(rsdp_response->address);
        std::printf("RSDP has been found on address: %llx\n", rsdp_address);
    
        xSDP = (xsdp_t*) rsdp_response->address;

        if (xSDP->revision == RSDP_VERSION_1)
        {
            std::printf("It's using old revsion 1.0\n");

            uint64_t rsdt_address = (uint64_t)xSDP->rsdt_address;
            std::printf("RSDT address: %llx\n", rsdt_address);

            rsdt = (acpi_sdt_header*)xSDP->rsdt_address;

            if (rsdt == nullptr) return;
            if (xSDP == nullptr) return;

            // ERROR!
            num_entries = (rsdt->length - sizeof(acpi_sdt_header)) / 4;

            // std::printf("ACPI Entries: %d\n", num_entries);
        }
        else 
        {
            std::printf("It's using new version, 2.0 and above.\n");
        }

    }
};