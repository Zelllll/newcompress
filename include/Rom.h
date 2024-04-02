#pragma once
#include <string>
#include <vector>

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
    EncodingType encoding;
    DmaEntry dmaEntry;
};

class Rom {
private:
    string inRomPath;
    string outRomPath;
    unsigned char *romBuf = nullptr;
    size_t romBufSize = 0;
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
     * Returns a constant pointer to the decompressed ROM buffer
     * @return
     */
    [[nodiscard]] unsigned const char* getDecompressedRomData() const;

    /**
     * Gets the current size of the `romBuf` array, which stores the decompressed size
     * @return
     */
    [[nodiscard]] size_t getDecompressedRomSize() const;

    /**
     * Swaps the endianness of entries within the DMA table
     * Note: Any time this function is used, it is expected to be called again
     * afterwards, in order to reverse the endianness to its previous state.
     */
    void swapDmaEndianess();

    /**
     * Compresses files and writes output ROM
     */
    void write();
};

