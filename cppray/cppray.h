// cppray.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <memory>
#include "Vector3.h"

using namespace std;

class PixelArray {
 private:
  int _width;
  int _height;

 public:
  PixelArray(int width, int height);
  int Width() const;
  int Height() const;
  void SetPixelColor(int x, int y, const ColorVector& color);
};

class RayTraceRenderData {
 private:
  int _width;
  int _height;
  int _traceDepth;
  int _maxParallelism;
  string _outputPath;

 public:
  RayTraceRenderData(const int width, const int height, const int traceDepth,
                     const int maxParallelism, const string& outputPath);

  // copy constructor
  RayTraceRenderData(const RayTraceRenderData& other) {
    _width = other.Width();
    _height = other.Height();
    _traceDepth = other.TraceDepth();
    _maxParallelism = other.MaxParallelism();
    _outputPath = other.OutputPath();
  }

  int Width() const;
  int Height() const;
  int TraceDepth() const;
  int MaxParallelism() const;
  string OutputPath() const;
};

class RayTracer {
 public:
  virtual ColorVector GetPixelColor(int x, int y) = 0;
};

class SimpleRayTracer : public RayTracer {
 private:
  RayTraceRenderData _renderData;

 public:
  SimpleRayTracer(const RayTraceRenderData& renderData);
  ColorVector GetPixelColor(int x, int y) override;
};

class RayTracerFactory {
 public:
  static shared_ptr<RayTracer> CreateSimpleTracer(
      const RayTraceRenderData& renderData);
};

class SceneRenderer {
 public:
  SceneRenderer();
  void RayTraceScene(const shared_ptr<RayTracer>& rayTracer,
                     PixelArray& pixelArray, int maxParallelism);
};
