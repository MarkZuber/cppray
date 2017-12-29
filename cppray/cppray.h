// cppray.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <vector>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)  // Perhaps also 4100, 4127, 4512
#endif
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/gil_all.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace std;

#define COLOR_VECTOR boost::qvm::vec<double, 3>
#define POS_VECTOR boost::qvm::vec<double, 3>

class PixelArray {
 private:
  int _width;
  int _height;
  boost::gil::rgb8_image_t _image;

 public:
  PixelArray(int width, int height);
  int Width() const;
  int Height() const;
  void SetPixelColor(int x, int y, const COLOR_VECTOR& color);
  void SaveAsPng(const string& outputFilePath);
};

class RayTraceRenderData {
 private:
  int _width;
  int _height;
  int _traceDepth;
  int _maxParallelism;
  string _outputPath;

 public:
  RayTraceRenderData(const int width, const int height, const int traceDepth,
                     const int maxParallelism, const string& outputPath);

  int Width() const;
  int Height() const;
  int TraceDepth() const;
  int MaxParallelism() const;
  string OutputPath() const;
};

class Ray {
 private:
  POS_VECTOR _position;
  POS_VECTOR _direction;

 public:
  Ray(const POS_VECTOR& position, const POS_VECTOR& direction);

  const POS_VECTOR& Position() const;
  const POS_VECTOR& Direction() const;
};

class Background {
 private:
  COLOR_VECTOR _color;
  double _ambience;

 public:
  Background(const COLOR_VECTOR& color, double ambience);
  COLOR_VECTOR Color() const;
  double Ambience() const;
};

class Camera {
 private:
  POS_VECTOR _equator;
  POS_VECTOR _lookAt;
  POS_VECTOR _position;
  POS_VECTOR _screen;
  POS_VECTOR _up;

 public:
  Camera(const POS_VECTOR& position, const POS_VECTOR& lookAt);
  Camera(const POS_VECTOR& position, const POS_VECTOR& lookAt,
         const POS_VECTOR& up);

  Ray GetRay(double vx, double vy) const;
  POS_VECTOR GetPosition() const;
};

class IntersectionInfo;

class Material {
 public:
  double Gloss() const;
  double Reflection() const;
  double Refraction() const;
  double Transparency() const;
};

class Light {
 public:
  COLOR_VECTOR Color() const;
  POS_VECTOR Position() const;
};

class Shape {
 private:
  static int _nextId;
  int _id;

 protected:
  Shape();

 public:
  int Id() const;
  virtual IntersectionInfo Intersect(const Ray& ray) = 0;
  shared_ptr<Material> GetMaterial();
  POS_VECTOR Position() const;
};

class IntersectionInfo {
 private:
  COLOR_VECTOR _color;
  double _distance;
  shared_ptr<Shape> _element;
  bool _isHit;
  int _hitCount;
  POS_VECTOR _normal;
  POS_VECTOR _position;

 public:
  IntersectionInfo();

  bool IsHit() const;
  double Distance() const;

  POS_VECTOR Normal() const;
  POS_VECTOR Position() const;
  shared_ptr<Shape> Element();
  COLOR_VECTOR Color() const;

  void SetDistance(double distance);
  void SetHitCount(int hitCount);
  void SetColor(const COLOR_VECTOR& color);
};

class Scene {
 private:
  shared_ptr<Background> _background;
  shared_ptr<Camera> _camera;
  vector<shared_ptr<Light>> _lights;
  vector<shared_ptr<Shape>> _shapes;
  bool _renderDiffuse;
  bool _renderHighlights;
  bool _renderReflection;
  bool _renderRefraction;
  bool _renderShadow;
  int _samplingQuality;

 public:
  Scene(const shared_ptr<Camera>& camera,
        const shared_ptr<Background>& background);

  shared_ptr<Background> GetBackground() const;
  void SetBackground(const shared_ptr<Background>& background);

  shared_ptr<Camera> GetCamera() const;
  void SetCamera(const shared_ptr<Camera>& camera);

  bool RenderDiffuse() const;
  bool RenderHighlights() const;
  bool RenderReflection() const;
  bool RenderRefraction() const;
  bool RenderShadow() const;
  int SamplingQuality() const;

  vector<shared_ptr<Shape>> Shapes();
  vector<shared_ptr<Light>> Lights();
};

class SceneFactory {
 public:
  static shared_ptr<Scene> CreateMarblesScene();
};

class RayTracer {
 public:
  virtual COLOR_VECTOR GetPixelColor(int x, int y) = 0;
};

class SimpleRayTracer : public RayTracer {
 private:
  shared_ptr<RayTraceRenderData> _renderData;
  shared_ptr<Scene> _scene;

 public:
  SimpleRayTracer(const shared_ptr<RayTraceRenderData>& renderData,
                  const shared_ptr<Scene> scene);
  COLOR_VECTOR GetPixelColor(int x, int y) override;

 private:
  COLOR_VECTOR CalculateColor(const Ray& ray);
  IntersectionInfo TestIntersection(const Ray& ray,
                                    const shared_ptr<Shape>& exclude);
  COLOR_VECTOR RayTrace(IntersectionInfo& info, const Ray& ray, int depth);

  Ray GetReflectionRay(const POS_VECTOR& p, const POS_VECTOR& n,
                       const POS_VECTOR& v);
  Ray GetRefractionRay(const POS_VECTOR& p, const POS_VECTOR& n,
                       const POS_VECTOR& v, double refraction);
};

class RayTracerFactory {
 public:
  static shared_ptr<RayTracer> CreateSimpleTracer(
      const shared_ptr<RayTraceRenderData>& renderData,
      const shared_ptr<Scene>& scene);
};

class SceneRenderer {
 public:
  SceneRenderer();
  void RayTraceScene(const shared_ptr<RayTracer>& rayTracer,
                     PixelArray& pixelArray, int maxParallelism);
};
