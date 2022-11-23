#include <stdint.h>
#include <stdio.h>

#include <cstring>

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

uint32_t addr_from(FileEntry_t file) {
  int cluster =
      (file.startingClusterHighBytes << 16) | file.startingClusterLowBytes;
  cluster -= 3;
  return 0x7f000 + 0x1000 * cluster;
}

void write_metadata_text(FILE *pFile, FlightInFlash *pFlight) {
  fseek(pFile, addr_from(pFlight->metadataFile), SEEK_SET);

  char str[512];
  int offset = 0;
  int string_len =
      snprintf(str, sizeof(str), "Flight starts: %2d/%2d/%2d %2d:%2d:%2d\n",
               pFlight->actualDataFile.createDate.months,
               pFlight->actualDataFile.createDate.days,
               pFlight->actualDataFile.createDate.years,
               pFlight->actualDataFile.createTime.hours,
               pFlight->actualDataFile.createTime.minutes,
               pFlight->actualDataFile.createTime.seconds_periods / 2);
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  string_len =
      snprintf(str, sizeof(str), "Flight ends:   %2d/%2d/%2d %2d:%2d:%2d\n",
               pFlight->actualDataFile.createDate.months,
               pFlight->actualDataFile.createDate.days,
               pFlight->actualDataFile.createDate.years,
               pFlight->actualDataFile.createTime.hours,
               pFlight->actualDataFile.createTime.minutes,
               pFlight->actualDataFile.createTime.seconds_periods / 2);
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  string_len = snprintf(str, sizeof(str), "Duration:      %u seconds\n",
                        pFlight->flightDurationSeconds);
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  string_len = snprintf(str, sizeof(str), "Launched?      %s\n",
                        pFlight->launched ? "YES" : "NO");
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  string_len =
      snprintf(str, sizeof(str), "Max altitude:  %f\n", pFlight->maxAltitude);
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  string_len =
      snprintf(str, sizeof(str), "Max velocity:  %f\n", pFlight->maxVelocity);
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  string_len =
      snprintf(str, sizeof(str), "Max accel:     %f\n", pFlight->maxAccel);
  fwrite(str, strlen(str), 1, pFile);
  offset += string_len;

  pFlight->metadataFile.fileSizeBytes = offset;
}

int main() {
  FILE *pFile;
  pFile = fopen("fat32_test.img", "wb");  // w for write, b for binary

  // Need to write a bunch of 0s for the rest of the file
  static uint8_t zeros[512 * 64] = {0};
  std::memset(zeros, 0, sizeof(zeros));
  for (int i = 0; i < 0x1e848000; i += 512 * 64) {
    fwrite(zeros, sizeof(zeros), 1, pFile);
  }

  fseek(pFile, 0, SEEK_SET);
  fwrite(bootsector, sizeof(bootsector), 1, pFile);

  fseek(pFile, 0x4000, SEEK_SET);
  uint8_t fat[] = {0xf8, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
                   0xf8, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f};
  fwrite(fat, sizeof(fat), 1, pFile);

  // Write root directory
  fseek(pFile, 0x7e000, SEEK_SET);
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
  fwrite(&entry, sizeof(entry), 1, pFile);

  FileEntry_t data = {0};
  data.createTime = {
      .seconds_periods = 10,
      .minutes = 1,
      .hours = 3,
  };
  data.createDate = {.days = 22, .months = 10, .years = 2022 - 1980};

  FileEntry_t meta2 = {
      .filename = {'0', '2', '_', 'm', 'e', 't', 'a', ' '},
      .extension =
          {
              't',
              'x',
              't',
          },
      .attributes = 0,
      .reserved = {0},
      .creationTimeTenths = 0,
      .createTime =
          {
              .seconds_periods = 0,
              .minutes = 0,
              .hours = 0,
          },
      .createDate = {.days = 22, .months = 10, .years = 2022 - 1980},
      .accessDate = {.days = 22, .months = 10, .years = 2022 - 1980},
      .startingClusterHighBytes = 0,
      .modifyTime =
          {
              .seconds_periods = 0,
              .minutes = 36,
              .hours = 14,  // gmt, I think -- set to 14 to make 10am ET
          },
      .modifyDate = {.days = 22, .months = 10, .years = 2022 - 1980},
      .startingClusterLowBytes = 3,
      .fileSizeBytes = 32};

  FileEntry_t meta = {
      .filename = {'0', '1', '_', 'm', 'e', 't', 'a', ' '},
      .extension =
          {
              't',
              'x',
              't',
          },
      .attributes = 0,
      .reserved = {0},
      .creationTimeTenths = 0,
      .createTime =
          {
              .seconds_periods = 0,
              .minutes = 0,
              .hours = 0,
          },
      .createDate = {.days = 22, .months = 10, .years = 2022 - 1980},
      .accessDate = {.days = 22, .months = 10, .years = 2022 - 1980},
      .startingClusterHighBytes = 0,
      .modifyTime =
          {
              .seconds_periods = 0,
              .minutes = 36,
              .hours = 14,  // gmt, I think -- set to 14 to make 10am ET
          },
      .modifyDate = {.days = 22, .months = 10, .years = 2022 - 1980},
      .startingClusterLowBytes = 4,
      .fileSizeBytes = 32};

  FlightInFlash flash_flight = {.metadataFile = meta,
                                .actualDataFile = data,
                                .launched = true,
                                .flightDurationSeconds = 69,
                                .maxAltitude = 420,
                                .maxVelocity = 1234,
                                .maxAccel = 666};
  FlightInFlash flash_flight2 = {.metadataFile = meta2,
                                 .actualDataFile = data,
                                 .launched = false,
                                 .flightDurationSeconds = 12,
                                 .maxAltitude = 111,
                                 .maxVelocity = 321,
                                 .maxAccel = 123};

  // This changes the size of the metadata file poitner within flash_flight
  write_metadata_text(pFile, &flash_flight);
  write_metadata_text(pFile, &flash_flight2);

  // Seek back to the right spot in the FAT to write our metadata entry
  fseek(pFile, 0x7e000 + sizeof(entry), SEEK_SET);
  fwrite(&flash_flight.metadataFile, sizeof(flash_flight.metadataFile), 1,
         pFile);

  // same for second metadata entry
  fseek(pFile, 0x7e000 + sizeof(entry) * 2, SEEK_SET);
  fwrite(&flash_flight2.metadataFile, sizeof(flash_flight2.metadataFile), 1,
         pFile);

  // Write the first fake actual flight file file
  // fseek(pFile, addr_from(data), SEEK_SET);
  // char file1[32] = "helloooooooasdf";
  // fwrite(&file1, sizeof(file1), 1, pFile);
}
