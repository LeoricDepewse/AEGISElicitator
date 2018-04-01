#ifndef ITEM_H
#define ITEM_H
#include <string>

enum DataType
{
    text,
    largeText,
    number,
    object,
    array,
    combo,
    nType
};

struct Item
{
    std::string key;
    std::string value;
};

struct ComboChoice
{
    const std::vector<std::string>* choices;
    int index = 0;
};

typedef struct DataProp
{
    std::string displayName;
    std::string name;
    DataType type;
    bool isDummy = false;
    void* data = nullptr;
} DataProp;

#endif // ITEM_H
