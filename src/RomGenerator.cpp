#include <iostream>
#include <cassert>
#include "RomGenerator.h"
#include "Utils.h"

/**
 * RomGenerator constructor
 */
RomGenerator::RomGenerator(Rom *romP) {
    size_t maxBufSize = romP->getDecompressedRomSize();
    rom = romP;
    encoderBuf = new unsigned char[maxBufSize];
    compressedRomBuf = new unsigned char[maxBufSize];
}

/**
 * RomGenerator destructor
 */
RomGenerator::~RomGenerator() {
    delete[] encoderBuf;
    delete[] compressedRomBuf;
}

/**
 * Injects a file into the compressed ROM, compressing if necessary
 * @param rf - pointer to RomFile to inject
 */
void RomGenerator::inject(RomFile *rf) {
    unsigned const char* decRomData =  rom->getDecompressedRomData();
    int decSz = static_cast<int>(rf->dmaEntry.vromEnd - rf->dmaEntry.vromStart);
    int backupWritePosition = writePosition;

    std::cout << std::hex << "Injecting file at 0x" << writePosition << std::dec << std::endl;

    // just write the decompressed file for now
    for (int i = 0; i < decSz; i++) {
        assert(rf->dmaEntry.vromStart + i < rom->getDecompressedRomSize());
        assert(writePosition < rom->getDecompressedRomSize());

        compressedRomBuf[writePosition] = decRomData[rf->dmaEntry.vromStart + i];
        writePosition++;
    }

    // align the new write position
    writePosition = Utils::alignValue(writePosition, 16);

    // set the compressed rom start to the original write position
    rf->dmaEntry.romStart = backupWritePosition;
    rf->dmaEntry.romEnd = 0; // writePosition;
}

/**
 * Saves compressed ROM to disk
 * @param outPath - path to write ROM to
 */
void RomGenerator::save(const string& outPath) {
    // swap the endianness of the DMA table to big endian for N64
    rom->swapDmaEndianness();

    bool success = Utils::writeUnsignedCharArrayToFile(compressedRomBuf, rom->getDecompressedRomSize(), outPath);

    // revert DMA table to little endian
    rom->swapDmaEndianness();

    if (!success) {
        std::cerr << "Failed to write output ROM" << std::endl;
    } else {
        std::cout << "Successfully wrote " << outPath << "!" << std::endl;
    }
}
