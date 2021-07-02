#include "Utils.h"
#include <algorithm>

namespace Utils
{
    std::string toLowerCase(std::string str)
    {
        std::transform(std::begin(str), std::end(str), std::begin(str),
                       [](unsigned char c) { return std::tolower(c); });
        return str;
    }

    bool hasWhitespaces(const std::string& str)
    {
        return str.find(' ') != std::string::npos;
    }
}
