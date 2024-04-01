#include "../include/utils.h"

#include <iostream>

/**
 * Interprets range expressed as a string
 * @param rangeStr - range expressed in the format of "start1-end1,start2-end2,..."
 * @return - vector of ranges
 */
void interpretRangeOld(const std::string &rangeStr, std::vector<int>& start, std::vector<int>& end) {
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


void interpretRange(const std::string &rangeStr, std::vector<int>& start, std::vector<int>& end) {
    std::vector<std::string> commaSplitStrings;
    std::vector<int> commaPositions;

    // find the position of every comma in the string
    for (int i = 0; i < rangeStr.size(); i++) {
        if (rangeStr[i] == ',')
            commaPositions.push_back(i);
    }

    // if there are no commas, simply use the entire string instead
    if (commaPositions.empty()) {
        commaSplitStrings.push_back(rangeStr);
    } else {
        int prevCommaPos = -1;
        for (auto index : commaPositions) {
            commaSplitStrings.push_back(rangeStr.substr(prevCommaPos + 1, index));
            prevCommaPos = index;
        }
        if (prevCommaPos != -1)
            commaSplitStrings.push_back(rangeStr.substr(prevCommaPos + 1, rangeStr.size() - prevCommaPos));
    }

    // determine ranges
    for (auto s : commaSplitStrings) {
        int dashPos = -1;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '-') {
                dashPos = i;
                break;
            }
        }

        if (dashPos == -1) {
            start.push_back(stoi(s));
            end.push_back(stoi(s));
        } else {
            start.push_back(stoi(s.substr(0, dashPos)));
            end.push_back(stoi(s.substr(dashPos + 1, s.size() - dashPos)));
        }
    }
}