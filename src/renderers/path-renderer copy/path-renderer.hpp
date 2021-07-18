#pragma once

#include "models/cameras/camera.hpp"
#include "models/materials/diffuse.hpp"
#include "renderers/renderer.hpp"
#include "models/primitives/primitive.hpp"
#include "models/rays/ray.hpp"

class PathRenderer : public Renderer {
public:
    PathRenderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth)
        : Renderer(type, output, dimensions, samples, depth) {}

    PathRenderer() : Renderer() {}
    ~PathRenderer() {}

    void prepare(const Scene &scene);
    void render(const std::shared_ptr<Camera> &camera);
    void save();

    bool trace(const vec3f &orig, const vec3f &dir, IsectInfo &isect, RayType rayType = primaryRay);

    vec3f cast_ray(const PinholeCamera &camera, const vec3f &orig, const vec3f &dir, float &dist, size_t depth);
    bool ray_intersect(const Sphere &sphere, const vec3f &orig, const vec3f &dir, float &t0) const;
};

void from_json(const json &j, PathRenderer &r);
void from_json(const json &j, std::shared_ptr<PathRenderer> &r);
