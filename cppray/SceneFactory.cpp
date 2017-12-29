#include "cppray.h"

/* static */
shared_ptr<Scene> SceneFactory::CreateMarblesScene() {
  POS_VECTOR pos = {0.0, 0.0, -5.0};
  POS_VECTOR lookAt = {0.0, 0.0, 1.0};

  auto camera = make_shared<Camera>(pos, lookAt);

  COLOR_VECTOR color = {0.2, 0.2, 0.2};
  auto background = make_shared<Background>(color, 0.2);
  auto scene = make_shared<Scene>(camera, background);

  return scene;
}
