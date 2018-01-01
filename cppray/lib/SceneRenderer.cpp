#include "cppray.h"

SceneRenderer::SceneRenderer() {}

shared_ptr<PixelArray> SceneRenderer::RenderScene(
    const shared_ptr<RayTraceRenderData> &renderData,
    const shared_ptr<Scene> &scene)
{
  auto pixelArray = make_shared<PixelArray>(renderData->Width(), renderData->Height());
  auto rayTracer = make_shared<SimpleRayTracer>(renderData, scene);

  // set to 1 or less for single threaded behavior
  if (renderData->MaxParallelism() <= 1)
  {
    RenderSingleThreaded(rayTracer, pixelArray);
  }
  else
  {
    RenderMultithreaded(rayTracer, pixelArray, renderData->MaxParallelism());
  }

  return pixelArray;
}

void SceneRenderer::RenderSingleThreaded(
    const shared_ptr<RayTracer> &rayTracer,
    shared_ptr<PixelArray> &pixelArray)
{
  for (int y = 0; y < pixelArray->Height(); y++)
  {
    for (int x = 0; x < pixelArray->Width(); x++)
    {
      auto colorVector = rayTracer->GetPixelColor(x, y);
      pixelArray->SetPixelColor(x, y, colorVector);
    }
    // cout << "y = " << y << endl << flush;

    /*
      trying this to update the info to include aspect ratio and such, but it renders a green image...
int ht = pixelArray->Height();
    int htover2 = ht / 2;
    int wd = pixelArray->Width();
    int wdover2 = wd / 2;

    double vinc = 2.0 / ((double)ht - 1); // * aspectratio
    double uinc = 2.0 / ((double)wd - 1);

    for (int y = htover2; y > -htover2; y--)
    {
      double v = y * vinc;
      for (int x = -wdover2; x < wdover2; x++)
      {
        double u = x * uinc;

        auto colorVector = rayTracer->GetPixelColor(u, v);
        pixelArray->SetPixelColor(x + wdover2, -y + htover2, colorVector);
      }
    }      */
  }
  // cout << endl << flush;
}

void ThreadRender(const shared_ptr<RayTracer> &rayTracer,
                  const shared_ptr<PixelArray> &pixelArray,
                  int segment,
                  int segmentWidth,
                  int &columnsDrawn)
{
  int startX = segment * segmentWidth;
  int endX = startX + segmentWidth - 1;

  if (pixelArray->Width() - endX < segmentWidth)
  {
    endX = pixelArray->Width() - 1;
  }

  // Console.WriteLine($ "starting segment {segment}. StartX ({startX})  EndX ({endX})");
  for (int i = startX; i <= endX; i++)
  {
    for (int j = 0; j < pixelArray->Height(); j++)
    {
      auto colorVector = rayTracer->GetPixelColor(i, j);
      pixelArray->SetPixelColor(i, j, colorVector);
    }

    bool shouldFireProgress = false;

    columnsDrawn++;
    int percentComplete = (columnsDrawn * 100) / pixelArray->Width();
    // shouldFireProgress = _lastPercentageComplete != percentComplete;
    // _lastPercentageComplete = percentComplete;
  }

  // if (shouldFireProgress)
  // {
  //   Progress ?.Invoke(this, new RayTraceProgressEventArgs(_lastPercentageComplete));
  // }
}

void SceneRenderer::RenderMultithreaded(
    const shared_ptr<RayTracer> &rayTracer,
    shared_ptr<PixelArray> &pixelArray,
    int maxParallelism)
{
  int segmentWidth = pixelArray->Width() / maxParallelism;

  int columnsDrawn = 0;
  int lastPercentageComplete = 0;

  boost::thread_group threads;

  for (int t = 0; t < maxParallelism; t++)
  {
    boost::thread *pth = new boost::thread(ThreadRender, rayTracer, pixelArray, t, segmentWidth, columnsDrawn);
    threads.add_thread(pth);
  }

  threads.join_all();
}

