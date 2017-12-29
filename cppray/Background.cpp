#include "cppray.h"

Background::Background(const COLOR_VECTOR &color, double ambience) : _color(color), _ambience(ambience) {}
COLOR_VECTOR Background::Color() const { return _color; }
double Background::Ambience() const { return _ambience; }

IntersectionInfo::IntersectionInfo() {}
bool IntersectionInfo::IsHit() const { return _isHit; }
double IntersectionInfo::Distance() const { return _distance; }
void IntersectionInfo::SetDistance(double distance) { _distance = distance; }
void IntersectionInfo::SetHitCount(int hitCount) { _hitCount = hitCount; }

POS_VECTOR IntersectionInfo::Normal() const { return _normal; }

POS_VECTOR IntersectionInfo::Position() const { return _position; }
shared_ptr<Shape> IntersectionInfo::Element() { return _element; }
COLOR_VECTOR IntersectionInfo::Color() const { return _color; }

void IntersectionInfo::SetColor(const COLOR_VECTOR &color) { _color = color; }

COLOR_VECTOR Light::Color() const { return _color; }
POS_VECTOR Light::Position() const { return _position; }

double Material::Reflection() const { return _reflection; }
double Material::Refraction() const { return _refraction; }
double Material::Transparency() const { return _transparency; }
double Material::Gloss() const { return _gloss; }