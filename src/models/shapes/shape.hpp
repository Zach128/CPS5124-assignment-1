#pragma once
#include <iostream>
#include "utils/vec.hpp"
#include "models/object.hpp"

struct Shape : TypedElement {
    vec3f position;

    Shape(const std::string &id, const std::string &type, vec3f position) : TypedElement(id, type), position(position) {}
};
