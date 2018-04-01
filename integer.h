#ifndef INTEGER_H
#define INTEGER_H


class Integer
{
public:
    Integer(int value);
    operator int();
private:
    int _value;
};

#endif // INTEGER_H
