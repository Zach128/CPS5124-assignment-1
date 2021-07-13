#pragma once

#include <iostream>
#include <vector>

#include "utils/vec.hpp"
#include "post-processor.hpp"

class BoxBlur : public PostProcessor {
public:
    BoxBlur(const size_t width, const size_t height): PostProcessor(width, height) {}

    void postProcess(Scene &scene) { this->postProcess(scene.renderer->framebuffer); }
    void postProcess(std::vector<vec3f> &framebuffer) {
        std::cout << "Applying Box Blur" << std::endl;

        float kernel[] = {
            1, 1, 1,
            1, 1, 1,
            1, 1, 1
        };

        int indices[9];
        vec3f buffer[9];

        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                // Obtain the colour samples and buffer them.
                indices[0] = ((x - 1 + width) % width + width * ((y - 1 + height) % height)); // location of the UPPER LEFT
				indices[1] = ((x - 0 + width) % width + width * ((y - 1 + height) % height)); // location of the UPPER CENTER
				indices[2] = ((x + 1 + width) % width + width * ((y - 1 + height) % height)); // location of the UPPER RIGHT
				indices[3] = ((x - 1 + width) % width + width * ((y + 0 + height) % height)); // location of the LEFT
				indices[4] = ((x - 0 + width) % width + width * ((y + 0 + height) % height)); // location of the CENTER PIXEL
				indices[5] = ((x + 1 + width) % width + width * ((y + 0 + height) % height)); // location of the RIGHT
				indices[6] = ((x - 1 + width) % width + width * ((y + 1 + height) % height)); // location of the LOWER LEFT
				indices[7] = ((x - 0 + width) % width + width * ((y + 1 + height) % height)); // location of the LOWER CENTER
				indices[8] = ((x + 1 + width) % width + width * ((y + 1 + height) % height)); // location of the LOWER RIGHT

                // Apply the kernel to the buffer.
                buffer[0] = framebuffer[indices[0]] * kernel[0]; // upper left
				buffer[1] = framebuffer[indices[1]] * kernel[1]; // upper mid
				buffer[2] = framebuffer[indices[2]] * kernel[2]; // upper right
				buffer[3] = framebuffer[indices[3]] * kernel[3]; // left
				buffer[4] = framebuffer[indices[4]] * kernel[4]; // center pixel
				buffer[5] = framebuffer[indices[5]] * kernel[5]; // right
				buffer[6] = framebuffer[indices[6]] * kernel[6]; // lower left
				buffer[7] = framebuffer[indices[7]] * kernel[7]; // lower mid
				buffer[8] = framebuffer[indices[8]] * kernel[8]; // lower right

                // Combine the colours
                vec3f new_color = std::reduce(std::cbegin(buffer), std::cend(buffer));

                // Average them.
                new_color.x /= 9.f;
                new_color.y /= 9.f;
                new_color.z /= 9.f;

                framebuffer[x + y * width] = new_color;
            }
        }

        std::cout << "Done" << std::endl;
    }
};
