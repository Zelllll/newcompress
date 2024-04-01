#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "../include/rom.h"

using namespace std;

/**
 * Reads DMA table from decompressed ROM, and populates the `romFiles` vector
 * @param dmaTableOffset
 */
void Rom::prepareFiles(unsigned int dmaTableOffset) {
    for (auto *dmaEntry = reinterpret_cast<DmaEntry *>(&romBuf[dmaTableOffset]);
         dmaEntry->vromEnd != 0; dmaEntry++) {
        if (dmaEntry->romEnd) {
            cerr << "Rom.splitFiles() : ROM appears to be already compressed" << endl;
        }

        auto *curFile = new RomFile;

        // byte swap since N64 is big endian
        curFile->dmaEntry.vromStart = _byteswap_ulong(dmaEntry->vromStart);
        curFile->dmaEntry.vromEnd = _byteswap_ulong(dmaEntry->vromEnd);
        curFile->dmaEntry.romStart = _byteswap_ulong(dmaEntry->romStart);
        curFile->dmaEntry.romEnd = _byteswap_ulong(dmaEntry->romEnd);

        // populate file parameters
        curFile->decompressedData = &romBuf[curFile->dmaEntry.vromStart];
        curFile->encoding = ENC_NONE;

        // write file pointer to vector
        romFiles.push_back(curFile);
    }
}

/**
 * Loads decompressed ROM data into `romBuf`.
 * Before running this, `romBuf` will be equal to nullptr
 */
void Rom::loadDecompressedRom() {
    // load decompressed ROM file
    std::ifstream file(inRomPath, std::ios::binary);
    file.unsetf(std::ios::skipws);

    // obtain decompressed ROM size
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data into the ROM buffer
    auto *arr = new unsigned char[fileSize];
    file.read(reinterpret_cast<char*>(arr), fileSize);
    romBuf = arr;
}

/**
 * ROM Constructor
 * @param inPath - input decompressed ROM path
 * @param outPath - output compressed ROM path
 * @param dmaTableOffset - offset of the file `dmaData` within the ROM
 */
Rom::Rom(const string &inPath, const string &outPath, unsigned int dmaTableOffset) {
    inRomPath = inPath;
    outRomPath = outPath;
    loadDecompressedRom();
    prepareFiles(dmaTableOffset);
}

/**
 * Marks files for compression with the specified encoding
 * @param args - range expressed in the format of "start1-end1,start2-end2,..."
 */
void Rom::markFilesForCompression(const string &args, EncodingType encoder) {
    if (romFiles.empty()) {
        cerr << "Rom.markFilesForCompression : Failed to mark files for compression, "
                "as no files exist in the vector" << endl;
    }

    vector<Range *> ranges = interpretRange(args);

    for (auto r: ranges) {
        // swap start and end if necessary
        if (r->end < r->start) {
            swap(r->end, r->start);
        }

        // mark files for compression
        for (int i = r->start; i <= r->end; i++) {
            cout << "Marking file " << i << " for compression..." << endl;
            romFiles[i]->encoding = encoder;
        }
    }
}

/**
 * Compresses files and writes output ROM
 */
void Rom::write() {

}
