#include "dataobject.h"

DataObject::DataObject(const int ID, const DataProp* elems, const int elemCount)
{
    _ID = ID;
    _propLength = elemCount;
    _properties = new DataProp[_propLength];
//    std::copy(_properties, _properties+_propLength, elems);
    for(int i = 0; i < _propLength; ++i)
    {
        _properties[i] = elems[i];
        if(_properties[i].type == combo)
        {
            ComboChoice* choice = new ComboChoice;
            switch(*(Integer*)_properties[i].data)
            {
            case T_SIGNIFICANCE:
                choice->choices = &SIGNIFICANCE;
                break;
            case T_ASSETTYPE:
                choice->choices = &gAssetType;
                break;
            default:
                throw "Invalid choices configuration";
            }
            _properties[i].data = choice;
        }
    }
}

QJsonObject DataObject::buildJson()
{
    QJsonObject object;
    for(int i = 0; i < _propLength; ++i)
    {
        QJsonValue val;
        switch(_properties[i].type)
        {
        case text:
        case largeText:
            val = QJsonValue(QString::fromStdString(*(std::string*)_properties[i].data));
            break;
        case number:
            val = QJsonValue(*(Integer*)_properties[i].data);
            break;
        case DataType::object:
            val = QJsonValue(((DataObject*)_properties[i].data)->buildJson());
            break;
        case array:
            if(_properties[i].data == nullptr)
            {
                val = QJsonArray();
                break;
            }
            val = QJsonValue(_buildJsonArray(*(std::vector<DataProp>*)_properties[i].data));
            break;
        case combo:
            {
                const ComboChoice* choice = (ComboChoice*)_properties[i].data;
                std::string value = (*choice->choices)[choice->index];
                val = QJsonValue(QString::fromStdString(value));
            }
            break;
        case nType:
            break;
        }
        object.insert(QString::fromStdString(_properties[i].name), val);
    }
    return object;
}

DataProp &DataObject::operator[](std::size_t idx) const
{
    if((unsigned)idx < _propLength)
        return _properties[idx];
    else throw "OutOfBounds";
}

int DataObject::size() const
{
    return _propLength;
}

QJsonArray DataObject::_buildJsonArray(std::vector<DataProp> array)
{
    QJsonArray jsonArray;
    foreach(DataProp property, array)
    {
        QJsonValue val;
        switch(property.type)
        {
        case text:
        case largeText:
            val = QJsonValue(QString::fromStdString(*(std::string*)property.data));
            break;
        case number:
            val = QJsonValue(*(Integer*)property.data);
            break;
        case DataType::object:
            val = QJsonValue(((DataObject*)property.data)->buildJson());
            break;
        case DataType::array:
            if(property.data == nullptr)
            {
                val = QJsonArray();
                break;
            }
            val = QJsonValue(_buildJsonArray(*(std::vector<DataProp>*)property.data));
            break;
        case combo:
            {
                const ComboChoice* choice = (ComboChoice*)property.data;
                const std::string value = (*choice->choices)[choice->index];
                val = QJsonValue(QString::fromStdString(value));
            }
            break;
        case nType:
            break;
        }
        jsonArray.push_back(val);
    }
    return jsonArray;
}

int DataObject::ID() const
{
    return _ID;
}
