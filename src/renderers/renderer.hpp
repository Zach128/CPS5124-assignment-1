#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/shapes/sphere.hpp"
#include "models/primitive.hpp"
#include "models/materials/diffuse.hpp"

using json = nlohmann::json;

// Forward declare visited classes.
class PinholeCamera;
class Scene;

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

    virtual void prepare(const Scene &) {};
    virtual void render(const std::shared_ptr<Camera> &) {};
    virtual void save() {};

    virtual vec3f cast_ray(PinholeCamera &) { return vec3f(0, 0, 0); };
    virtual bool ray_intersect(const Sphere &, float &t0) const { t0 = 0; return false; };
    virtual vec3f get_diffuse(const DiffuseMaterial &) { return vec3f(0, 0, 0); };
};

void from_json(const json &j, Renderer &r);
void from_json(const json &j, std::shared_ptr<Renderer> &r);
