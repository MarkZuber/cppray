// cppray.cpp : Defines the entry point for the application.
//

#include "cppray.h"
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>

void Render(const shared_ptr<RayTracer>& rayTracer,
            const RayTraceRenderData& renderData,
            const string& outputFilePath) {
  PixelArray pixelArray(renderData.Width(), renderData.Height());
  SceneRenderer sceneRenderer;

  sceneRenderer.RayTraceScene(rayTracer, pixelArray,
                              renderData.MaxParallelism());
}

int main() {
  cout << "Hello CMake." << endl;

  // // boost sample
  // using namespace boost::lambda;
  // typedef std::istream_iterator<int> in;
  // std::for_each(in(std::cin), in(), std::cout << (_1 * 3) << " ");

  Vector3<double> v(1.0, 2.0, 3.0);
  cout << v << endl;
  Vector3<double> v2 = v / 1.5;
  cout << v2 << endl;

  string inputContentRoot = "../content/";
  string outputContentRoot = "../outputContent/";
  // directory.CreateDirectory(outputContentRoot);

  const int processorCount = 8;

  RayTraceRenderData data(640, 640, 5, processorCount, inputContentRoot);
  auto rayTracer = RayTracerFactory::CreateSimpleTracer(data);

  // todo: path.combine to check for full path, ensure path separators, etc.
  string outputFilePath = outputContentRoot + "cppray.png";

  Render(rayTracer, data, outputFilePath);

  return 0;
}
