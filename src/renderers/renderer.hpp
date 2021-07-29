#pragma once

#include <filesystem>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

#include "utils/vec.hpp"
#include "models/rays/ray.hpp"
#include "models/cameras/camera.hpp"
#include "models/shapes/sphere.hpp"
#include "models/primitives/primitive.hpp"
#include "models/lights/light.hpp"
#include "models/materials/diffuse.hpp"
#include "models/materials/specular.hpp"

namespace fs = std::filesystem;
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

    virtual void prepare(const Scene &);
    virtual void render(const std::shared_ptr<Camera> &) {};
    virtual void save();
    void save_framebuffer(const fs::path &path);
    void save_depth(const fs::path &path);

    bool scene_intersect(const RayInfo &ray, vec3f &hit, vec3f &N, float &dist, std::shared_ptr<Primitive> &out);
    virtual vec3f cast_ray(const PinholeCamera &, const RayInfo &, float &, size_t) { return vec3f(0, 0, 0); };
    virtual vec3f cast_ray(const LensCamera &, const RayInfo &, float &, size_t) { return vec3f(0, 0, 0); };

    void compute_diffuse_intensity(const std::shared_ptr<Light> &light, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out);
    void compute_diffuse_intensity(const vec3f &light_dir, const vec3f &light_intensity, const float &light_distance, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out);
    void compute_specular_intensity(const std::shared_ptr<Light> &light, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out);
private:
    void get_paths(const std::string &filePath, fs::path &renderPath, fs::path &depthPath);
};

void from_json(const json &j, Renderer &r);
void from_json(const json &j, std::shared_ptr<Renderer> &r);
