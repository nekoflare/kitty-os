def convert_to_struct(input_file, output_file):
    with open(input_file, 'r') as f:
        lines = f.readlines()

    pci_devices = []

    for line in lines:
        line = line.strip()
        if line.startswith('device'):
            parts = line.split('\t')
            vendor = parts[0].split()[1]
            device = parts[1].split()[1]
            name = parts[1].split()[2]
            pci_devices.append(f'{{0x{vendor}, 0x{device}, "{name}"}}')

    with open(output_file, 'w') as f:
        f.write("struct PCIDeviceInfo pciDeviceDatabase[] = {\n")
        for device in pci_devices:
            f.write("    " + device + ",\n")
        f.write("};\n")

input_file = "pci.ids"
output_file = "pci.ids.cpp"
convert_to_struct(input_file, output_file)
