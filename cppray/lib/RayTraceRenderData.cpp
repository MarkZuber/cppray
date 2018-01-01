#include "cppray.h"

RayTraceRenderData::RayTraceRenderData(const int width, const int height,
                                       const int traceDepth,
                                       const int maxParallelism,
                                       const string& outputPath) {
  _width = width;
  _height = height;
  _traceDepth = traceDepth;
  _maxParallelism = maxParallelism;
  _outputPath = outputPath;
}

int RayTraceRenderData::Width() const { return _width; }
int RayTraceRenderData::Height() const { return _height; }
int RayTraceRenderData::TraceDepth() const { return _traceDepth; }
int RayTraceRenderData::MaxParallelism() const { return _maxParallelism; }
string RayTraceRenderData::OutputPath() const { return _outputPath; }
