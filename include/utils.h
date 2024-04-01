#pragma once
#include <vector>
#include <string>

struct Range {
    int start;
    int end;
};

/**
 * Interprets range expressed as a string
 * @param rangeStr - range expressed in the format of "start1-end1,start2-end2,..."
 * @return - vector of ranges
 */
std::vector<Range *> interpretRange(const std::string &rangeStr);
