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

enum RendererType { RENDERER_NONE = -1, RENDERER_WHITTED, RENDERER_PATH };

NLOHMANN_JSON_SERIALIZE_ENUM(RendererType, {
    { RENDERER_NONE, nullptr},
    { RENDERER_WHITTED, "WRT" },
    { RENDERER_PATH, "PT" }
})

class Renderer {
public:
    RendererType type;
    std::string output;
    int width;
    int height;
    int samples;
    size_t depth;

    std::vector<vec3f> framebuffer;
    std::vector<float> depthbuffer;

    Renderer(const RendererType type, const std::string &output, const vec2i &dimensions, const int samples, const size_t depth) : type(type), output(output), width(dimensions.x), height(dimensions.y), samples(samples), depth(depth) {}
    Renderer() {}
    ~Renderer() {}

    virtual void prepare(const Scene &);
    virtual void render(const std::shared_ptr<Camera> &) {};
    virtual void save();
    void save_framebuffer(const fs::path &path);
    void save_depth(const fs::path &path);

    bool scene_intersect(const RayInfo &ray, vec3f &hit, vec3f &N, float &dist, std::shared_ptr<Primitive> &out);
    virtual vec3f cast_ray(const Camera &, const RayInfo &, float &, size_t) { return vec3f(0, 0, 0); };

    void compute_diffuse_intensity(const std::shared_ptr<Light> &light, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out);
    void compute_diffuse_intensity(const vec3f &light_dir, const vec3f &light_intensity, const float &light_distance, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out);
    void compute_specular_intensity(const std::shared_ptr<Light> &light, const RayInfo &ray, const vec3f &hit, const vec3f &N, vec3f &out);
    void compute_glossy(const Camera &camera, const RayInfo &ray, const vec3f &hit, const vec3f &N, const std::shared_ptr<Material> &material, size_t &depth, vec3f &out);
    void compute_fresnel(const Camera &camera, const RayInfo &ray, const vec3f &hit, const vec3f &N, const std::shared_ptr<Material> &material, size_t &depth, vec3f &out);
    vec3f compute_reflection(const Camera &camera, const RayInfo &srcRay, const vec3f &hit, const vec3f &N, const std::shared_ptr<Material> &material, size_t &depth);
    vec3f compute_refraction(const Camera &camera, const RayInfo &srcRay, const vec3f &hit, const vec3f &N, const std::shared_ptr<Material> &material, size_t &depth);

private:
    void get_paths(const std::string &filePath, fs::path &renderPath, fs::path &depthPath);
};

void from_json(const json &j, Renderer &r);
void from_json(const json &j, std::shared_ptr<Renderer> &r);
