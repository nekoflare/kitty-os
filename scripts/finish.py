import os

last_build = 0
with open("last_build", "r") as f:
    last_build = int(f.read())

os.system(f'cp template.iso builds/{str(last_build)}.build.iso')