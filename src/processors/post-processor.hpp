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

    void get_min_max(const std::vector<vec3f> &framebuffer, vec3f &out_min, vec3f &out_max) {
        size_t length = width * height;

        for(size_t i = 0; i < length; i++) {
            framebuffer[i].x > out_max.x ? out_max.x = framebuffer[i].x : false;
            framebuffer[i].y > out_max.y ? out_max.y = framebuffer[i].y : false;
            framebuffer[i].z > out_max.z ? out_max.z = framebuffer[i].z : false;

            framebuffer[i].x < out_min.x ? out_min.x = framebuffer[i].x : false;
            framebuffer[i].y < out_min.y ? out_min.y = framebuffer[i].y : false;
            framebuffer[i].z < out_min.z ? out_min.z = framebuffer[i].z : false;
        }
    }

    void preScale(std::vector<vec3f> &framebuffer, const vec3f &top, const vec3f &bottom, vec3f &min, vec3f &max) {
        const size_t size = width * height;

        const vec3f bias(0.1, 0.1, 0.1);

        min = top;
        max = bottom;

        get_min_max(framebuffer, min, max);

        const vec3f maxMinDiff = max - min;
        const vec3f rangeDiff = top - bottom;

        for(size_t i = 0; i < size; i++) {
            framebuffer[i] = rangeDiff * (framebuffer[i] - min) / maxMinDiff + bottom;
        }

        min = top, max = bottom;

        get_min_max(framebuffer, min, max);
    }

    virtual void postProcess(Scene &scene) { (*this).postProcess(scene.renderer->framebuffer); }
    virtual void postProcess(std::vector<vec3f> &) {}
};
