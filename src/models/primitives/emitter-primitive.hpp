#pragma once
#include <iostream>
#include <memory>

#include "models/primitives/primitive.hpp"
#include "models/materials/material.hpp"
#include "models/lights/area-light.hpp"
#include "utils/mat.hpp"
#include "models/shapes/shape.hpp"

struct EmitterPrimitive : Primitive {
    std::shared_ptr<AreaLight> light;

    EmitterPrimitive(const std::string &id, const std::string &type, const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material, const std::shared_ptr<AreaLight> &light)
        : Primitive(id, type, shape, material), light(light) {}
};
