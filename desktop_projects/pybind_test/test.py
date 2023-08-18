from ctypes import *
so_file = "/home/matt/stm32-avionics/build/desktop_projects/pybind_test/libstm32-avionics-py.so"
my_functions = CDLL(so_file)
print((my_functions))

message_len = 48
def OutputSize(inLen: int):
    ret = 4 * (int(inLen / 2) + 1)
    return int(ret)

InArrayType = c_uint8 * message_len
OutArrayType = c_uint8 * OutputSize(message_len)
LenType = c_size_t

import time
start = time.time()

inarr = InArrayType()
outarr = OutArrayType()

inarr[0] = 3
inarr[1] = 1
inarr[2] = 2
inarr[3] = 3

my_functions.fec_encode(inarr, outarr, message_len)
outarr = list(outarr)

end = time.time() - start

print(list(inarr))
print(outarr)
print(f"took {round(end * 1e6)} uS!")
