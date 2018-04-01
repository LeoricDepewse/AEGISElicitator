#include "integer.h"

Integer::Integer(int value)
{
    _value = value;
}

Integer::operator int()
{
    return _value;
}
