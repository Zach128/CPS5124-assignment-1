#pragma once
#include "models/rays/ray.hpp"
#include "models/cameras/camera.hpp"
#include "models/materials/diffuse.hpp"
#include "renderers/renderer.hpp"
#include "models/primitives/primitive.hpp"
#include "utils/utils.hpp"

class PathRenderer : public Renderer {
public:
    PathRenderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth)
        : Renderer(type, output, dimensions, samples, depth) {}

    PathRenderer() : Renderer() {}
    ~PathRenderer() {}

    void prepare(const Scene &scene);
    void render(const std::shared_ptr<Camera> &camera);

    vec3f cast_ray(const PinholeCamera &camera, const RayInfo &ray, float &dist, size_t depth = 0);
};

void from_json(const json &j, PathRenderer &r);
void from_json(const json &j, std::shared_ptr<PathRenderer> &r);
