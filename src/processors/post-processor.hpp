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
        const size_t length = width * height;

        for(size_t i = 0; i < length; i++) {
            // Check if any component is greater than our max, saving it if so.
            framebuffer[i].x > out_max.x ? out_max.x = framebuffer[i].x : false;
            framebuffer[i].y > out_max.y ? out_max.y = framebuffer[i].y : false;
            framebuffer[i].z > out_max.z ? out_max.z = framebuffer[i].z : false;

            // Check if any component is less than our min, saving it if so.
            framebuffer[i].x < out_min.x ? out_min.x = framebuffer[i].x : false;
            framebuffer[i].y < out_min.y ? out_min.y = framebuffer[i].y : false;
            framebuffer[i].z < out_min.z ? out_min.z = framebuffer[i].z : false;
        }
    }

    void preScale(std::vector<vec3f> &framebuffer, const vec3f &top, const vec3f &bottom, vec3f &min, vec3f &max) {
        const size_t length = width * height;

        min = top;
        max = bottom;

        // Get the initial min and max colours.
        get_min_max(framebuffer, min, max);

        // Get the min-max range of the current min-max and the target min-max.

        const vec3f maxMinDiff = max - min;
        const vec3f rangeDiff = top - bottom;

        // Convert and scale the given vector relative to the new range.
        for(size_t i = 0; i < length; i++) {
            framebuffer[i] = rangeDiff * (framebuffer[i] - min) / maxMinDiff + bottom;
        }

        min = top, max = bottom;

        // Get the new min and max values.
        get_min_max(framebuffer, min, max);
    }

    virtual void postProcess(Scene &scene) { (*this).postProcess(scene.renderer->framebuffer); }
    virtual void postProcess(std::vector<vec3f> &) {}
};
