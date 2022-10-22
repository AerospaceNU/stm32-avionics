#include <stdint.h>

uint8_t bootsector[] = {
    235, 60,  144, 109, 107, 102, 115, 46,  102, 97,  116, 0,   2,   64,  64,
    0,   1,   0,   4,   0,   0,   248, 128, 0,   63,  0,   32,  0,   0,   0,
    0,   0,   126, 132, 30,  0,   128, 0,   41,  22,  202, 41,  253, 69,  77,
    66,  69,  68,  68,  69,  68,  32,  70,  83,  70,  65,  84,  49,  54,  32,
    32,  32,  14,  31,  190, 91,  124, 172, 34,  192, 116, 11,  86,  180, 14,
    187, 7,   0,   205, 16,  94,  235, 240, 50,  228, 205, 22,  205, 25,  235,
    254, 84,  104, 105, 115, 32,  105, 115, 32,  110, 111, 116, 32,  97,  32,
    98,  111, 111, 116, 97,  98,  108, 101, 32,  100, 105, 115, 107, 46,  32,
    32,  80,  108, 101, 97,  115, 101, 32,  105, 110, 115, 101, 114, 116, 32,
    97,  32,  98,  111, 111, 116, 97,  98,  108, 101, 32,  102, 108, 111, 112,
    112, 121, 32,  97,  110, 100, 13,  10,  112, 114, 101, 115, 115, 32,  97,
    110, 121, 32,  107, 101, 121, 32,  116, 111, 32,  116, 114, 121, 32,  97,
    103, 97,  105, 110, 32,  46,  46,  46,  32,  13,  10,  0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    85,  170};

typedef struct __attribute__((packed)) {
    unsigned int days: 5; // 1-31
    unsigned int months: 4; // 1-12
    unsigned int years: 7; //Years since 1980
} FATdate_t;

typedef struct __attribute__((packed)) {
    unsigned int seconds_periods: 5; // 2 second resolution
    unsigned int minutes: 6; // 0-59
    unsigned int hours: 5; // 0-23
} FATtime_t;

enum class FileAttributes {
    READ_ONLY = 1,
    HIDDEN = 2,
    SYSTEM_FILE = 4,
    VOLUME_LABEL=8,
    SUBDIRECTORY=0x10,
    ARCHIVE=0X20,
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

    uint8_t reserved2[2]; // unused for fat16
    
    FATtime_t modifyTime;
    FATdate_t modifyDate;
    uint16_t startingCluster;
    uint32_t fileSizeBytes;
} FileEntry_t;

#include <stdio.h>

int main() {


    
    FILE *ptr;
    ptr = fopen("test.img","wb");  // w for write, b for binary

    // Need to write a bunch of 0s for the rest of the file
    for (int i = 0; i < 0x3d090000; i += 512 * 64) {
        static const uint8_t zeros[512*64] = {0};
        fwrite(zeros, sizeof(zeros), 1, ptr);
    }

    fseek(ptr, 0, SEEK_SET);
    fwrite(bootsector, 0x200, 1, ptr);

    fseek(ptr, 0x8000, SEEK_SET);
    uint8_t fat[] = {0xf8, 0xff, 0xff, 0xff, 0xff, 0xff};
    fwrite(fat, 6, 1, ptr);


    // Write root directory
    fseek(ptr, 0x18000, SEEK_SET);
    FileEntry_t entry {
        .filename={'E','M','B','E','D','D','E','D'},
        .extension={' ','F','S',},
        .attributes = (char)FileAttributes::VOLUME_LABEL,
        .reserved = {0},
        .modifyTime = {
            .seconds_periods = 0,
            .minutes = 59,
            .hours = 9,
        },
        .modifyDate = {
            .days = 22,
            .months = 10,
            .years = 2022-1980
        },
        .startingCluster = 0,
        .fileSizeBytes = 0
    };
    fwrite(&entry, sizeof(entry), 1, ptr);

    entry = {
        .filename={'t','e','s','t','2',' ',' ',' '},
        .extension={'t','x','t',},
        .attributes = 0,
        .reserved = {0},
        .creationTimeTenths = 0,
        .createTime = {
            .seconds_periods = 0,
            .minutes = 0,
            .hours = 0,
        },
        .createDate = {
            .days = 22,
            .months = 10,
            .years = 2022-1980
        },
        .accessDate = {
            .days = 22,
            .months = 10,
            .years = 2022-1980
        },
        .modifyTime = {
            .seconds_periods = 0,
            .minutes = 36,
            .hours = 14, // gmt, I think -- set to 14 to make 10am ET
        },
        .modifyDate = {
            .days = 22,
            .months = 10,
            .years = 2022-1980
        },
        .startingCluster = 2,
        .fileSizeBytes = 32
    };
    fwrite(&entry, sizeof(entry), 1, ptr);


    // Write the first file
    fseek(ptr, 0x20000, SEEK_SET);
    char file1[32] = "helloooooooasdf";
    fwrite(&file1, sizeof(file1), 1, ptr);
}
