#include "cppray.h"

SimpleRayTracer::SimpleRayTracer(const RayTraceRenderData& renderData)
    : _renderData(renderData) {}
ColorVector SimpleRayTracer::GetPixelColor(int x, int y) {
  return ColorVector(1.0, 2.0, 3.0);
}

/* static */ shared_ptr<RayTracer> RayTracerFactory::CreateSimpleTracer(
    const RayTraceRenderData& renderData) {
  return make_shared<SimpleRayTracer>(renderData);
}