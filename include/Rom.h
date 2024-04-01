#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "../include/utils.h"

using namespace std;

enum EncodingType {
    ENC_NONE,
    ENC_LZO,
    ENC_APLIB,
    ENC_MAX,
};

struct DmaEntry {
    unsigned __int32 vromStart;
    unsigned __int32 vromEnd;
    unsigned __int32 romStart;
    unsigned __int32 romEnd;
};

struct RomFile {
    unsigned char *decompressedData;
    unsigned char *compressedData;
    EncodingType encoding;
    DmaEntry dmaEntry;
};

class Rom {
    string inRomPath;
    string outRomPath;
    unsigned char *romBuf = nullptr;
    vector<RomFile *> romFiles;

    /**
     * Reads DMA table from decompressed ROM, and populates the `romFiles` vector
     * @param dmaTableOffset
     */
    void prepareFiles(unsigned int dmaTableOffset);

    /**
     * Loads decompressed ROM data into `romBuf`.
     * Before running this, `romBuf` will be equal to nullptr
     */
    void loadDecompressedRom();

public:
    /**
     * ROM Constructor
     * @param inPath - input decompressed ROM path
     * @param outPath - output compressed ROM path
     * @param dmaTableOffset - offset of the file `dmaData` within the ROM
     */
    Rom(const string &inPath, const string &outPath, unsigned int dmaTableOffset);

    /**
     * Rom destructor
     */
    ~Rom();

    /**
     * Marks files for compression with the specified encoding
     * @param args - range expressed in the format of "start1-end1,start2-end2,..."
     */
    void markFilesForCompression(const string &args, EncodingType encoder);

    /**
     * Compresses files and writes output ROM
     */
    void write();
};
