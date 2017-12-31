#include "cppray.h"

/* static */
shared_ptr<Scene> SceneFactory::CreateMarblesScene()
{
  POS_VECTOR pos = {60.0, 7.5, 150.0};
  POS_VECTOR lookAt = {-0.1, 0.1, 0.0};
  POS_VECTOR up = {0.0, 0.0, 1.0};

  auto camera = make_shared<Camera>(pos, lookAt, up, 50.0);

  COLOR_VECTOR color = {0.0, 0.0, 0.0};
  auto background = make_shared<Background>(color, 0.2);
  auto scene = make_shared<Scene>(camera, background);

  // var marbleTexture = Texture.FromFile(Path.Combine(contentRoot, "textures/marble.png"));
  // var texture = new TextureMaterial(marbleTexture, 0.0, 0.0, 1, .5);

  // setup blue spheres from origin positive in X direction
  // setup red spheres from origin positive in Y direction
  // setup green spheres from origin positive in Z direction

  auto blueMaterial = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.0, 0.0, 0.9}, 0.2, 0.0, 2.0, 0.0);
  auto redMaterial = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.9, 0.0, 0.0}, 0.2, 0.0, 2.0, 0.0);
  auto greenMaterial = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.0, 0.9, 0.0}, 0.2, 0.0, 2.0, 0.0);

  double sphereRadius = 2.0;
  double increment = 4.0;
  double maxAxis = increment * 10.0;

  // X spheres
  for (double x = 0.0; x <= maxAxis; x += increment)
  {
    scene->AddShape(make_shared<SphereShape>((POS_VECTOR){x, 0.0, 0.0}, sphereRadius, blueMaterial));
  }
  // Y spheres
  for (double y = 0.0; y <= maxAxis; y += increment)
  {
    scene->AddShape(make_shared<SphereShape>((POS_VECTOR){0.0, y, 0.0}, sphereRadius, redMaterial));
  }
  // Z spheres
  for (double z = 0.0; z <= maxAxis; z += increment)
  {
    scene->AddShape(make_shared<SphereShape>((POS_VECTOR){0.0, 0.0, z}, sphereRadius, greenMaterial));
  }

  // setup a solid reflecting sphere
  // auto solidMaterial = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.0, 0.0, 0.5}, 0.2, 0.0, 2.0, 0.0);
  // auto sphere1 = make_shared<SphereShape>((POS_VECTOR){-0.1, 0.1, 5.0}, 1.0, solidMaterial);
  // scene->AddShape(sphere1);

  // setup sphere with a marble texture from an image
  // auto solidMaterial2 = std::make_shared<SolidMaterial>((COLOR_VECTOR){1.0, 0.0, 0.0}, 0.3, 0.0, 2.0, 0.0);
  // auto sphere2 = make_shared<SphereShape>((POS_VECTOR){1.0, 1.0, -.0}, 4.0, solidMaterial2);
  // scene->AddShape(sphere2);

  // setup the chessboard floor
  auto chessboardMaterial = make_shared<ChessboardMaterial>((COLOR_VECTOR){0.8, 0.8, 0.8}, (COLOR_VECTOR){0.0, 0.0, 0.0}, 0.2, 0.0, 1.0, 0.0, 15.0);
  POS_VECTOR planePos = {1.0, 0.0, 0.0};
  boost::qvm::normalize(planePos);
  auto plane1 = make_shared<PlaneShape>(planePos, 0.0, chessboardMaterial);
  scene->AddShape(plane1);

  // add two lights for better lighting effects
  scene->AddLight((POS_VECTOR){-5.0, 10.0, 10.0}, (COLOR_VECTOR){0.8, 0.8, 0.8});
  scene->AddLight((POS_VECTOR){5.0, 10.0, 10.0}, (COLOR_VECTOR){0.8, 0.8, 0.8});

  return scene;
}

/* static */
/*
shared_ptr<Scene> SceneFactory::CreateMarblesScene()
{
  POS_VECTOR pos = {0.0, 0.0, -15.0};
  POS_VECTOR lookAt = {-0.2, 0.0, 5.0};
  POS_VECTOR up = {0, 1, 0};

  auto camera = make_shared<Camera>(pos, lookAt, up);

  COLOR_VECTOR color = {0.2, 0.2, 0.2};
  auto background = make_shared<Background>(color, 0.2);
  auto scene = make_shared<Scene>(camera, background);

  // var marbleTexture = Texture.FromFile(Path.Combine(contentRoot, "textures/marble.png"));

  // var texture = new TextureMaterial(marbleTexture, 0.0, 0.0, 1, .5);

  // setup a solid reflecting sphere
  auto solidMaterial = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.0, 0.0, 0.5}, 0.2, 0.0, 2.0, 0.0);
  auto sphere1 = make_shared<SphereShape>((POS_VECTOR){-1.5, 0.5, 0.0}, 0.5, solidMaterial);
  scene->AddShape(sphere1);

  // setup sphere with a marble texture from an image
  auto solidMaterial2 = std::make_shared<SolidMaterial>((COLOR_VECTOR){0.8, 0.0, 0.0}, 0.3, 0.0, 2.0, 0.0);
  auto sphere2 = make_shared<SphereShape>((POS_VECTOR){0.0, 0.0, 0.0}, 1, solidMaterial2);
  scene->AddShape(sphere2);

  // setup the chessboard floor
  auto chessboardMaterial = make_shared<ChessboardMaterial>((COLOR_VECTOR){0.8, 0.8, 0.8}, (COLOR_VECTOR){0.0, 0.0, 0.0}, 0.2, 0.0, 1.0, 0.0, 0.7);
  POS_VECTOR planePos = {0.1, 0.9, -0.5};
  boost::qvm::normalize(planePos);
  auto plane1 = make_shared<PlaneShape>(planePos, 1.2, chessboardMaterial);
  scene->AddShape(plane1);

  // add two lights for better lighting effects
  scene->AddLight((POS_VECTOR){5.0, 10.0, -1.0}, (COLOR_VECTOR){0.8, 0.8, 0.8});
  scene->AddLight((POS_VECTOR){-3.0, 5.0, -15.0}, (COLOR_VECTOR){0.8, 0.8, 0.8});

  return scene;
}
*/