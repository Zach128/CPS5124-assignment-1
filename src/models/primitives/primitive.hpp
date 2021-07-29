#pragma once
#include <iostream>
#include <memory>

#include "models/materials/material.hpp"
#include "utils/mat.hpp"
#include "models/shapes/shape.hpp"

enum PrimitiveType { PRIMITIVE_NONE = -1, PRIMITIVE_GEOMETRIC, PRIMITIVE_EMISSIVE };

NLOHMANN_JSON_SERIALIZE_ENUM(PrimitiveType, {
    { PRIMITIVE_NONE, nullptr},
    { PRIMITIVE_GEOMETRIC, "geometric" },
    { PRIMITIVE_EMISSIVE, "emissive" }
})
struct Primitive {
    std::string id;
    PrimitiveType type;

    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    Matrix44f worldToObject;
    Matrix44f objectToWorld;

    Primitive(const std::string &id, const PrimitiveType type, const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material)
        :   id(id),
            type(type),
            shape(shape),
            material(material) {}

    virtual vec3f get_emittance() { return vec3f(0, 0, 0); }
};
