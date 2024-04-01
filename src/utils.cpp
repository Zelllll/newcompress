#include "../include/utils.h"

/**
 * Interprets range expressed as a string
 * @param rangeStr - range expressed in the format of "start1-end1,start2-end2,..."
 * @return - vector of ranges
 */
std::vector<Range *> interpretRange(const std::string &rangeStr) {
    std::vector<Range *> vec;

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
