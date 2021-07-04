#pragma once
#include <iostream>
#include "models/cameras/camera.hpp"
#include "models/materials/material.hpp"
#include "models/shapes/shape.hpp"
#include "models/lights/light.hpp"
#include "models/primitive.hpp"
#include "renderers/renderer.hpp"

class Scene {
public:
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::shared_ptr<Renderer> renderer;

    Scene(const std::vector<std::shared_ptr<Camera>> &cameras, const std::vector<std::shared_ptr<Material>> &materials, const std::vector<std::shared_ptr<Shape>> &shapes, const std::vector<std::shared_ptr<Light>> &lights, const std::shared_ptr<Renderer> &renderer, const std::vector<std::shared_ptr<Primitive>> &primitives)
        : cameras(cameras),
          materials(materials),
          shapes(shapes),
          lights(lights),
          primitives(primitives),
          renderer(renderer) {}
    
    Scene() {}

    void prepare() {
        renderer->prepare();
    }

    void render() {
        renderer->render(cameras[0], primitives);
    }

    void save() {
        renderer->save();
    }
};
