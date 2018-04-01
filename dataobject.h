#ifndef DATAOBJECT_H
#define DATAOBJECT_H
#include <vector>
#include <string>
#include <QJsonObject>
#include <QJsonArray>
#include "objectpayloads.h"
#include "integer.h"
#include "item.h"

class DataObject
{
public:
    DataObject(const int ID, const DataProp* elems, const int elemCount);
    QJsonObject buildJson();
    DataProp &operator[](std::size_t idx) const;
    int size() const;
    int ID() const;

private:
    QJsonArray _buildJsonArray(std::vector<DataProp> array);
    int _ID;
    int _propLength;
    DataProp* _properties;
};

#endif // DATAOBJECT_H
