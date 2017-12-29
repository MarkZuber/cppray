#include "cppray.h"
using namespace boost::qvm;

Camera::Camera(const POS_VECTOR &position, const POS_VECTOR &lookAt) : Camera(position, lookAt, {0.0, 1.0, 0.0}) {}
Camera::Camera(const POS_VECTOR &position, const POS_VECTOR &lookAt, const POS_VECTOR &up) : _position(position), _lookAt(lookAt)
{
    _up = up;
    normalize(_up);
    POS_VECTOR tempLookAt = lookAt;
    normalize(tempLookAt);
    _equator = cross(tempLookAt, _up);
    _screen = _position + _lookAt;
}

Ray Camera::GetRay(double vx, double vy) const
{
    POS_VECTOR pos = _screen - _up * vy - _equator * vx;
    POS_VECTOR dir = pos - _position;
    normalize(dir);
    Ray ray(pos, dir);
    return ray;
}

POS_VECTOR Camera::GetPosition() const { return _position; }
