#include "cppray.h"

int Shape::_nextId = 1;

Shape::Shape()
{
    _id = ++_nextId;
}

int Shape::Id() const
{
    return _id;
}

shared_ptr<Material> Shape::GetMaterial() { return _material; }
POS_VECTOR Shape::Position() const { return _position; }
