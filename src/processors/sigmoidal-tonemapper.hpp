#pragma once

#include <limits>
#include <iostream>
#include <vector>

#include "utils/vec.hpp"
#include "post-processor.hpp"

class SigmoidalToneMapper : public PostProcessor {
public:

    SigmoidalToneMapper(const size_t width, const size_t height): PostProcessor(width, height) {}

    void postProcess(Scene &scene) { this->postProcess(scene.renderer->framebuffer); }
    void postProcess(std::vector<vec3f> &framebuffer) {
        std::cout << "Applying Sigmoidal Tonemap" << std::endl;

        const vec3f top(255, 255, 255);
        const vec3f bottom(0, 0, 0);

        vec3f max_color = top, min_color = bottom;
        size_t length = width * height;

        // Scale down the colours to the range of 0..1.
        preScale(framebuffer, vec3f(1, 1, 1), bottom, min_color, max_color);

        // Apply the sigmoidal function to each colour.
        for(size_t i = 0; i < length; ++i) {
            framebuffer[i] = (framebuffer[i] / (1 + framebuffer[i]));
        }

        // Scale back up to the original range (0..255).
        preScale(framebuffer, top, bottom, min_color, max_color);

        std::cout << "Done" << std::endl;
    }
};
