#include "cppray.h"

typedef image_t::view_t view_t;
typedef view_t::iterator img_iterator_t;
typedef img_iterator_t::value_type pixel;

PixelArray::PixelArray(int width, int height) : _image(width, height)
{
  _width = width;
  _height = height;
  _image.recreate(width, height);
}

double clampcolor(double orig)
{
  if (orig < 0.0)
  {
    return 0.0;
  }
  if (orig > 1.0)
  {
    return 1.0;
  }
  return orig;
}

int PixelArray::Width() const { return _width; }
int PixelArray::Height() const { return _height; }
void PixelArray::SetPixelColor(int x, int y, const COLOR_VECTOR &color)
{
  boost::gil::rgb8_view_t imgview = view(_image);
  boost::gil::rgb8_view_t::x_iterator iter = imgview.row_begin(y);
  boost::gil::rgb8_pixel_t pixel(
      (unsigned char)(clampcolor(X(color)) * 255.0),
      (unsigned char)(clampcolor(Y(color)) * 255.0),
      (unsigned char)(clampcolor(Z(color)) * 255.0));
  iter[x] = pixel;
}
void PixelArray::SaveAsPng(const string &outputFilePath)
{
  {
    LogTimer saveTime("total png save time: ");
    boost::gil::png_write_view(outputFilePath.c_str(), const_view(_image));
  }
}