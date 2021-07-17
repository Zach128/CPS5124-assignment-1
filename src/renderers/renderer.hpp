#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include "utils/vec.hpp"
#include "models/rays/ray.hpp"
#include "models/cameras/camera.hpp"
#include "models/shapes/sphere.hpp"
#include "models/primitive.hpp"
#include "models/materials/diffuse.hpp"
#include "models/materials/specular.hpp"

using json = nlohmann::json;

// Forward declare visited classes.
class PinholeCamera;
class LensCamera;
class Scene;

class Renderer {
public:
    std::string type;
    std::string output;
    int width;
    int height;
    int samples;
    size_t depth;

    std::vector<vec3f> framebuffer;
    std::vector<float> depthbuffer;

    Renderer(const std::string &type, const std::string &output, const vec2i &dimensions, const int samples, const size_t depth) : type(type), output(output), width(dimensions.x), height(dimensions.y), samples(samples), depth(depth) {}
    Renderer() {}
    ~Renderer() {}

    virtual void prepare(const Scene &) {};
    virtual void render(const std::shared_ptr<Camera> &) {};
    virtual void save();
    virtual void save_depth();

    virtual vec3f cast_ray(const PinholeCamera &, const RayInfo &, float &, size_t) { return vec3f(0, 0, 0); };
    virtual vec3f cast_ray(const LensCamera &, const RayInfo &, float &, size_t) { return vec3f(0, 0, 0); };
    virtual bool ray_intersect(const Sphere &, const RayInfo &, float &t0) const { t0 = 0; return false; };
};

void from_json(const json &j, Renderer &r);
void from_json(const json &j, std::shared_ptr<Renderer> &r);
