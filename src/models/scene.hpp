#pragma once
#include <iostream>
#include "models/cameras/camera.hpp"
#include "models/materials/material.hpp"
#include "models/shapes/shape.hpp"
#include "models/lights/light.hpp"
#include "models/primitives/primitive.hpp"
#include "renderers/renderer.hpp"

class Scene {
public:
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::shared_ptr<Renderer> renderer;
    std::string selectedCamera;

    Scene(const std::vector<std::shared_ptr<Camera>> &cameras, const std::vector<std::shared_ptr<Material>> &materials, const std::vector<std::shared_ptr<Shape>> &shapes, const std::vector<std::shared_ptr<Light>> &lights, const std::shared_ptr<Renderer> &renderer, const std::vector<std::shared_ptr<Primitive>> &primitives, const std::string &selectedCamera)
        : cameras(cameras),
          materials(materials),
          shapes(shapes),
          lights(lights),
          primitives(primitives),
          renderer(renderer),
          selectedCamera(selectedCamera) {}

    Scene() {}

    void prepare();
    void render();
    void save();
};
