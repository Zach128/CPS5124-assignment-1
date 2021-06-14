#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/primitive.hpp"

using json = nlohmann::json;

class Renderer {
public:
    std::string type;
    std::string output;
    int width;
    int height;
    int samples;
    int depth;

    Renderer(const std::string &type, const std::string &output, const vec2i &dimensions, const int samples, const int depth) : type(type), output(output), width(dimensions.x), height(dimensions.y), samples(samples), depth(depth) {}
    Renderer() {}
    ~Renderer() {}

    virtual void prepare() {};
    virtual void render(const std::shared_ptr<Camera> &, const std::vector<std::shared_ptr<Primitive>> &) {};
    virtual void save() {};
};

void from_json(const json &j, Renderer &r);
void from_json(const json &j, std::shared_ptr<Renderer> &r);
