#pragma once
#include <iostream>
#include "models/shapes/shape.hpp"
#include "models/materials/material.hpp"
#include "models/object.hpp"

struct Primitive : TypedElement {
    Shape shape;
    Material material;

    Primitive(const std::string &id, const std::string &type, const Shape &shape, const Material &material)
        : TypedElement(id, type),
          shape(shape),
          material(material) {}
};
