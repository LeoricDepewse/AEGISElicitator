#include "additionalfunctions.h"

std::vector<std::string> split(const std::string &str, const char &delimiter)
{
    std::vector<std::string> strings;
    std::istringstream f(str);
    std::string s;
    while(std::getline(f, s, delimiter))
        strings.push_back(s);
    return strings;
}
