#ifndef ADDITIONALFUNCTIONS_H
#define ADDITIONALFUNCTIONS_H
#include "item.h"
#include <vector>
#include <QJsonObject>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <locale>

std::vector<std::string> split(const std::string &str, const char &delimiter);
void lTrim(std::string &s);
void rTrim(std::string &s);
void trim(std::string &s);
DataProp* buildDataProp(QJsonObject &obj);

#endif // ADDITIONALFUNCTIONS_H
