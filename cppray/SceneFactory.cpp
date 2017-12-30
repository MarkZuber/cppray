#include "cppray.h"

/* static */
shared_ptr<Scene> SceneFactory::CreateMarblesScene()
{
  POS_VECTOR pos = {0.0, 0.0, -15.0};
  POS_VECTOR lookAt = {-0.2, 0.0, 5.0};
  POS_VECTOR up = {0, 1, 0};

  auto camera = make_shared<Camera>(pos, lookAt, up);

  COLOR_VECTOR color = {0.2, 0.2, 0.2};
  auto background = make_shared<Background>(color, 0.2);
  auto scene = make_shared<Scene>(camera, background);

  /*
  var marbleTexture = Texture.FromFile(Path.Combine(contentRoot, "textures/marble.png"));

  var texture = new TextureMaterial(marbleTexture, 0.0, 0.0, 1, .5);
*/

  // setup a solid reflecting sphere
  auto solidMaterial = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.0, 0.5, 0.5}, 0.2, 0.0, 2.0, 0.0);
  auto sphere1 = make_shared<SphereShape>((POS_VECTOR){-1.5, 0.5, 0.0}, 0.5, solidMaterial);
  scene->AddShape(sphere1);
  // scene->Shapes.Add(new SphereShape(new Vector(-1.5, 0.5, 0), .5, new SolidMaterial(new Vector(0, .5, .5), 0.2, 0.0, 2.0)));

  /*
  // setup sphere with a marble texture from an image
  scene.Shapes.Add(new SphereShape(new Vector(0, 0, 0), 1, texture));
  // scene.Shapes1.Add(new SphereShape(new Vector(0, 0, 0), 1, new SolidMaterial(new Vector(0.3, 0.0, 0.0), 0.7, 0.0, 3.0)));

  // setup the chessboard floor
  scene.Shapes.Add(new PlaneShape(new Vector(0.1, 0.9, -0.5).Normalize(), 1.2, new ChessboardMaterial(new Vector(0.5, 0.5, 0.5), new Vector(0, 0, 0), 0.2, 0, 1, 0.7)));
*/

  // add two lights for better lighting effects
  scene->AddLight((POS_VECTOR){5.0, 10.0, -1.0}, (COLOR_VECTOR){0.8, 0.8, 0.8});
  scene->AddLight((POS_VECTOR){-3.0, 5.0, -15.0}, (COLOR_VECTOR){0.8, 0.8, 0.8});

  return scene;
}
