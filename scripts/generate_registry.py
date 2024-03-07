last_build = 0

with open("last_build", "r") as f:
    last_build = int(f.read())
    last_build += 1

with open("last_build", "w") as f:
    f.write(str(last_build))

with open("files/registry/kernel/SysInfo.kr", "w") as f:
    f.write("KernelVerbose=1\n");
    f.write("SystemName=\"KittyOS\"\n");
    f.write(f'SystemVersion="0.{last_build} beta"');
