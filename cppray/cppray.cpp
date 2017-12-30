// cppray.cpp : Defines the entry point for the application.
//

#include "cppray.h"
#include <boost/lambda/lambda.hpp>

void Render(const shared_ptr<RayTracer> &rayTracer,
            const shared_ptr<RayTraceRenderData> &renderData,
            const string &outputFilePath)
{
  auto pixelArray = make_shared<PixelArray>(renderData->Width(), renderData->Height());
  SceneRenderer sceneRenderer;

  sceneRenderer.RayTraceScene(rayTracer, pixelArray,
                              renderData->MaxParallelism());

  pixelArray->SaveAsPng(outputFilePath);
}

int main()
{
  string inputContentRoot = "../content/";
  string outputContentRoot = "/Users/mzuber/repos/cppray/output/";
  // directory.CreateDirectory(outputContentRoot);

  const int processorCount = 8;

  auto renderData = make_shared<RayTraceRenderData>(900, 900, 5, processorCount,
                                                    inputContentRoot);
  auto scene = SceneFactory::CreateMarblesScene();
  auto rayTracer = RayTracerFactory::CreateSimpleTracer(renderData, scene);

  // todo: path.combine to check for full path, ensure path separators, etc.
  string outputFilePath = outputContentRoot + "cppray.png";

  cout << "starting rendering..." << endl;

  auto start = boost::posix_time::microsec_clock::local_time();
  Render(rayTracer, renderData, outputFilePath);
  auto finish = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration diff = finish - start;
  double totalMs = diff.total_milliseconds();

  cout << "total render time: " << totalMs << "ms" << endl;
  return 0;
}
