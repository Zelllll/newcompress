#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iterator>

using namespace std;

class Rom {
private:
    enum EncodingType {
        ENC_NONE,
        ENC_LZO,
        ENC_APLIB,
        ENC_MAX,
    };

    struct RomFile {
        unsigned char* decompressedData;
//        vector<unsigned char>* compressedData;
        EncodingType encoding;
    };

    struct DmaEntry {
        unsigned __int32 vromStart;
        unsigned __int32 vromEnd;
        unsigned __int32 romStart;
        unsigned __int32 romEnd;
    };

    struct Range {
        int start;
        int end;
    };

    // file paths
    string inRomPath;
    string outRomPath;
    // decompressed ROM data
    unsigned char* romBuf;
    // decompressed ROM file size

    // info for each file in the game
    vector<RomFile*> romFiles;

    vector<Range*> interpretRange(const string& rangeStr) {
        vector<Range*> vec;

        int lastFoundCommaPosition = -1;
        for (int i = 0; i < rangeStr.size(); i++) {
            if (rangeStr.at(i) == ',' || (i == rangeStr.size() - 1 && lastFoundCommaPosition != -1)) {
                for (int j = lastFoundCommaPosition + 1; j < i; j++) {
                    if (rangeStr.at(j) == '-') {
                        vec.push_back(new Range{
                            stoi(rangeStr.substr(lastFoundCommaPosition + 1, j - lastFoundCommaPosition + 1)),
                            stoi(rangeStr.substr(j + 1, i))});
                        break;
                    }
                }
                lastFoundCommaPosition = i;
            }
        }

        // range does not have commas
        if (lastFoundCommaPosition == -1) {
            vec.push_back(new Range{stoi(rangeStr), stoi(rangeStr)});
        }

        return vec;
    }

    void splitFiles(unsigned int dmaTableOffset) {
        cout << "Parsing DMA table..." << endl;

        for (auto *dmaEntry = reinterpret_cast<DmaEntry *>(&romBuf[dmaTableOffset]);
             dmaEntry->vromEnd != 0; dmaEntry++) {
            if (dmaEntry->romEnd) {
                cerr << "Rom.splitFiles() : ROM appears to be already compressed" << endl;
            }

            auto *curFile = new RomFile;

            curFile->decompressedData = &romBuf[dmaEntry->vromStart];
            curFile->encoding = ENC_NONE;
//            curFile->compressedData = nullptr;

            romFiles.push_back(curFile);
        }
    }

    void loadDecompressedRom()
    {
        // load decompressed ROM file
        std::ifstream file(inRomPath, std::ios::binary);
        file.unsetf(std::ios::skipws);

        // obtain decompressed ROM size
        std::streampos fileSize;
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // read the data into the ROM buffer
        auto* arr = new unsigned char[fileSize];
        file.read((char*)arr, fileSize);
        romBuf = arr;
    }

public:
    Rom(const string& inPath, const string& outPath, unsigned int dmaTableOffset) {
        inRomPath = inPath;
        outRomPath = outPath;
        loadDecompressedRom();
        splitFiles(dmaTableOffset);
    }

    void markFilesForCompression(const string& args) {
        vector<Range*> ranges = interpretRange(args);

        for (auto r : ranges) {
            // swap start and end if necessary
            if (r->end < r->start) {
                swap(r->end, r->start);
            }

            // mark files for compression
            for (int i = r->start; i <= r->end; i++) {
                cout << "Marking file " << i << " for compression..." << endl;
                romFiles[i]->encoding = ENC_LZO;
            }
        }
    }

    void markSpecialFilesForCompression(const string& args) {
        vector<Range*> ranges = interpretRange(args);

        for (auto r : ranges) {
            // swap start and end if necessary
            if (r->end < r->start) {
                swap(r->end, r->start);
            }

            // mark files for special compression
            for (int i = r->start; i <= r->end; i++) {
                cout << "Marking file " << i << " for special compression..." << endl;
                romFiles[i]->encoding = ENC_APLIB;
            }
        }
    }

    void write() {

    }
};



int main() {
    Rom myRom {"sf_uncompressed.z64", "sf.z64", 0x16c7520};
    myRom.markFilesForCompression("14-185,189-376");
    myRom.markSpecialFilesForCompression("377");
    myRom.write();
    return 0;
}
