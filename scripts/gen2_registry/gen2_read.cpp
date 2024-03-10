#include <iostream>
#include <fstream>
#include <string>

namespace Gen2Reg
{
    enum EntryType : uint64_t
    {
        STRING = 0,
        BYTE = 1,
        WORD = 2,
        DWORD = 3,
        QWORD = 4,
        REGISTRY_TREE = 5,
        UNICODE_STRING = 6,
        FLOAT = 7,
        DOUBLE = 8,
        BOOL = 9,
    };

    struct Header
    {
        char        Name[4];
        uint64_t    EntryCount;
        uint16_t    OffsetToEntries;
    };

    struct Entry
    {
        uint16_t            NameLength;
        uint16_t            TreeNameLength;
        uint32_t            DataLength;
        Gen2Reg::EntryType  EntryType;
        const char*         Tree;
        const char*         EntryName;
        const char*         EntryData;
    };
};

using namespace Gen2Reg;

int main()
{
    std::ifstream v2_registry("sys_registry", std::ios::binary);

    if (!v2_registry.is_open())
    {
        std::cerr << "[ERROR] Failed to open registry file" << std::endl;
        return 1;
    }

    Gen2Reg::Header hdr;
    v2_registry.read(reinterpret_cast<char*>(&hdr), sizeof(Gen2Reg::Header));

    

    v2_registry.close();

    return 0;
}
