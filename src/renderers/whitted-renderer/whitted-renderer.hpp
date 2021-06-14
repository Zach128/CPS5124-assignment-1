#pragma once
#include "renderers/renderer.hpp"

class WhittedRenderer : public Renderer {
private:
    std::vector<vec3f> framebuffer;
    
public:
    WhittedRenderer(const std::string &type, const std::string &output, const vec2i &dimensions, int samples, int depth)
        : Renderer(type, output, dimensions, samples, depth) {}

    WhittedRenderer() : Renderer() {}
    ~WhittedRenderer() {}

    void prepare();
    void render();
    void save();
};

void from_json(const json &j, WhittedRenderer &r);
void from_json(const json &j, std::shared_ptr<WhittedRenderer> &r);
