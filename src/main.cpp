#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

#include "../include/Rom.h"

using namespace std;

#define TEST_IN "sf_uncompressed.z64"
#define TEST_OUT "sf.z64"
#define TEST_COMPRESS_RANGE "14-185,189-376"
#define TEST_COMPRESS_SPECIAL_RANGE "377"
#define TEST_DMA_TABLE 0x16c7520

/**
 * Main driver
 * @return
 */
int main() {
    Rom myRom{TEST_IN, TEST_OUT, TEST_DMA_TABLE};
    myRom.markFilesForCompression(TEST_COMPRESS_RANGE, ENC_LZO);
    myRom.markFilesForCompression(TEST_COMPRESS_SPECIAL_RANGE, ENC_APLIB);
    myRom.write();
    return 0;
}
