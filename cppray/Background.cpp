#include "cppray.h"

Background::Background(const COLOR_VECTOR& color, double ambience) : _color(color), _ambience(ambience) {}
COLOR_VECTOR Background::Color() const { return _color; }

IntersectionInfo::IntersectionInfo() {}
bool IntersectionInfo::IsHit() const { return _isHit; }
double IntersectionInfo::Distance() const { return _distance; }
void IntersectionInfo::SetDistance(double distance) { _distance = distance; }
void IntersectionInfo::SetHitCount(int hitCount) { _hitCount = hitCount; }
