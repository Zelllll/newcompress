#pragma once

#include <string>
#include "Rom.h"

class RomGenerator {
private:
    unsigned char *compRomBuf;
    int writePosition = 0;
    Rom *rom;

public:
    /**
     * RomGenerator constructor
     */
    explicit RomGenerator(Rom *romP);

    /**
     * RomGenerator destructor
     */
    ~RomGenerator();

    /**
     * Injects a file into the compressed ROM, compressing if necessary
     * @param rf - pointer to RomFile to inject
     */
    void inject(RomFile *rf);

    /**
     * Saves compressed ROM to disk
     * @param outPath - path to write ROM to
     */
    void save(const string& outPath);
};
