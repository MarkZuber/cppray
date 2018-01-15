// cppray.cpp : Defines the entry point for the application.
//

#include "cppray.h"
#include <boost/lambda/lambda.hpp>

void RenderFrame(const shared_ptr<RayTraceRenderData> &renderData,
                 const shared_ptr<Scene> &scene, const string &outputFilePath)
{
  {
    LogTimer renderResult("total render time: ");
    SceneRenderer sceneRenderer;
    auto pixelArray = sceneRenderer.RenderScene(renderData, scene);
    pixelArray->SaveAsPng(outputFilePath);
  }
}

void RenderAnimation(const shared_ptr<RayTraceRenderData> &renderData,
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
You can simply rotate your setup: let ϕ∈[0,2π] parametrize your circle like you
did, but use θ∈[0,π2] to describe the rotation of your plane. Then you can use

x=rcosϕcosθ
y=rsinϕ
z=rcosϕsinθ

For θ=0 this gives a circle in the xy plane, and for θ=π/2 the circle lies in
the zy plane.    */

    double currentAngleDegrees = tickAngleDegrees * (i - 1);
    double phi =
        currentAngleDegrees * boost::math::constants::pi<double>() / 180;
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

  const int width = 400;
  const int height = 400;
  const int rayTraceDepth = 5;

  POS_VECTOR cameraPos = {60.0, 7.5, 150.0};
  POS_VECTOR cameraLookAt = {-0.1, 0.1, 0.0};
  POS_VECTOR cameraUp = {0.0, 0.0, 1.0};
  double cameraFov = 50.0;
  COLOR_VECTOR backgroundColor = {0.0, 0.0, 0.0};
  double backgroundAmbience = 0.2;

  double sphereRadius = 2.0;
  double sphereDistanceIncrement = 4.0;
  int numSpheresPerAxis = 10;

  bool showPlane = true;
  POS_VECTOR planePos = {1.0, 0.0, 0.0};
  double planeDVal = 0.0;

  auto renderData = make_shared<RayTraceRenderData>(
      width, height, rayTraceDepth, processorCount, inputContentRoot);
  // auto scene = SceneFactory::CreateMarblesScene();

  auto scene = SceneFactory::CreateMarblesSceneWithParameters(
      cameraPos, cameraLookAt, cameraUp, cameraFov, backgroundColor,
      backgroundAmbience, sphereRadius, sphereDistanceIncrement,
      numSpheresPerAxis, showPlane, planePos, planeDVal);

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
