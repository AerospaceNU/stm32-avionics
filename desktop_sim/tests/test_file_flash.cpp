#include <gtest/gtest.h>
#include "file_backed_flash.h"
#include <cstring>

TEST(FileFlash, ReadWrite) {
    std::string ext_flash_path = "flash_test_file.hex";
    FileBackedFlash *externalFlash = new FileBackedFlash(ext_flash_path, 0x4000000);

    uint8_t pdata[10] = {1,2,3,4,5,6,7,8,9,10};
    uint8_t pdata2[10] = {0};
    int LOCATION = 0;

    LOCATION = 0;
    externalFlash->WriteStart(LOCATION, 10, pdata);
    externalFlash->ReadStart(LOCATION, 10, pdata2);
    EXPECT_TRUE( 0 == std::memcmp( pdata, pdata2, sizeof( pdata ) ) );

    LOCATION = 100;
    externalFlash->WriteStart(LOCATION, 10, pdata);
    externalFlash->ReadStart(LOCATION, 10, pdata2);
    EXPECT_TRUE( 0 == std::memcmp( pdata, pdata2, sizeof( pdata ) ) );
    LOCATION = 40;
    externalFlash->WriteStart(LOCATION, 10, pdata);
    externalFlash->ReadStart(LOCATION, 10, pdata2);
    EXPECT_TRUE( 0 == std::memcmp( pdata, pdata2, sizeof( pdata ) ) );

    delete externalFlash;
    auto externalFlash2 = new FileBackedFlash(ext_flash_path, 0x4000000);
    LOCATION = 40;
    EXPECT_TRUE(externalFlash2->ReadStart(LOCATION, 10, pdata2));
    EXPECT_TRUE( 0 == std::memcmp( pdata, pdata2, sizeof( pdata ) ) );
    delete externalFlash2;
}

TEST(FileFlash, WriteMiddle) {
    std::string ext_flash_path = "flash_test_file2.hex";
    FileBackedFlash *externalFlash = new FileBackedFlash(ext_flash_path, 0x100);
    externalFlash->Reinit(true);

    uint8_t correct[0x100];
    memset(correct,0xff,0x100);

    uint8_t pdata[10] = {1,2,3,4,5,6,7,8,9,10};
    memset(pdata,1,10);

    int LOCATION = 10;
    externalFlash->WriteStart(LOCATION, 10, pdata);
    memcpy(correct + LOCATION, pdata, 10);
    memset(pdata,2,10);

    LOCATION = 15;
    externalFlash->WriteStart(LOCATION, 10, pdata);
    memcpy(correct + LOCATION, pdata, 10);
    memset(pdata,3,10);

    LOCATION = 20;
    externalFlash->WriteStart(LOCATION, 10, pdata);
    memcpy(correct + LOCATION, pdata, 10);

    // read back
    uint8_t readData[0x100];
    externalFlash->ReadStart(0, 0x100, readData);

    for (int i = 0; i < sizeof(correct); i++) {
        EXPECT_EQ(correct[i], readData[i]);
    }


    delete externalFlash;
}