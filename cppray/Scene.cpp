#include "cppray.h"

Scene::Scene(const shared_ptr<Camera>& camera,
             const shared_ptr<Background>& background)
    : _camera(camera), _background(background) {
  _samplingQuality = 0;
  _renderDiffuse = true;
  _renderHighlights = true;
  _renderShadow = true;
  _renderReflection = true;
  _renderRefraction = true;
}

shared_ptr<Background> Scene::GetBackground() const { return _background; }
shared_ptr<Camera> Scene::GetCamera() const { return _camera; }
bool Scene::RenderDiffuse() const { return _renderDiffuse; }
bool Scene::RenderHighlights() const { return _renderHighlights; }
bool Scene::RenderReflection() const { return _renderReflection; }
bool Scene::RenderRefraction() const { return _renderRefraction; }
bool Scene::RenderShadow() const { return _renderShadow; }
int Scene::SamplingQuality() const { return _samplingQuality; }

vector<shared_ptr<Shape>> Scene::Shapes() { return _shapes; }

vector<shared_ptr<Light>> Scene::Lights() { return _lights; }