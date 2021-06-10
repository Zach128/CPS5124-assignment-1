#pragma once
#include "renderers/renderer.hpp"

class WhittedRenderer : public Renderer {
private:
    const float &width = dimensions.x;
    const float &height = dimensions.y;

    std::vector<vec3f> framebuffer;
public:
    WhittedRenderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth)
        : Renderer(type, output, dimensions, samples, depth) {}

    WhittedRenderer() : Renderer() {}

    void prepare();
    void render();
    void save();
};

void from_json(const json &j, WhittedRenderer &r) {
    j.at("type").get_to(r.type);
    j.at("samples").get_to(r.samples);
    j.at("depth").get_to(r.depth);
    j.at("output").get_to(r.output);
    r.dimensions = vec2i(j.at("dimensions").get<std::vector<int>>().data());
}

void from_json(const json &j, std::shared_ptr<WhittedRenderer> &r) {
    r = std::make_shared<WhittedRenderer>();

    j.at("type").get_to(r->type);
    j.at("samples").get_to(r->samples);
    j.at("depth").get_to(r->depth);
    j.at("output").get_to(r->output);
    r->dimensions = vec2i(j.at("dimensions").get<std::vector<int>>().data());
}
