// cppray.cpp : Defines the entry point for the application.
//

#include "cppray.h"
#include <boost/lambda/lambda.hpp>

void RenderFrame(const shared_ptr<RayTraceRenderData> &renderData,
                 const shared_ptr<Scene> &scene,
                 const string &outputFilePath)
{
  {
    LogTimer renderResult("total render time: ");
    SceneRenderer sceneRenderer;
    auto pixelArray = sceneRenderer.RenderScene(renderData, scene);
    pixelArray->SaveAsPng(outputFilePath);
  }
}

void RenderAnimation(
    const shared_ptr<RayTraceRenderData> &renderData,
    const shared_ptr<Scene> &scene,
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
    RenderFrame(renderData, scene, outputFilePath);
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
  bool animation = false;

  if (animation)
  {
    RenderAnimation(renderData, scene, outputContentRoot);
  }
  else
  {
    string outputFilePath = outputContentRoot + "cppray.png";
    RenderFrame(renderData, scene, outputFilePath);
  }

  return 0;
}
