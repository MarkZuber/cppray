#include "cppray.h"

Ray::Ray(const POS_VECTOR& position, const POS_VECTOR& direction) : _position(position), _direction(direction) {}

const POS_VECTOR& Ray::Position() const { return _position; }
const POS_VECTOR& Ray::Direction() const { return _direction; }
