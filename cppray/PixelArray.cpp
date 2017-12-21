#include "cppray.h"

PixelArray::PixelArray(int width, int height) {
  _width = width;
  _height = height;
}

int PixelArray::Width() const { return _width; }
int PixelArray::Height() const { return _height; }
void PixelArray::SetPixelColor(int x, int y, const ColorVector& color) {}