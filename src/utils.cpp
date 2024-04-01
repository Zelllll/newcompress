#include "../include/utils.h"

/**
 * Interprets range expressed as a string
 * @param rangeStr - range expressed in the format of "start1-end1,start2-end2,..."
 * @return - vector of ranges
 */
void interpretRange(const std::string &rangeStr, std::vector<int>& start, std::vector<int>& end) {
    int lastFoundCommaPosition = -1;
    for (int i = 0; i < rangeStr.size(); i++) {
        if (rangeStr.at(i) == ',' || (i == rangeStr.size() - 1 && lastFoundCommaPosition != -1)) {
            for (int j = lastFoundCommaPosition + 1; j < i; j++) {
                if (rangeStr.at(j) == '-') {
                    start.push_back(stoi(rangeStr.substr(lastFoundCommaPosition + 1, j - lastFoundCommaPosition + 1)));
                    end.push_back(stoi(rangeStr.substr(j + 1, i)));
                    break;
                }
            }
            lastFoundCommaPosition = i;
        }
    }

    // range does not have commas
    if (lastFoundCommaPosition == -1) {
        start.push_back(stoi(rangeStr));
        end.push_back(stoi(rangeStr));
    }
}
