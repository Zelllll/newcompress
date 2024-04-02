#pragma once
#include <vector>
#include <string>
#include <memory>

namespace Utils {
    /**
     * Interprets range expressed as a string
     * @param rangeStr - range expressed in the format of "start1-end1,start2-end2,..."
     * @return - vector of ranges
     */
    void interpretRange(const std::string &rangeStr, std::vector<int>& start, std::vector<int>& end);
}
