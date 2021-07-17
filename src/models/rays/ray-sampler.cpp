#include "utils/mat.hpp"
#include "utils/utils.hpp"
#include "utils/vec.hpp"
#include "models/cameras/camera.hpp"
#include "models/rays/ray.hpp"

#include "ray-sampler.hpp"

void RaySampler::get_sample_rays(const size_t x, const size_t y, std::vector<RayInfo> &rays) {
    vec3f cameraDir(0, 0, -1), worldDir;

    for (size_t j = 0; j < samples; j++) {
        // Calculate a uniform sample offset.
        float sx = pixelWidth * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float sy = pixelHeight * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        // Calculate the ray direction of the current pixel in the framebuffer.
        cameraDir.x = (-(2. * (x + .5) / frameWidth - 1)) * scale  + sx;
        cameraDir.y = ((1 - 2 * (y + .5) / frameHeight)) * scale + sy;

        // Convert the ray direction to world space to obtain the true coordinates.
        cameraToWorld.multDirMatrix(cameraDir, worldDir);
        worldDir.normalize();

        rays.push_back(RayInfo(camera.position, worldDir));
    }
}
