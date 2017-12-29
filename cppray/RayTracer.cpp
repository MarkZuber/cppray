#include "cppray.h"
using namespace boost::qvm;

SimpleRayTracer::SimpleRayTracer(
    const shared_ptr<RayTraceRenderData>& renderData,
    const shared_ptr<Scene> scene)
    : _renderData(renderData), _scene(scene) {}

COLOR_VECTOR SimpleRayTracer::GetPixelColor(int x, int y) {
  double xd = (double)x;
  double yd = (double)y;

  if (_scene->SamplingQuality() == 0) {
    double xp = xd / (double)_renderData->Width() * 2.0 - 1.0;
    double yp = yd / (double)_renderData->Height() * 2.0 - 1.0;

    Ray ray = _scene->GetCamera()->GetRay(xp, yp);
    return CalculateColor(ray);
  } else {
    // insert sampling quality math here
    return {1.0, 2.0, 3.0};
  }
}

COLOR_VECTOR SimpleRayTracer::CalculateColor(const Ray& ray) {
  IntersectionInfo info = TestIntersection(ray, nullptr);
  if (info.IsHit()) {
    return RayTrace(info, ray, 0);
  } else {
    return _scene->GetBackground()->Color();
  }
}

IntersectionInfo SimpleRayTracer::TestIntersection(
    const Ray& ray, const shared_ptr<Shape>& exclude) {
  int hitCount = 0;
  IntersectionInfo best;
  best.SetDistance(std::numeric_limits<double>::max());

  std::for_each(_scene->Shapes().begin(), _scene->Shapes().end(),
                [&](shared_ptr<Shape>& shape) {
                  if (shape->Id() != exclude->Id()) {
                    IntersectionInfo info = shape->Intersect(ray);
                    if (info.IsHit() && info.Distance() < best.Distance() &&
                        info.Distance() >= 0.0) {
                      best = info;
                      hitCount++;
                    }
                  }
                });

  best.SetHitCount(hitCount);
  return best;
}

void BlendColor(COLOR_VECTOR& refColor, const COLOR_VECTOR& otherColor,
                double weight) {
  refColor *= (1.0 - weight);
  refColor += otherColor * (1.0 - weight);
}

COLOR_VECTOR SimpleRayTracer::RayTrace(IntersectionInfo& info, const Ray& ray,
                                       int depth) {
  // Calculate ambient light
  COLOR_VECTOR color = info.Color() * _scene->GetBackground()->Ambience();
  double shininess = pow(10, info.Element()->GetMaterial()->Gloss() + 1);

  std::for_each(
      _scene->Lights().begin(), _scene->Lights().end(),
      [&](shared_ptr<Light>& light) {
        // calculate diffuse lighting
        POS_VECTOR v = light->Position() - info.Position();
        normalize(v);

        if (_scene->RenderDiffuse()) {
          double l = dot(v, info.Normal());
          if (l > 0.0) {
            COLOR_VECTOR tempColor = {X(info.Color()) * X(light->Color()),
                                      Y(info.Color()) * Y(light->Color()),
                                      Z(info.Color()) * Z(light->Color())};
            color += tempColor * l;
          }
        }

        // this is the max depth of raytracing
        // increasing depth will calculate more color but it will take
        // exponentially longer
        if (depth < _renderData->TraceDepth()) {
          // calculate reflection ray
          if (_scene->RenderReflection() &&
              info.Element()->GetMaterial()->Reflection() > 0.0) {
            Ray reflectionRay = GetReflectionRay(info.Position(), info.Normal(),
                                                 ray.Direction());
            IntersectionInfo refl =
                TestIntersection(reflectionRay, info.Element());
            if (refl.IsHit() && refl.Distance() > 0.0) {
              // recursive call, this makes reflections expensive
              refl.SetColor(RayTrace(refl, reflectionRay, depth + 1));
            } else {
              refl.SetColor(_scene->GetBackground()->Color());
            }
            BlendColor(color, refl.Color(),
                       info.Element()->GetMaterial()->Reflection());
          }

          // calculate refraction raty
          if (_scene->RenderRefraction() &&
              info.Element()->GetMaterial()->Transparency() > 0.0) {
            Ray refractionRay = GetRefractionRay(
                info.Position(), info.Normal(), ray.Direction(),
                info.Element()->GetMaterial()->Refraction());
            IntersectionInfo refr = info.Element()->Intersect(refractionRay);
            if (refr.IsHit()) {
              refractionRay = GetRefractionRay(
                  refr.Position(), refr.Normal(), refractionRay.Direction(),
                  refr.Element()->GetMaterial()->Refraction());
              refr = TestIntersection(refractionRay, info.Element());
              if (refr.IsHit() && refr.Distance() > 0.0) {
                // recursive call
                refr.SetColor(RayTrace(refr, refractionRay, depth + 1));
              } else {
                refr.SetColor(_scene->GetBackground()->Color());
              }
            } else {
              refr.SetColor(_scene->GetBackground()->Color());
            }
            BlendColor(color, refr.Color(),
                       info.Element()->GetMaterial()->Transparency());
          }

          IntersectionInfo shadow;
          if (_scene->RenderShadow()) {
            // calculate shadow.  create ray from intersection point to light
            Ray shadowRay(info.Position(), v);

            // find any element in between intersection point and light
            shadow = TestIntersection(shadowRay, info.Element());
            if (shadow.IsHit() &&
                shadow.Element()->Id() != info.Element()->Id()) {
              // only cast the shadow if the found intersection is another
              // element than the current element
              color *=
                  0.5 +
                  0.5 * pow(shadow.Element()->GetMaterial()->Transparency(),
                            0.5);  // math.pow(0.5, shadow.hitcount)
            }
          }

          // only show highlights if it is not in the shadow of another object
          if (_scene->RenderHighlights() && !shadow.IsHit() &&
              info.Element()->GetMaterial()->Gloss() > 0.0) {
            // only show gloss light if it is not in shadow of another element.
            // calculate gloss lighting (phong)
            POS_VECTOR lv = info.Element()->Position() - light->Position();
            normalize(lv);
            POS_VECTOR e =
                _scene->GetCamera()->GetPosition() - info.Element()->Position();
            normalize(e);
            POS_VECTOR h = e - lv;
            normalize(h);

            double glossweight =
                pow(std::max(dot(info.Normal(), h), 0.0), shininess);
            color += light->Color() * glossweight;
          }
        }
      });
  return color;
}

/* static */ shared_ptr<RayTracer> RayTracerFactory::CreateSimpleTracer(
    const shared_ptr<RayTraceRenderData>& renderData,
    const shared_ptr<Scene>& scene) {
  return make_shared<SimpleRayTracer>(renderData, scene);
}

Ray SimpleRayTracer::GetReflectionRay(const POS_VECTOR& p, const POS_VECTOR& n,
                                      const POS_VECTOR& v) {
  double c1 = -dot(n, v);
  POS_VECTOR rl = v + (n * 2 * c1);
  Ray r(p, rl);
  return r;
}

Ray SimpleRayTracer::GetRefractionRay(const POS_VECTOR& p, const POS_VECTOR& n,
                                      const POS_VECTOR& v, double refraction) {
  double c1 = dot(n, v);
  double c2 = 1.0 - refraction * refraction * (1.0 - c1 * c1);

  c2 = sqrt(c2);
  POS_VECTOR t = (n * (refraction * c1 - c2) - v * refraction) * -1;
  normalize(t);
  Ray r(p, t);
  return r;
}