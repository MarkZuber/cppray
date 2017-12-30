#include "cppray.h"

SceneRenderer::SceneRenderer() {}
void SceneRenderer::RayTraceScene(const shared_ptr<RayTracer> &rayTracer,
                                  PixelArray &pixelArray, int maxParallelism)
{
  for (int y = 0; y < pixelArray.Height(); y++)
  {
    for (int x = 0; x < pixelArray.Width(); x++)
    {
      auto colorVector = rayTracer->GetPixelColor(x, y);
      pixelArray.SetPixelColor(x, y, colorVector);
    }
    // cout << "y = " << y << endl << flush;
  }
  // cout << endl << flush;
}