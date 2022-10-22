import numpy as np

image = open("flash_expariments/big.img", "rb")

def list_to_string(list):
    return ",".join(map(lambda it: str(it), list))

# First, generate first boot sector
boots = image.read(512)
boots = list(boots)
print(f"uint8_t boots[] = {'{'}{list_to_string(boots)}{'}'};")

