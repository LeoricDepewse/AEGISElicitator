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

void lTrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void rTrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s)
{
    lTrim(s);
    rTrim(s);
}

DataProp *buildDataProp(QJsonObject &obj)
{

}
