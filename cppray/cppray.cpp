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

void RenderFrame(const shared_ptr<RayTracer> &rayTracer,
                 const shared_ptr<RayTraceRenderData> &renderData,
                 const string &outputFilePath)
{
  auto start = boost::posix_time::microsec_clock::local_time();
  Render(rayTracer, renderData, outputFilePath);
  auto finish = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration diff = finish - start;
  double totalMs = diff.total_milliseconds();

  cout << "total render time: " << totalMs << "ms" << endl;
}

void RenderAnimation(
    const shared_ptr<Scene> &scene,
    const shared_ptr<RayTracer> &rayTracer,
    const shared_ptr<RayTraceRenderData> &renderData,
    const string &outputContentRoot)
{
  int numFrames = 60;
  double cameraRadius = 15.0;
  double theta = 0; // boost::math::constants::pi<double>() / 2.0; // xy plane
  double tickAngleDegrees = 360 / numFrames;

  for (int i = 1; i <= numFrames; i++)
  {
    // todo: path.combine to check for full path, ensure path separators, etc.
    stringstream outputFilePathStream;
    outputFilePathStream << outputContentRoot << "cppray" << i << ".png";
    string outputFilePath = outputFilePathStream.str();
    // string outputFilePath = outputContentRoot + "cppray.png";

    /*
You can simply rotate your setup: let ϕ∈[0,2π] parametrize your circle like you did, but use θ∈[0,π2] to describe the rotation of your plane. Then you can use

x=rcosϕcosθ
y=rsinϕ
z=rcosϕsinθ

For θ=0 this gives a circle in the xy plane, and for θ=π/2 the circle lies in the zy plane.    */

    double currentAngleDegrees = tickAngleDegrees * (i - 1);
    double phi = currentAngleDegrees * boost::math::constants::pi<double>() / 180;
    double x = cameraRadius * cos(phi) * cos(theta);
    double y = cameraRadius * sin(phi);
    double z = -cameraRadius; // cameraRadius * cos(phi) * sin(theta);

    x = (double)i;
    y = (double)i;

    POS_VECTOR cameraPos = {x, y, z};

    cout << "x: " << x << "  y: " << y << "  z: " << z << endl;

    scene->GetCamera()->SetPosition(cameraPos);

    cout << "starting rendering..." << i << endl;
    RenderFrame(rayTracer, renderData, outputFilePath);
  }
}

int main()
{
  string inputContentRoot = "../content/";
  string outputContentRoot = "/Users/mzuber/repos/cppray/output/";
  // directory.CreateDirectory(outputContentRoot);

  const int processorCount = 8;

  auto renderData = make_shared<RayTraceRenderData>(400, 400, 5, processorCount,
                                                    inputContentRoot);
  auto scene = SceneFactory::CreateMarblesScene();
  auto rayTracer = RayTracerFactory::CreateSimpleTracer(renderData, scene);

  bool animation = false;

  if (animation)
  {
    RenderAnimation(scene, rayTracer, renderData, outputContentRoot);
  }
  else
  {
    string outputFilePath = outputContentRoot + "cppray.png";
    RenderFrame(rayTracer, renderData, outputFilePath);
  }

  return 0;
}
