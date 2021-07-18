#pragma once
#include "models/rays/ray.hpp"
#include "models/cameras/camera.hpp"
#include "models/materials/diffuse.hpp"
#include "renderers/renderer.hpp"
#include "models/primitives/primitive.hpp"

class WhittedRenderer : public Renderer {
public:
    WhittedRenderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth)
        : Renderer(type, output, dimensions, samples, depth) {}

    WhittedRenderer() : Renderer() {}
    ~WhittedRenderer() {}

    void prepare(const Scene &scene);
    void render(const std::shared_ptr<Camera> &camera);

    vec3f cast_ray(const PinholeCamera &camera, const RayInfo &ray, float &dist, size_t depth);
};

void from_json(const json &j, WhittedRenderer &r);
void from_json(const json &j, std::shared_ptr<WhittedRenderer> &r);
