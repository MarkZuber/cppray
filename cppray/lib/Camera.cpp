#include "cppray.h"
using namespace boost::qvm;

Camera::Camera(const POS_VECTOR &position, const POS_VECTOR &lookAt, const POS_VECTOR &up, double viewAngleDegrees)
    : _position(position), _lookAt(lookAt), _up(up), _viewAngleDegrees(viewAngleDegrees)
{
    PrecalcEyeVectors();
}

void Camera::PrecalcEyeVectors()
{
    _a3 = _lookAt - _position;
    _a1 = cross(_a3, _up);
    _a2 = cross(_a1, _a3);
    normalize(_a1);
    normalize(_a2);
    normalize(_a3);
    double viewAngleRadians = _viewAngleDegrees * 0.017453293;
    _dval = cos(viewAngleRadians / 2.0) / sin(viewAngleRadians / 2.0);
}

Ray Camera::GetRay(double vx, double vy) const
{
    // POS_VECTOR right = _a1;
    // POS_VECTOR up = _a2;
    POS_VECTOR center = _a3 * _dval;
    POS_VECTOR dir = center + (_a1 * vx) + (_a2 * vy);

    normalize(dir);
    Ray ray(_position, dir);
    return ray;
}

POS_VECTOR Camera::GetPosition() const { return _position; }

void Camera::SetPosition(const POS_VECTOR &position)
{
    _position = position;
    PrecalcEyeVectors();
}
