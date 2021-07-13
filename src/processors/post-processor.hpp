#pragma once

#include <iostream>
#include <vector>
#include "models/scene.hpp"
#include "utils/vec.hpp"

class PostProcessor {
public:
    size_t width;
    size_t height;

    PostProcessor(const size_t width, const size_t height): width(width), height(height) {}

    virtual void postProcess(Scene &scene) { this->postProcess(scene.renderer->framebuffer); }
    virtual void postProcess(std::vector<vec3f> &) {}
};
