#pragma once
#include <iostream>
#include "models/cameras/camera.hpp"
#include "models/materials/material.hpp"
#include "models/shapes/shape.hpp"
#include "models/lights/light.hpp"
#include "models/primitive.hpp"

class Scene {
public:
    std::vector<Camera> cameras;
    std::vector<Material> materials;
    std::vector<Shape> shapes;
    std::vector<Light> lights;
    std::vector<Primitive> primitives;

    Scene(const std::vector<Camera> &cameras, const std::vector<Material> &materials, const std::vector<Shape> &shapes, const std::vector<Light> &lights)
        : cameras(cameras),
          materials(materials),
          shapes(shapes),
          lights(lights) {}
    
    void AddPrimitive(const Primitive &primitive) {
        primitives.push_back(primitive);
    }
};
