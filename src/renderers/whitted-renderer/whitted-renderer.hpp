#pragma once

#include "models/cameras/camera.hpp"
#include "renderers/renderer.hpp"
#include "models/primitive.hpp"

class WhittedRenderer : public Renderer {
private:
    std::vector<vec3f> framebuffer;
    
public:
    WhittedRenderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth)
        : Renderer(type, output, dimensions, samples, depth) {}

    WhittedRenderer() : Renderer() {}
    ~WhittedRenderer() {}

    void prepare(const Scene &scene);
    void render(const std::shared_ptr<Camera> &camera);
    void save();

    vec3f cast_ray(PinholeCamera &camera);
    bool ray_intersect(const Sphere &sphere, float &t0) const;
};

void from_json(const json &j, WhittedRenderer &r);
void from_json(const json &j, std::shared_ptr<WhittedRenderer> &r);
