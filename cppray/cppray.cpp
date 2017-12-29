// cppray.cpp : Defines the entry point for the application.
//

#include "cppray.h"
#include <boost/lambda/lambda.hpp>

void Render(const shared_ptr<RayTracer>& rayTracer,
            const shared_ptr<RayTraceRenderData>& renderData,
            const string& outputFilePath) {
  PixelArray pixelArray(renderData->Width(), renderData->Height());
  SceneRenderer sceneRenderer;

  sceneRenderer.RayTraceScene(rayTracer, pixelArray,
                              renderData->MaxParallelism());
}

int main() {
  cout << "Hello CMake." << endl;

  // // boost sample
  // using namespace boost::lambda;
  // typedef std::istream_iterator<int> in;
  // std::for_each(in(std::cin), in(), std::cout << (_1 * 3) << " ");

  COLOR_VECTOR v = {1.0, 2.0, 3.0};
  COLOR_VECTOR v2 = v / 1.5;

  string inputContentRoot = "../content/";
  string outputContentRoot = "../outputContent/";
  // directory.CreateDirectory(outputContentRoot);

  const int processorCount = 8;

  auto renderData = make_shared<RayTraceRenderData>(640, 640, 5, processorCount,
                                                    inputContentRoot);
  auto scene = SceneFactory::CreateMarblesScene();
  auto rayTracer = RayTracerFactory::CreateSimpleTracer(renderData, scene);

  // todo: path.combine to check for full path, ensure path separators, etc.
  string outputFilePath = outputContentRoot + "cppray.png";

  Render(rayTracer, renderData, outputFilePath);

  return 0;
}
