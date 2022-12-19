#include <stdint.h>
#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>

#include "tasks/data_log.h"

// Looks like two copies of the boot sector?? interesting
uint8_t bootsector[] = {
    235, 88,  144, 109, 107, 102, 115, 46,  102, 97,  116, 0,   2,   8,   32,
    0,   1,   0,   0,   0,   0,   248, 0,   0,   32,  0,   64,  0,   0,   0,
    0,   0,   64,  66,  15,  0,   208, 3,   0,   0,   0,   0,   0,   0,   2,
    0,   0,   0,   1,   0,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   128, 0,   41,  38,  132, 148, 221, 69,  77,  66,  69,
    68,  68,  69,  68,  32,  70,  83,  70,  65,  84,  51,  50,  32,  32,  32,
    14,  31,  190, 119, 124, 172, 34,  192, 116, 11,  86,  180, 14,  187, 7,
    0,   205, 16,  94,  235, 240, 50,  228, 205, 22,  205, 25,  235, 254, 84,
    104, 105, 115, 32,  105, 115, 32,  110, 111, 116, 32,  97,  32,  98,  111,
    111, 116, 97,  98,  108, 101, 32,  100, 105, 115, 107, 46,  32,  32,  80,
    108, 101, 97,  115, 101, 32,  105, 110, 115, 101, 114, 116, 32,  97,  32,
    98,  111, 111, 116, 97,  98,  108, 101, 32,  102, 108, 111, 112, 112, 121,
    32,  97,  110, 100, 13,  10,  112, 114, 101, 115, 115, 32,  97,  110, 121,
    32,  107, 101, 121, 32,  116, 111, 32,  116, 114, 121, 32,  97,  103, 97,
    105, 110, 32,  46,  46,  46,  32,  13,  10,  0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    85,  170, 82,  82,  97,  65,  0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   114, 114, 65,  97,  201, 231, 1,   0,   2,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   85,  170, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   235, 88,  144,
    109, 107, 102, 115, 46,  102, 97,  116, 0,   2,   8,   32,  0,   1,   0,
    0,   0,   0,   248, 0,   0,   32,  0,   64,  0,   0,   0,   0,   0,   64,
    66,  15,  0,   208, 3,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,
    1,   0,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   128, 0,   41,  38,  132, 148, 221, 69,  77,  66,  69,  68,  68,  69,
    68,  32,  70,  83,  70,  65,  84,  51,  50,  32,  32,  32,  14,  31,  190,
    119, 124, 172, 34,  192, 116, 11,  86,  180, 14,  187, 7,   0,   205, 16,
    94,  235, 240, 50,  228, 205, 22,  205, 25,  235, 254, 84,  104, 105, 115,
    32,  105, 115, 32,  110, 111, 116, 32,  97,  32,  98,  111, 111, 116, 97,
    98,  108, 101, 32,  100, 105, 115, 107, 46,  32,  32,  80,  108, 101, 97,
    115, 101, 32,  105, 110, 115, 101, 114, 116, 32,  97,  32,  98,  111, 111,
    116, 97,  98,  108, 101, 32,  102, 108, 111, 112, 112, 121, 32,  97,  110,
    100, 13,  10,  112, 114, 101, 115, 115, 32,  97,  110, 121, 32,  107, 101,
    121, 32,  116, 111, 32,  116, 114, 121, 32,  97,  103, 97,  105, 110, 32,
    46,  46,  46,  32,  13,  10,  0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   85,  170};

typedef struct __attribute__((packed)) {
  unsigned int days : 5;    // 1-31
  unsigned int months : 4;  // 1-12
  unsigned int years : 7;   // Years since 1980
} FATdate_t;

typedef struct __attribute__((packed)) {
  unsigned int seconds_periods : 5;  // 2 second resolution
  unsigned int minutes : 6;          // 0-59
  unsigned int hours : 5;            // 0-23
} FATtime_t;

enum class FileAttributes : uint8_t {
  READ_ONLY = 1,
  HIDDEN = 2,
  SYSTEM_FILE = 4,
  VOLUME_LABEL = 8,
  SUBDIRECTORY = 0x10,
  ARCHIVE = 0X20,
};

typedef struct __attribute__((packed)) {
  char filename[8];
  char extension[3];
  uint8_t attributes;

  // 10 bytes reserved, total
  uint8_t reserved[1];

  uint8_t creationTimeTenths;
  FATtime_t createTime;
  FATdate_t createDate;
  FATdate_t accessDate;

  // fat32-specific
  uint16_t startingClusterHighBytes;

  FATtime_t modifyTime;
  FATdate_t modifyDate;
  uint16_t startingClusterLowBytes;
  uint32_t fileSizeBytes;
} FileEntry_t;

struct FlightInFlash {
  // "pointer" to where the flight actually exists in onboard flash
  int flightNumber;
  uint32_t internalFlashStartAddress;
  uint32_t internalFlashOnePastEndAddress;

  uint32_t
      metadataCluster;  // The cluster in MSC address land occupied by metadata
  uint32_t
      firstFlightCluster;  // The first FAT cluster this flight "occupies". The
                           // FAT entry here should point to first + 1, and that
                           // to first + 2... up to the last. Note that the
                           // first file will occupy sector #2, technically
  uint32_t lastFlightCluster;  // The last FAT cluster this flight "occupies".
                               // FAT entry for this cluster should be the end
                               // of cluster marker.

  uint32_t
      flightDataFirstFatEntryIdx;  // Index in the FAT of the first flight data
                                   // entry. The flight has (lastFATcluster -
                                   // firstFATcluster) + 1 many entries in the

  // We expect the entries to have correct date/time
  FileEntry_t metadataFile;
  FileEntry_t actualDataFile;

  // If the rocket launched
  bool launched;

  // Total flight duration, in seconds
  uint32_t flightDurationSeconds;

  // More misc metadata that might be helpful to know
  float maxAltitude;
  float maxVelocity;
  float maxAccel;
};

#define MAX_FLIGHTS 256
FlightInFlash flightArray[MAX_FLIGHTS] = {0};
int maxFlightNum;

#define CLUSTER_SIZE_BYTES 0x1000

#define FAT_START_ADDRESS 0x4000

#define ROOT_DIRECTORY_ADDR 0x7e000
#define FIRST_FILE_ADDRESS 0x7f000
#define FIRST_FLIGHT_CLUSTER_NUM 3

/**
 * @param entry The entry to assign date/times to
 * @param gpsTimeStart Seconds since unix epoch of first packet of the flight
 * @param gpsTimeEnd Seconds since unix epoch of last packet of the flight
 */
void updateFileTimestamp(FileEntry_t *entry, int64_t gpsTimeStart,
                         int64_t gpsTimeEnd) {
  tm *dt = gmtime(&gpsTimeStart);
  if (dt && dt->tm_year > 20 && dt->tm_year < 300) {
    entry->createDate.years = dt->tm_year + 1900 - 1980;
    entry->createDate.months = dt->tm_mon;
    entry->createDate.days = dt->tm_mday;
    entry->createTime.hours = dt->tm_hour;
    entry->createTime.minutes = dt->tm_min;
    entry->createTime.seconds_periods = dt->tm_sec / 2;
    entry->creationTimeTenths = 0;
  }

  dt = gmtime(&gpsTimeEnd);
  if (dt && dt->tm_year > 20 && dt->tm_year < 300) {
    entry->modifyDate.years = dt->tm_year + 1900 - 1980;
    entry->modifyDate.months = dt->tm_mon;
    entry->modifyDate.days = dt->tm_mday;
    entry->modifyTime.hours = dt->tm_hour;
    entry->modifyTime.minutes = dt->tm_min;
    entry->modifyTime.seconds_periods = dt->tm_sec / 2;
  }
}

/**
 * So what's really annoying about this is we don't have a way of knowing the
 * file size before we try writing it. We could run this function while caching
 * all the flights, or (lmao lol rofl) we could just create a file 512 bytes
 * long, padded with blank ASCII braile characters. I'm lazy, so let's do that
 *
 * @param pFile
 * @param pFlight
 */
void write_metadata_text(FlightInFlash *pFlight, char *pSectorStart) {
  const unsigned int sector_size = 512;
  int string_len = 0;
  string_len +=
      snprintf(pSectorStart, sector_size - string_len,
               "Flight starts: %2d/%2d/%2d %2d:%2d:%2f\n",
               pFlight->actualDataFile.createDate.months,
               pFlight->actualDataFile.createDate.days,
               pFlight->actualDataFile.createDate.years + 1980,
               pFlight->actualDataFile.createTime.hours,
               pFlight->actualDataFile.createTime.minutes,
               pFlight->actualDataFile.createTime.seconds_periods / 2.0);

  string_len +=
      snprintf(pSectorStart + string_len, sector_size - string_len,
               "Flight ends:   %2d/%2d/%2d %2d:%2d:%2f\n",
               pFlight->actualDataFile.createDate.months,
               pFlight->actualDataFile.createDate.days,
               pFlight->actualDataFile.createDate.years + 1980,
               pFlight->actualDataFile.createTime.hours,
               pFlight->actualDataFile.createTime.minutes,
               pFlight->actualDataFile.createTime.seconds_periods / 2.0);

  string_len +=
      snprintf(pSectorStart + string_len, sector_size - string_len,
               "Duration:      %u seconds\n", pFlight->flightDurationSeconds);

  string_len +=
      snprintf(pSectorStart + string_len, sector_size - string_len,
               "Launched?      %s\n", pFlight->launched ? "YES" : "NO");

  string_len += snprintf(pSectorStart + string_len, sector_size - string_len,
                         "Max altitude:  %f\n", pFlight->maxAltitude);

  string_len += snprintf(pSectorStart + string_len, sector_size - string_len,
                         "Max velocity:  %f\n", pFlight->maxVelocity);

  string_len += snprintf(pSectorStart + string_len, sector_size - string_len,
                         "Max accel:     %f\n", pFlight->maxAccel);

  // Pad rest of file with a bunch of spaces
  memset(pSectorStart + string_len, ' ', sector_size - string_len);
}

// Update the list of flights in the flightArray based on internal flash state
void mapFlashToClusters() {
  int bound = dataLog_getLastFlightNum() - 1;
  printf("Found %i flights\n", bound);

  // The first flight will have metadata at cluster 3, and
  // flight at 4-???
  int firstFreeCluster = 3;

  for (int i = 0; i < bound; i++) {
    const int datalog_flight_idx = i + 1;
    uint32_t firstSector, lastSector;
    dataLog_getFlightSectors(datalog_flight_idx, &firstSector, &lastSector);

    // Sector should be >0 always
    if (firstSector == 0 || lastSector == 0) {
      continue;
    }

    FlightInFlash &flight = flightArray[i];
    flight.flightNumber = i;
    flight.internalFlashStartAddress = firstSector * FLASH_MAX_SECTOR_BYTES;
    flight.internalFlashOnePastEndAddress =
        (lastSector + 1) * FLASH_MAX_SECTOR_BYTES + 1;
    flight.actualDataFile.fileSizeBytes =
        flight.internalFlashOnePastEndAddress -
        flight.internalFlashStartAddress;

    dataLog_readFlightNumMetadata(datalog_flight_idx);
    FlightMetadata_s *meta = dataLog_getFlightMetadata();

    uint64_t startTime = meta->gpsTimestamp;  // seconds
    uint64_t duration =
        dataLog_getLastFlightTimestamp(datalog_flight_idx) / 1000;
    uint64_t endTime = startTime + duration;
    updateFileTimestamp(&flight.actualDataFile, startTime, endTime);
    updateFileTimestamp(&flight.metadataFile, startTime, endTime);

    // Also set filename and jazz
    char buff[8] = {0};
    snprintf(buff, sizeof(buff), "%i_meta", i);
    memcpy(flight.metadataFile.filename, buff, sizeof(buff));
    snprintf(buff, sizeof(buff), "%i_data", i);
    memcpy(flight.actualDataFile.filename, buff, sizeof(buff));
    char txt_ext[3] = {'t', 'x', 't'};
    char hex_ext[3] = {'h', 'e', 'x'};
    memcpy(flight.metadataFile.extension, txt_ext, sizeof(txt_ext));
    memcpy(flight.actualDataFile.extension, hex_ext, sizeof(hex_ext));

    // Figure out what clusters to allocate to which part of the flight
    flight.metadataCluster = firstFreeCluster;
    flight.firstFlightCluster = firstFreeCluster + 1;

    uint64_t flightSizeBytes =
        (lastSector - firstSector + 1) * FLASH_MAX_SECTOR_BYTES;
    // Need to round _up_ to get # of sectors in MSC land
    const uint64_t numClusters =
        (int)ceil(flightSizeBytes / CLUSTER_SIZE_BYTES);
    flight.lastFlightCluster = flight.firstFlightCluster + numClusters;

    // Update the actual FileEntry as well
    // TODO this assumes 32-bit size, b/c am lazy. should be sufficient,
    // technically
    flight.metadataFile.startingClusterHighBytes = 0;
    flight.metadataFile.startingClusterLowBytes = flight.metadataCluster;

    flight.actualDataFile.startingClusterHighBytes = 0;
    flight.actualDataFile.startingClusterLowBytes = flight.firstFlightCluster;

    // Hard coded, coz it's padded with spaces at the end, lol
    flight.metadataFile.fileSizeBytes = 512;

    printf("Meta cluster: %i data cluster %i\n",
           flight.metadataFile.startingClusterLowBytes,
           flight.actualDataFile.startingClusterLowBytes);

    firstFreeCluster += numClusters;

    flight.launched = meta->launched;
    flight.flightDurationSeconds = duration;
  }
  maxFlightNum = bound;
}

// Get the flight # in flash from a given address in mass-storage-land
FlightInFlash *getFlightFromMSCaddress(uint32_t mscAddress) {
  uint32_t offsetFromDataStart = mscAddress - FIRST_FILE_ADDRESS;
  // FAT-cluster-numbers, so FIRST_FILE_ADDRESS actually occupies the third
  // cluster
  uint32_t clusterNumber =
      (offsetFromDataStart / CLUSTER_SIZE_BYTES) + FIRST_FLIGHT_CLUSTER_NUM;

  for (int i = 0; i < MAX_FLIGHTS; i++) {
    FlightInFlash *it = flightArray + i;
    if (clusterNumber >= it->firstFlightCluster &&
        clusterNumber <= it->lastFlightCluster) {
      return it;
    }
  }

  return NULL;
}

uint32_t clusterToMSCglobalAddress(uint32_t cluster) {
  return (cluster - FIRST_FLIGHT_CLUSTER_NUM) * CLUSTER_SIZE_BYTES +
         FIRST_FILE_ADDRESS;
}

/**
 * @brief Get the location of the first byte of a file, in MSC address space
 */
uint32_t getFileDataAddress(FileEntry_t file) {
  int cluster =
      (file.startingClusterHighBytes << 16) | file.startingClusterLowBytes;
  return clusterToMSCglobalAddress(cluster);
}

uint32_t mscAddressToClusterNumber(uint32_t mscAddress) {
  return (int)ceil((mscAddress - FIRST_FILE_ADDRESS) /
                   (int)CLUSTER_SIZE_BYTES) +
         FIRST_FLIGHT_CLUSTER_NUM;
}

/**
 * Find the corresponding data in physical flash for a given msc address,
 * and copy it into the cluster
 */
void copyFlighDataToCluster(uint32_t mscAddress, uint8_t *pCluster) {
  FlightInFlash *flight = getFlightFromMSCaddress(mscAddress);
  if (!flight) return;

  // The offset of the cluster, in bytes, relative to the start of the flight in
  // msc address space
  uint32_t mscClusterStartAddress_offsetFromStartOfFlight =
      mscAddress - clusterToMSCglobalAddress(flight->firstFlightCluster);

  // Fill pCluster with flash data
  dataLog_flashRead(flight->internalFlashStartAddress +
                        mscClusterStartAddress_offsetFromStartOfFlight,
                    512, pCluster);
}

#define MIN(a, b) a < b ? a : b
#define FAT_ENTRY_BYTES 4  // fat32

// This is the main function of the fake SD card. It handles creating the
// appropriate fake data based on cluster requested by host
// pCluster had better be >= 512 (one physical cluster)
void retrieveSector(uint32_t mscAddress, uint32_t lengthBytes,
                    uint8_t *pSector) {
  memset(pSector, 0xff, 512);

  // First blob of data: the boot sector. Never changes
  if (mscAddress < sizeof(bootsector)) {
    // We can accidentally read past the end of the boot sector
    // so try not to do that
    uint8_t *pBootsectorEnd = bootsector + sizeof(bootsector);

    // TODO do I need the -1
    memcpy(pSector, bootsector + mscAddress,
           (size_t)MIN(512, (unsigned long)(pBootsectorEnd - mscAddress - 1)));
    return;
  }

  // Second blob of data: the FAT
  else if ((mscAddress >= FAT_START_ADDRESS) &&
           mscAddress < ROOT_DIRECTORY_ADDR) {
    // Generate the FAT. We can safely assume that all flights are actually
    // contiguous in MSC memory
    // first 2 entries are [f8 ff ff 0f] and [ff ff ff 0f]
    // if a cluster is the last of a file, write [ff ff ff 0f]
    // if not, write little-endian the next cluster -- ex, [05, 00, 00, 00]

    static uint8_t eoc[] = {0xff, 0xff, 0xff, 0x0f};
    static uint8_t boot[] = {0xf8, 0xff, 0xff, 0x0f};

    // write boot and root dir entries
    offset = 0;
    if (mscAddress == FAT_START_ADDRESS) {
      // start with boot and root dir clusters
      memcpy(pSector, boot, sizeof(boot));
      memcpy(pSector + sizeof(boot), eoc, sizeof(eoc));
      offset = 8;
    }

    /*
    TODO so I started a really hacky impl, but it turns out it's a bit more involved.
    Flights in the fat aren't guaranteed to be aligned with the bounds of sectors, so
    a flight could start on one sector and continue onto the next. We should probably
    think more about the algorithm before just naively sending it, exp. coz the usb host
    can just call this function in an entirely arbitrary order.

    We should:
    - Find the cluster # of the first entry in our FAT
    - Find the Flight that should contain that cluster
    - Do the rest of the logic I already have below
    */

    int flightIdx = 0;
    while (offset < 512) {
      // Find the flight responsible for a given cluster

      // FAT32 -- each entry takes up 4 bytes
      auto clusterNum = offset / 4;

      auto &flight = flightArray[flightIdx];
      
      // metadata will always come first
      if (flight.metadataCluster == clusterNum) {
        memcpy(pSector + offset, eoc, sizeof(eoc));
        offset += sizeof(eoc);
      } else if (cluster >= flight.firstFlightCluster) {
        // Check if it's time to write end-of-chain yet
        if (cluster < flight.lastFlightCluster) {
          uint32_t next_cluster = cluster + 1;
          uint8_t entry[4] = {
            next_cluster & 0xff,
            (next_cluster >> 8) & 0xff,
            (next_cluster >> 16) & 0xff,
            (next_cluster >> 24) & 0xff,
          };
          memcpy(pSector + offset, entry, sizeof(eoc));
          offset += sizeof(entry);
        }
      }
    }

  }

  // Third blob of data: the root directory
  // Need to generate a 512-byte chunk of the root directory
  else if ((mscAddress >= ROOT_DIRECTORY_ADDR) &&
           mscAddress < FIRST_FILE_ADDRESS) {
    const bool add_root_dir = (mscAddress == ROOT_DIRECTORY_ADDR);
    if (add_root_dir) {
      FileEntry_t entry{
          .filename = {'E', 'M', 'B', 'E', 'D', 'D', 'E', 'D'},
          .extension =
              {
                  ' ',
                  'F',
                  'S',
              },
          .attributes = (char)FileAttributes::VOLUME_LABEL,
          .reserved = {0},
          .creationTimeTenths = 0,
          .createTime = {},
          .createDate = {},
          .accessDate = {},
          .startingClusterHighBytes = {0},
          .modifyTime =
              {
                  .seconds_periods = 0,
                  .minutes = 59,
                  .hours = 9,
              },
          .modifyDate = {.days = 22, .months = 10, .years = 2022 - 1980},
          .startingClusterLowBytes = 0,
          .fileSizeBytes = 0};

      memcpy(pSector + 0, &entry, sizeof(entry));

      uint32_t zeros[sizeof(entry)] = {0};
      memcpy(pSector + 32, zeros, sizeof(zeros));
    }

    // How far, in bytes, into the root directory we are
    const uint32_t offset = mscAddress - ROOT_DIRECTORY_ADDR;

    // We assume each dir entry takes up exactly 32 bits (no long file name
    // extension stupidity) the entry idx
    uint32_t firstEntryIdx = offset / sizeof(FileEntry_t) / 2;
    uint32_t lastEntryIdx = firstEntryIdx + (512 / sizeof(FileEntry_t) / 2);

    // Need to take out root dir entry we add above. This offsets everything
    // following by 2
    if (!add_root_dir) {
      firstEntryIdx -= 2;
    }
    lastEntryIdx -= 2;

    // We iterate forward 2 files each time, as we write a meta and actual file
    // for each flight
    for (int i = firstEntryIdx; i < lastEntryIdx; i++) {
      size_t clusterOffset = i * sizeof(FileEntry_t) + 64;
      const uint32_t flight_num = i;
      if (flight_num < maxFlightNum) {
        FlightInFlash *it = &(flightArray[flight_num]);

        // Write metadata file and flight data file
        memcpy(pSector + clusterOffset, &it->metadataFile, sizeof(FileEntry_t));
        memcpy(pSector + clusterOffset + sizeof(FileEntry_t),
               &it->actualDataFile, sizeof(FileEntry_t));
      }
    }

    return;
  } else if (mscAddress >= FIRST_FILE_ADDRESS) {
    // Search through all flights
    for (int i = 0; i < maxFlightNum; i++) {
      FlightInFlash &flight = flightArray[i];

      // Check if this cluster should contain the metadata file
      uint64_t metadataAddress = getFileDataAddress(flight.metadataFile);
      uint64_t flightAddress = getFileDataAddress(flight.actualDataFile);

      auto mscClusterNum = mscAddressToClusterNumber(mscAddress);

      if ((metadataAddress == mscAddress)) {
        // Contains the metadata file! We only have one sector with metadata
        // data in it, anyways
        write_metadata_text(&flight, (char *)pSector);
      } else if ((mscClusterNum >= flight.firstFlightCluster) &&
                 (mscClusterNum <= (flight.lastFlightCluster))) {
        // Within the flight data file! Though we aren't quite sure at what
        // offset because clusters are 0x1000 bytes, not 0x200 bytes, long.

        uint64_t offsetFromFlightStart = mscAddress - flightAddress;
        uint32_t bytesToRead = MIN(
            512, flight.internalFlashOnePastEndAddress - offsetFromFlightStart);
        hm_flashReadStart(
            0, flight.internalFlashStartAddress + offsetFromFlightStart,
            bytesToRead, pSector);
      }
    }
  }
}
