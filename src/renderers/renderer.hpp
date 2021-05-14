#pragma once
#include <iostream>
#include "math/vec.hpp"

class Renderer {
    std::string type;
    std::string output;
    vec2i dimensions;

    Renderer(const std::string &type, const std::string &output, const vec2i &dimensions) : type(type), output(output), dimensions(dimensions) {}
};
