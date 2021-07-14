#pragma once

#include <limits>
#include <iostream>
#include <vector>

#include "utils/vec.hpp"
#include "post-processor.hpp"

class LinearToneMapper : public PostProcessor {
public:
    const vec3f LUM_VECTOR = vec3f(0.2126f, 0.7152f, 0.0722f);

    LinearToneMapper(const size_t width, const size_t height): PostProcessor(width, height) {}

    void postProcess(Scene &scene) { this->postProcess(scene.renderer->framebuffer); }
    void postProcess(std::vector<vec3f> &framebuffer) {
        std::cout << "Applying Linear Tonemap" << std::endl;
        const vec3f top(255, 255, 255);
        const vec3f bottom(0, 0, 0);
        vec3f max_color = top, min_color = bottom;
        size_t length = width * height;

        std::vector<float> frame_luminance = std::vector<float>(length);

        preScale(framebuffer, vec3f(1, 1, 1), bottom, min_color, max_color);

        for(size_t i = 0; i < length; i++) {
            framebuffer[i] = (framebuffer[i] / (max_color));
        }

        preScale(framebuffer, top, bottom, min_color, max_color);

        std::cout << "Done" << std::endl;
    }
};
