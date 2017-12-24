#include "cppray.h"

PixelArray::PixelArray(int width, int height) : _image(width, height)
{
  _width = width;
  _height = height;
}

int PixelArray::Width() const { return _width; }
int PixelArray::Height() const { return _height; }
void PixelArray::SetPixelColor(int x, int y, const ColorVector &color) {}
void PixelArray::SaveAsPng(const string &outputFilePath)
{
  boost::gil::png_write_view(outputFilePath.c_str(), const_view(_image));
}