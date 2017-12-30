#include "cppray.h"

Background::Background(const COLOR_VECTOR &color, double ambience) : _color(color), _ambience(ambience) {}
COLOR_VECTOR Background::Color() const { return _color; }
double Background::Ambience() const { return _ambience; }

IntersectionInfo::IntersectionInfo() : _isHit(false), _distance(0.0), _hitCount(0) {}
bool IntersectionInfo::IsHit() const { return _isHit; }
void IntersectionInfo::SetIsHit(bool isHit) { _isHit = isHit; }
double IntersectionInfo::Distance() const { return _distance; }
void IntersectionInfo::SetDistance(double distance) { _distance = distance; }
int IntersectionInfo::HitCount() const { return _hitCount; }
void IntersectionInfo::SetHitCount(int hitCount) { _hitCount = hitCount; }

POS_VECTOR IntersectionInfo::Normal() const { return _normal; }
void IntersectionInfo::SetNormal(const POS_VECTOR &normal) { _normal = normal; }

POS_VECTOR IntersectionInfo::Position() const { return _position; }
void IntersectionInfo::SetPosition(const POS_VECTOR &position) { _position = position; }
shared_ptr<Shape> IntersectionInfo::Element() { return _element; }
void IntersectionInfo::SetElement(const shared_ptr<Shape> &shape) { _element = shape; }
COLOR_VECTOR IntersectionInfo::Color() const { return _color; }
void IntersectionInfo::SetColor(const COLOR_VECTOR &color) { _color = color; }

Light::Light(const POS_VECTOR &position, const COLOR_VECTOR &color) : _position(position), _color(color) {}
COLOR_VECTOR Light::Color() const
{
    return _color;
}
POS_VECTOR Light::Position() const
{
    return _position;
}

double Material::Reflection() const
{
    return _reflection;
}
double Material::Refraction() const
{
    return _refraction;
}
double Material::Transparency() const
{
    return _transparency;
}
double Material::Gloss() const
{
    return _gloss;
}
Material::Material(double reflection, double transparency, double gloss, double refraction) : _reflection(reflection), _transparency(transparency), _gloss(gloss), _refraction(refraction)
{
}

/// <summary>
/// wraps any value up in the inteval [-1,1] in a rotational manner
/// e.g. 1.7 -> -0.3
/// e.g. -1.1 -> 0.9
/// e.g. -2.3 -> -0.3
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
double Material::WrapUp(double t) const
{
    double q = fmod(t, 2.0);
    if (q < -1)
    {
        q = q + 2.0;
    }
    if (q >= 1)
    {
        q -= 2.0;
    }
    return q;
}

SolidMaterial::SolidMaterial(const COLOR_VECTOR &color, double reflection, double transparency, double gloss, double refraction)
    : Material(reflection, transparency, gloss, refraction), _color(color) {}

COLOR_VECTOR SolidMaterial::GetColor(double u, double v) const
{
    return _color;
}

bool SolidMaterial::HasTexture() const
{
    return false;
}

ChessboardMaterial::ChessboardMaterial(
    const COLOR_VECTOR &coloreven,
    const COLOR_VECTOR &colorodd,
    double reflection,
    double transparency,
    double gloss,
    double refraction,
    double density) : Material(reflection, transparency, gloss, refraction), _colorEven(coloreven), _colorOdd(colorodd), _density(density) {}

bool ChessboardMaterial::HasTexture() const { return true; }

COLOR_VECTOR ChessboardMaterial::GetColor(double u, double v) const
{
    double t = WrapUp(u) * WrapUp(v);

    if (t < 0.0)
    {
        return _colorEven;
    }
    else
    {
        return _colorOdd;
    }
}