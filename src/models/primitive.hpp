#pragma once
#include <iostream>
#include <memory>

#include "models/shapes/shape.hpp"
#include "models/materials/material.hpp"
#include "models/object.hpp"

struct Primitive : TypedElement {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;

    Primitive(const std::string &id, const std::string &type, const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material)
        : TypedElement(id, type),
          shape(shape),
          material(material) {}
};
