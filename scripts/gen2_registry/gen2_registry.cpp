#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> // For perror
#include <cstring> // for strcpy

namespace Gen2Reg
{
    enum EntryType : uint64_t
    {
        STRING = 0,         // Stream of bytes.
        BYTE = 1,           // 8-bit value
        WORD = 2,           // 16-bit value
        DWORD = 3,          // 32-bit value
        QWORD = 4,          // 64-bit value
        REGISTRY_TREE = 5,  // It's like a folder
        UNICODE_STRING = 6, // Unicode string.
        FLOAT = 7,          // Float
        DOUBLE = 8,         // Double
        BOOL = 9,           // Boolean
    };

    struct Header
    {
        char        Name[4];        // G2RG
        uint64_t    EntryCount;     // Depends.
        uint16_t    OffsetToEntries;    // Offset from the beginning of Header to the entries of registry. sizeof(Header) is default.
    } __attribute__((packed));

    struct Entry
    {
        uint16_t            NameLength;      // Length of the name (includes null-byte).
        uint16_t            TreeNameLength;  // Length of the tree name.
        uint32_t            DataLength;      // Length of the data.
        Gen2Reg::EntryType  EntryType;       // Entry type.
        const char*         Tree;            // Tree name that this entry belongs to.
        const char*         EntryName;       // This must be treated. The name of the entry, if it's the registry tree then it's name of the tree.
        const char*         EntryData;       // This must be treated. This will have nothing if the entry type is registry tree. If it's registry tree then it's permission field(QWORD)/
    } __attribute((packed));
};

float           test_float      = 100.200f;
double          test_double     = 925.748;
unsigned int    verbosity_level = 3;   // Max verbosity level.

Gen2Reg::Entry entries[] = {
    {4,  0,  0,                     Gen2Reg::EntryType::REGISTRY_TREE,  "",            "ROOT",                   "00000000"},
    {11, 4,  0,                     Gen2Reg::EntryType::REGISTRY_TREE,  "ROOT",        "SKEY_KERNEL",            "00000000"},
    {11, 11, 8,                     Gen2Reg::EntryType::STRING,         "SKEY_KERNEL", "SYSTEM_NAME",            "Kitty OS"},
    {14, 11, 8,                     Gen2Reg::EntryType::STRING,         "SKEY_KERNEL", "SYSTEM_VERSION",         "1.0 Beta"},
    {22, 11, sizeof(unsigned int),  Gen2Reg::EntryType::DWORD,          "SKEY_KERNEL", "SYSTEM_VERBOSITY_LEVEL", (const char*)&verbosity_level},
    
    // Here are some test things.
    {4,  0, 0,               Gen2Reg::EntryType::REGISTRY_TREE, "",     "TEST",        "00000000"},
    {11, 4, 11,              Gen2Reg::EntryType::STRING,        "TEST", "STRING_TEST", "Test String"},
    {11, 4, sizeof(double),  Gen2Reg::EntryType::DOUBLE,        "TEST", "DOUBLE_TEST", (const char*)&test_double},
    {10, 4, sizeof(float),   Gen2Reg::EntryType::FLOAT,         "TEST", "FLOAT_TEST",  (const char*)&test_float},
};

using namespace Gen2Reg;

int main(void)
{
    std::ofstream v2_registry("sys_registry", std::ios::binary);

    std::cout << "Float size: " << sizeof(float) << std::endl;

    if (!v2_registry.is_open())
    {
        std::perror("[CRIT] Failed to open registry file");
        return 1;
    }

    Gen2Reg::Header hdr; 
    std::strcpy(hdr.Name, "G2RG");
    hdr.EntryCount = sizeof(entries) / sizeof(entries[0]);
    hdr.OffsetToEntries = sizeof(Gen2Reg::Header);  

    // Write header to the file.
    v2_registry.write(reinterpret_cast<const char*>(&hdr), sizeof(Gen2Reg::Header));

    // Write entries to the file.
    for (const auto& entry : entries)
    {
        v2_registry.write(reinterpret_cast<const char*>(&entry.NameLength), sizeof(entry.NameLength) + 1); // Plus 1 for the null byte.
        v2_registry.write(reinterpret_cast<const char*>(&entry.TreeNameLength), sizeof(entry.TreeNameLength) + 1);
        v2_registry.write(reinterpret_cast<const char*>(&entry.DataLength), sizeof(entry.DataLength) + 1);
        v2_registry.write(reinterpret_cast<const char*>(&entry.EntryType), sizeof(entry.EntryType));
        v2_registry.write(entry.Tree, entry.TreeNameLength + 1);        // Plus 1 for the null byte.
        v2_registry.write(entry.EntryName, entry.NameLength + 1);

        if (entry.EntryType == Gen2Reg::EntryType::REGISTRY_TREE)
        {
            v2_registry.write(entry.EntryData, 8);
        }
        else 
        {
            if (entry.EntryType == Gen2Reg::EntryType::STRING)
            {
                v2_registry.write(entry.EntryData, entry.DataLength + 1);
            }
            else 
            {
                v2_registry.write(entry.EntryData, entry.DataLength);
            }
        }
    }

    v2_registry.close();

    return 0;
}