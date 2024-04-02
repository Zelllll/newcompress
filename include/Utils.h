#pragma once
#include <vector>
#include <string>

namespace Utils {
    /**
     * Interprets range expressed as a string
     * @param rangeStr - range expressed in the format of "start1-end1,start2-end2,..."
     * @return - vector of ranges
     */
    void interpretRange(const std::string &rangeStr, std::vector<int> &start, std::vector<int> &end);

    /**
     * Generic function to align the value `n` to `alignTo`
     * Note: `alignTo` should only be 8, 16, 32, 64, or 256.
     * @param n - input value
     * @param alignTo - aligner
     * @return - aligned value
     */
    template<typename T>
    T alignValue(T n, int alignTo) {
        switch (alignTo) {
            case 8:
            case 16:
            case 32:
            case 64:
            case 256:
                return (n + (alignTo - 1)) & ~(alignTo - 1);
            default:
                std::cerr << "Utils::alignValue() : alignTo is not a power of 2" << std::endl;
                return n;
        }
    }

    /**
     * Writes an unsigned char array to a file
     * @param data - pointer to the data buffer to write
     * @param size - size of the file
     * @param filePath - output file path
     * @return - returns true if successful
     */
    void writeArrayToFile(const unsigned char* data, size_t size, const std::string& filePath);

    /**
     * Reads a file into an unsigned char array
     * @param filePath - output file path
     * @return - returns true if successful
     */
    unsigned char* readFileIntoArray(const std::string& filePath, size_t& bufSize);
}
