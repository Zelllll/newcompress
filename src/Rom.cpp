#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <memory>
#include "Rom.h"
#include "Utils.h"
#include "RomGenerator.h"

using namespace std;

/**
 * ROM constructor
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
 * Rom destructor
 */
Rom::~Rom() {
    delete[] romBuf;
    for (auto* p : romFiles) {
        delete p;
    }
}

/**
 * Reads DMA table from decompressed ROM, and populates the `romFiles` vector
 * @param dmaTableOffset
 */
void Rom::prepareFiles(unsigned int dmaTableOffset) {
    for (auto *dmaEntry = reinterpret_cast<DmaEntry *>(&romBuf[dmaTableOffset]);
         dmaEntry->vromEnd != 0; dmaEntry++) {
        if (dmaEntry->romEnd) {
            cerr << "Rom::prepareFiles() : ROM appears to be already compressed" << endl;
        }

        auto *curFile = new RomFile;

        // set each field
        curFile->dmaEntry.vromStart = dmaEntry->vromStart;
        curFile->dmaEntry.vromEnd =   dmaEntry->vromEnd;
        curFile->dmaEntry.romStart =  dmaEntry->romStart;
        curFile->dmaEntry.romEnd =    dmaEntry->romEnd;

        // populate file parameters
        curFile->decompressedData = &romBuf[curFile->dmaEntry.vromStart];
        curFile->encoding = ENC_NONE;

        // write file pointer to vector
        romFiles.push_back(curFile);
    }

    // byte swap to little endian
    swapDmaEndianess();
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
    romBufSize = fileSize;
    auto *arr = new unsigned char[fileSize];
    file.read(reinterpret_cast<char*>(arr), fileSize);
    romBuf = arr;
}

/**
 * Marks files for compression with the specified encoding
 * @param args - range expressed in the format of "start1-end1,start2-end2,..."
 */
void Rom::markFilesForCompression(const string &args, EncodingType encoder) {
    if (romFiles.empty()) {
        cerr << "Rom::markFilesForCompression() : Failed to mark files for compression, "
                "as no files exist in the vector" << endl;
    }

    vector<int> startVec, endVec;
    Utils::interpretRange(args, startVec, endVec);

    assert(startVec.size() == endVec.size());

    for (int i = 0; i < startVec.size(); i++) {
        // swap start and end if necessary
        if (startVec[i] > endVec[i]) {
            swap(startVec[i], endVec[i]);
        }

        // mark files for compression
        for (int j = startVec[i]; j <= endVec[i]; j++) {
            cout << "Marking file " << j << " for compression..." << endl;
            romFiles[j]->encoding = encoder;
        }
    }
}

/**
 * Gets the current size of the `romBuf` array, which stores the decompressed size
 * @return
 */
size_t Rom::getDecompressedRomSize() const {
    return romBufSize;
}

/**
 * Returns a constant pointer to the decompressed ROM buffer
 * @return
 */
unsigned const char* Rom::getDecompressedRomData() const {
    return romBuf;
}

/**
 * Swaps the endianness of entries within the DMA table
 * Note: Any time this function is used, it is expected to be called again
 * afterwards, in order to reverse the endianness to its previous state.
 */
void Rom::swapDmaEndianess() {
    for (auto* r : romFiles) {
        r->dmaEntry.vromStart = _byteswap_ulong(r->dmaEntry.vromStart);
        r->dmaEntry.vromEnd = _byteswap_ulong(r->dmaEntry.vromEnd);
        r->dmaEntry.romStart = _byteswap_ulong(r->dmaEntry.romStart);
        r->dmaEntry.romEnd = _byteswap_ulong(r->dmaEntry.romEnd);
    }
}

/**
 * Compresses files and writes output ROM
 */
void Rom::write() {
    unique_ptr<RomGenerator> romGenerator(new RomGenerator(this));

    for (auto* r : romFiles) {
        romGenerator->inject(r);
    }

    romGenerator->save(outRomPath);
}
