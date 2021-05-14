#pragma once
#include <iostream>
#include "models/object.hpp"
#include "utils/vec.hpp"

struct Light : TypedElement {
    vec3f position;
    vec3f intensity;

    Light(const std::string &id, const std::string &type, const vec3f &position, const vec3f &intensity)
        : TypedElement(id, type),
          position(position),
          intensity(intensity) {}
};
