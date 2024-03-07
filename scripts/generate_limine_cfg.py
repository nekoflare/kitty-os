import os

def list_files(directory):
    files_list = []
    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            # Remove the leading './files/' from the file path
            file_path = file_path.replace(directory, '', 1)
            files_list.append(file_path)
    return files_list

def generate_name(path):
    # Replace directory separator with dot
    return path.replace(os.path.sep, '$')

directory = './files/'
files = list_files(directory)

#for file in files:
#    print(generate_name(file))

with open("limine.cfg", "w") as f:
    f.write("# Timeout in seconds that Limine will use before automatically booting.\n")
    f.write("TIMEOUT=5\n")
    f.write("\n")
    f.write("# The entry name that will be displayed in the boot menu.\n")
    f.write(":Kitty OS\n")
    f.write("\t# We use the Limine boot protocol.\n")
    f.write("\tPROTOCOL=limine\n")
    f.write("\tKASLR=no\n")
    f.write("\n")

    for file in files:
        gen_name = generate_name(file)
        f.write(f'\tMODULE_PATH=boot:///{file}\n')
        f.write(f'\tMODULE_CMDLINE={gen_name}\n');
    
    f.write('\n')
        

    f.write("\tKERNEL_PATH=boot:///kernel\n")