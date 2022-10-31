import numpy as np

image = open("big.img", "rb")

def list_to_string(list):
    return ",".join(map(lambda it: str(it), list))

# First, generate first boot sector
boots = image.read(0xe00)
boots = list(boots)
print(f"uint8_t boots[] = {'{'}{list_to_string(boots)}{'}'};")

