#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "utils/vec.hpp"

using json = nlohmann::json;

class Renderer {
public:
    std::string type;
    std::string output;
    vec2i dimensions;
    int samples;
    int depth;

    Renderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth) : type(type), output(output), dimensions(dimensions), samples(samples), depth(depth) {}
    Renderer() {}

    void prepare();
    void render();
    void save();
};

void from_json(const json &j, Renderer &r);
void from_json(const json &j, std::shared_ptr<Renderer> &r);
