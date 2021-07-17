#pragma once
#include <iostream>
#include <memory>

#include "models/materials/material.hpp"
#include "models/object.hpp"
#include "utils/mat.hpp"
#include "models/shapes/shape.hpp"

struct Primitive : TypedElement {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    Matrix44f worldToObject;
    Matrix44f objectToWorld;

    Primitive(const std::string &id, const std::string &type, const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material)
        : TypedElement(id, type),
          shape(shape),
          material(material) {}

};
