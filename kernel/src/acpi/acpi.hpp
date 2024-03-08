#ifndef ACPI_HPP
#define ACPI_HPP

#include <stdint.h>
#include <limine.h>

struct xsdp_t
{
    char signature[8];
    char oemid[6];
    uint8_t checksum;
    uint8_t revision;
    uint32_t rsdt_address;

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved;
} __attribute__((packed));

struct acpi_sdt_header
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));


#define RSDP_VERSION_1 0
#define RSDP_NEW_VERSION 2

namespace Acpi
{
    void acpi_initialize();
};

#endif