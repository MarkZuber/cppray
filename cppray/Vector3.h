#pragma once

#include <iostream>
#include <sstream>
using namespace std;

template <class T>
class Vector3 {
 private:
  T _x;
  T _y;
  T _z;

 public:
  Vector3() {}

  Vector3(T x, T y, T z) {
    _x = x;
    _y = y;
    _z = z;
  }

  const T& X() const { return _x; }

  const T& Y() const { return _y; }

  const T& Z() const { return _z; }

  Vector3<T> operator/(T num) {
    return Vector3<T>(X() / num, Y() / num, Z() / num);
  }

  Vector3<T> operator/(Vector3<T> denom) {
    return Vector3<T>(X() / denom.X(), Y() / denom.Y(), Z() / denom.Z());
  }

  Vector3<T> DivideBy(T m) { return Vector3(_x / m, _y / m, _z / m); }

  friend ostream& operator<<<>(ostream& os, const Vector3& v);
};

template <class T>
ostream& operator<<(ostream& os, const Vector3<T>& v) {
  os << "Vector3(" << v.X() << ", " << v.Y() << ", " << v.Z() << ")";
  return os;
}

  // template <typename T>
  // typename std::enable_if<std::is_base_of<Vector3<T>, T>::value,
  // std::ostream&>::type  operator<<(std::ostream& out, const T& x)
  //{
  //    out << x.X() << ", " << x.Y() << ", " << x.Z();
  //}

#define ColorVector Vector3<double>
